import gmplot
import serial

latitude_list = []
longitude_list = []

# Tiva-c connected to com port 5
s = serial.Serial('COM5') 

while True:
    readedText = s.readline()
    readedText = readedText.decode("utf-8")
    # Check if reached destination
    if("end" in readedText):
        print(latitude_list)
        print(longitude_list)
        gmap = gmplot.GoogleMapPlotter(30.2887, 31.7257, 13)
        gmap.scatter(latitude_list, longitude_list, '#FF0000', size = 1, marker = True)
        gmap.plot(latitude_list, longitude_list, 'cornflowerblue', edge_width = 2.5)
        gmap.draw( "map.html")
        break
    # Parsing lat and lon then store them
    data = readedText.split(',')
    print('Latitude: {0} | Longitude: {1}'.format(data[0], data[1]))
    latitude_list.append(float(data[0]))
    longitude_list.append(float(data[1]))
print("Finished !")
s.close()





