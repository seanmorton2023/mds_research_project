import serial
import time
arduino = serial.Serial(port='COM4', baudrate=115200, timeout=.1)
def write_read(x):
    arduino.write(x.encode())
    time.sleep(0.05)
    data = arduino.readline()
    return data

#def write_num(x):
#    arduino.write(bytes(x, 'utf-8'))
#    time.sleep(0.05)
#    data = arduino.readline()
#    return data

concat = ""
obj = input("Enter an object: ") # Taking input from user
concat += obj

x_coord = input("Enter the x coordinate: ") # Taking input from user
concat += ","
concat += x_coord

y_coord = input("Enter the y coordinate: ") # Taking input from user
concat += ","
concat += y_coord
concat += ","
value = write_read(concat)
print(value) # printing the value