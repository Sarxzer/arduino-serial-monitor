import serial
import time

# Open the serial port
ser = serial.Serial('COM4', 9600)

# Wait for 2 seconds to give the Arduino time to get ready
time.sleep(2)

# Send the shutdown signal
ser.write(b'SHUTDOWN')

# Close the serial port
ser.close()