# Importing libraries
import serial
import time
arduino = serial.Serial(port='COM5', baudrate= 9600, timeout=0.1)
#def write_read(x):
    #arduino.write(bytes(x + '\n', 'utf-8'))
    #time.sleep(0.05)
    #data = arduino.readline()
    #return data
while True:
    #num = input("Enter a number: ") # Taking input from user
    #value = write_read(num)
    #print(value)
    arduino.write(bytes('1\r', 'utf-8'))
    time.sleep(0.05)
    data = arduino.readline()
    print(data)
    time.sleep(2)
    arduino.write(bytes('0\r', 'utf-8'))
    time.sleep(0.05)
    data = arduino.readline()
    print(data)
    time.sleep(2)
