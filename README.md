# GPS-Tracking-System


## Trajectory Plotter Andriod App

We have made an android application which connects to the Tiva through Bluetooth Module and receives a real time location updates from the Tiva and plots the Trajectory on map using Google Maps API.

![Mobile App](https://user-images.githubusercontent.com/35642947/121815329-327a1180-cc76-11eb-9603-d93da0c47ec5.gif)

## Circuit Implementation

<img src="https://user-images.githubusercontent.com/35642947/121815925-06ac5b00-cc79-11eb-94d0-1f2d63a70773.png" width="800" alt="Google Maps Screenshot">

Our project consists of five main components:
1.	TM4C123GH6PM microcontroller
2.	RC1602E character LCD
3.	Ublox NEO-6m GPS Module
4.	HC-05 Bluetooth Module
5.	Trajectory Plotter App


## Pin Configuration

Bluetooth Module PINS  | TM4C123 GPIO PINS
------------- | -------------
+5V | VBUS
GND  | GND
TX  | PE4(uart7_rx)
RX | PE5(uart7_tx)


GPS module PINS  | TM4C123 GPIO PINS
------------- | -------------
VCC | VBUS
GND  | GND
TX  | PB0(uart0_rx)
RX | PB1(uart0_tx)

|                |LCD PINS                          |TM4C123 GPIO PINS                         |
|----------------|-------------------------------|-----------------------------|
|LCD Control |RS            |PE1     |
|LCD Control |RW            |PE2            |
|LCD Control |E            |PE3          |
|LCD Data         |D7         |PD3           |
|LCD Data         |D6         |PD2           |
|LCD Data         |D5         |PD1           |
|LCD Data         |D4         |PD0           |
|Power          | VSS | GND |
|Power          | VDD | VBUS |
|Power          | VO | Potentiometer out |
|Power          | A | VBUS |
|Power          | K | GND |


## System States

The system goes through various states throughout its lifetime
*	Searching state
    *	The initial state of the system after it has been powered up.
    *	The GPS searches for nearest satellites to read accurate locations.
    *	It usually takes around 1min and it could last longer if we were in a closed place.
    *	“Searching..” on the lcd and GREEN led indicate we are in the searching state.

*	Waiting state
    *	We enter the Waiting state after the GPS has initialized correctly and we can get readings now.
    *	We should push SW1 to exits this state and start moving.
    *	“GPS Ready” on the lcd indicates we are in the waiting state.

*	Reading state
    *	When we are moving, and the distance is being accumulated.
    *	the distance we have moved would appear on lcd.
    *	BLUE led indicates we are in the reading state.

*	Reaching state
    *	When we reach our destination, which is Total distance of 100m or more.
    *	The readings shall stop, and we shall not increase the distance again.
    *	RED led indicates we have reached our destination.

*	Error state
    *	This state is rare and should not occur in normal operation.
    *	It occurs if the GPS disconnected, or it cannot find enough satellites to get reading from during the reading state.
    *	“Error: GPS Stopped" on the lcd indicates we are in Error state.
    *	A valid reading from the GPS would exit this state and return to Reading state.

## Screenshots

<img src="https://user-images.githubusercontent.com/35642947/121815748-3a3ab580-cc78-11eb-8d23-0b0551188841.PNG" width="800" alt="Google Maps Screenshot">
<img src="https://user-images.githubusercontent.com/35642947/121815750-3d35a600-cc78-11eb-950f-869874291387.PNG" width="800" alt="Path Map Screenshot">

