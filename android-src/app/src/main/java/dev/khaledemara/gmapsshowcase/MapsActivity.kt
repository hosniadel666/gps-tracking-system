package dev.khaledemara.gmapsshowcase

import android.Manifest
import android.annotation.SuppressLint
import android.bluetooth.BluetoothAdapter
import android.bluetooth.BluetoothDevice
import android.content.Intent
import android.content.IntentSender
import android.content.pm.PackageManager
import android.location.Location
import android.os.Bundle
import android.os.Looper
import android.util.Log
import androidx.activity.result.ActivityResult
import androidx.activity.result.ActivityResultLauncher
import androidx.activity.result.IntentSenderRequest
import androidx.activity.result.contract.ActivityResultContracts
import androidx.appcompat.app.AppCompatActivity
import androidx.core.app.ActivityCompat
import androidx.core.content.ContextCompat
import androidx.core.os.HandlerCompat
import com.google.android.gms.common.api.ResolvableApiException
import com.google.android.gms.location.*
import com.google.android.gms.maps.CameraUpdateFactory
import com.google.android.gms.maps.GoogleMap
import com.google.android.gms.maps.OnMapReadyCallback
import com.google.android.gms.maps.SupportMapFragment
import com.google.android.gms.maps.model.JointType
import com.google.android.gms.maps.model.LatLng
import com.google.android.gms.maps.model.PolylineOptions
import com.google.android.gms.maps.model.RoundCap
import com.google.android.gms.tasks.Task
import dev.khaledemara.gmapsshowcase.databinding.ActivityMapsBinding
import dev.khaledemara.gmapsshowcase.util.MyBluetoothService
import java.io.IOException
import java.util.*
import kotlin.collections.ArrayList

class MapsActivity : AppCompatActivity(), OnMapReadyCallback {

    private lateinit var fusedLocationClient: FusedLocationProviderClient
    private lateinit var mMap: GoogleMap
    private lateinit var binding: ActivityMapsBinding
    private lateinit var locationCallback: LocationCallback

    private val bluetoothAdapter: BluetoothAdapter? = BluetoothAdapter.getDefaultAdapter()
    private val connectThread = ConnectThread()

    private val latLngList = ArrayList<LatLng>()

    private val polyline by lazy {
        mMap.addPolyline(
            PolylineOptions()
                .width(resources.displayMetrics.density * 2)
                .color(ContextCompat.getColor(this, R.color.purple_500))
                .startCap(RoundCap())
                .endCap(RoundCap())
                .geodesic(true)
                .jointType(JointType.ROUND)
        )
    }

    private lateinit var locationRequest: LocationRequest
    private lateinit var requestEnableLocationSettingsLauncher: ActivityResultLauncher<IntentSenderRequest>
    private lateinit var requestEnableBtLauncher: ActivityResultLauncher<Intent>

    @SuppressLint("MissingPermission")
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        binding = ActivityMapsBinding.inflate(layoutInflater)
        setContentView(binding.root)

        val mapFragment = supportFragmentManager
            .findFragmentById(R.id.map) as SupportMapFragment
        mapFragment.getMapAsync(this)

        fusedLocationClient = LocationServices.getFusedLocationProviderClient(this)
        locationCallback = object : LocationCallback() {
            override fun onLocationResult(locationResult: LocationResult?) {
                locationResult ?: return
                return
//                for (location in locationResult.locations) {
//                    latLngList.add(LatLng(location.latitude, location.longitude))
//                    polyline?.points = latLngList
//                    mMap.moveCamera(CameraUpdateFactory.newLatLng(latLngList.last()))
//                }
            }
        }

        locationRequest = LocationRequest.create().apply {
            interval = 1000
            fastestInterval = 500
            priority = LocationRequest.PRIORITY_HIGH_ACCURACY
        }
        requestEnableLocationSettingsLauncher =
            registerForActivityResult(ActivityResultContracts.StartIntentSenderForResult()) { result: ActivityResult ->
                if (result.resultCode == RESULT_OK) {
                    requestEnableBt()
                    fusedLocationClient.requestLocationUpdates(
                        locationRequest,
                        locationCallback,
                        Looper.getMainLooper()
                    )
                }
            }
        requestEnableBtLauncher =
            registerForActivityResult(ActivityResultContracts.StartActivityForResult()) { result: ActivityResult ->
                if (result.resultCode == RESULT_OK) {
                    connectThread.start()
                }
            }

        val requestPermissionLauncher =
            registerForActivityResult(
                ActivityResultContracts.RequestPermission()
            ) { isGranted: Boolean ->
                if (isGranted) {
                    requestEnableLocationSettings()
                }
            }

        if (ContextCompat.checkSelfPermission(
                applicationContext,
                Manifest.permission.ACCESS_FINE_LOCATION
            ) == PackageManager.PERMISSION_GRANTED
        ) {
            requestEnableLocationSettings()
        } else {
            requestPermissionLauncher.launch(
                Manifest.permission.ACCESS_FINE_LOCATION
            )
        }
    }

    @SuppressLint("MissingPermission")
    private fun requestEnableLocationSettings() {
        val builder = LocationSettingsRequest.Builder()
            .addLocationRequest(locationRequest)
        val client: SettingsClient = LocationServices.getSettingsClient(this)
        val task: Task<LocationSettingsResponse> = client.checkLocationSettings(builder.build())

        task.addOnSuccessListener {
            requestEnableBt()
            fusedLocationClient.requestLocationUpdates(
                locationRequest,
                locationCallback,
                Looper.getMainLooper()
            )

            if (latLngList.isNotEmpty() && ::mMap.isInitialized) {
                polyline?.points = latLngList
                mMap.moveCamera(CameraUpdateFactory.newLatLngZoom(latLngList.last(), 20F))
            } else {
//                updateFromLastKnownLocation()
            }
        }

        task.addOnFailureListener { exception ->
            if (exception is ResolvableApiException) {
                try {
                    val intentBuilder =
                        IntentSenderRequest.Builder(exception.resolution)

                    requestEnableLocationSettingsLauncher.launch(intentBuilder.build())
                } catch (sendEx: IntentSender.SendIntentException) {
                }
            }
        }
    }

    private fun requestEnableBt() {
        if (bluetoothAdapter?.isEnabled == true) {
            connectThread.start()
        } else {
            val enableBtIntent = Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE)

            requestEnableBtLauncher.launch(enableBtIntent)
        }
    }

    @SuppressLint("MissingPermission")
    private fun updateFromLastKnownLocation() {
        fusedLocationClient.lastLocation
            .addOnSuccessListener { location: Location? ->
                if (location != null) {
                    mMap.moveCamera(
                        CameraUpdateFactory.newLatLngZoom(
                            LatLng(location.latitude, location.longitude),
                            20F
                        )
                    )
                }
            }
    }

    private inner class ConnectThread : Thread() {
        private lateinit var connectedThread: MyBluetoothService.ConnectedThread

        private val stringBuilder = StringBuilder()

        override fun run() {
            var found = false

            while (!found) {
                val pairedDevices: Set<BluetoothDevice>? =
                    bluetoothAdapter?.bondedDevices

                pairedDevices?.forEach { device ->
//                    if (device.address == "98:D3:71:F9:84:A3") {
                    if (device.address == "FC:A8:9A:00:6E:82") {
                        val socket =
                            device.createRfcommSocketToServiceRecord(UUID.fromString("00001101-0000-1000-8000-00805F9B34FB"))

                        socket.let { sock ->
                            try {
                                sock.connect()

                                connectedThread =
                                    MyBluetoothService(HandlerCompat.createAsync(Looper.getMainLooper()) { message ->
                                        Log.v("MainLooperMessage", message.toString())

                                        stringBuilder.append(
                                            (message.obj as ByteArray).toString(
                                                Charsets.US_ASCII
                                            )
                                        )

                                        var startIdx = stringBuilder.indexOf('$')
                                        var endIdx = stringBuilder.indexOf('@', startIdx)

                                        while (startIdx != -1 && endIdx != -1) {
                                            val str =
                                                stringBuilder.removeRange(startIdx, endIdx + 1)
                                            val crd = str.split(',')

                                            if (crd.size == 2) {
                                                val lat = crd[0].toDoubleOrNull()
                                                val lng = crd[1].toDoubleOrNull()

                                                if (lat != null && lng != null) {
                                                    latLngList.add(
                                                        LatLng(
                                                            lat,
                                                            lng
                                                        )
                                                    )
                                                    polyline?.points = latLngList
                                                    mMap.moveCamera(
                                                        CameraUpdateFactory.newLatLng(
                                                            latLngList.last()
                                                        )
                                                    )
                                                }
                                            }

                                            startIdx = stringBuilder.indexOf('$')
                                            endIdx = stringBuilder.indexOf('@', startIdx)
                                        }

                                        true
                                    }).ConnectedThread(
                                        sock
                                    )
                                connectedThread.start()

                                found = true
                            } catch (ex: Exception) {
                                Log.e("Socket Connection", ex.toString())
                            }
                        }
                    }
                }
            }
        }

        fun write(bytes: ByteArray) {
            if (::connectedThread.isInitialized) {
                connectedThread.write(bytes)
            }
        }

        fun cancel() {
            if (::connectedThread.isInitialized) {
                try {
                    connectedThread.cancel()
                } catch (e: IOException) {
                    Log.e("Bluetooth", "Could not close the client socket", e)
                }
            }
        }
    }

    override fun onMapReady(googleMap: GoogleMap) {
        mMap = googleMap

        if (latLngList.isNotEmpty()) {
            polyline?.points = latLngList
            mMap.moveCamera(CameraUpdateFactory.newLatLngZoom(latLngList.last(), 20F))
        } else {
            if (ActivityCompat.checkSelfPermission(
                    this,
                    Manifest.permission.ACCESS_FINE_LOCATION
                ) == PackageManager.PERMISSION_GRANTED
            ) {
//                updateFromLastKnownLocation()
            }
        }
    }

    override fun onDestroy() {
        super.onDestroy()

        connectThread.cancel()
        fusedLocationClient.removeLocationUpdates(locationCallback)
    }
}
