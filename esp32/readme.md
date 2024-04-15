Ide settings:

- Set the board to `ESP32 Dev Module`
- Don't touch all the other settings
- Set the serial port to the one that the ESP32 is connected to (like `/dev/ttyUSB0` or `COM3`)
- Set the baud rate to `9600`

To upload the code to the ESP32, press the `boot` button on the ESP32, then press the `upload` button in the IDE. (DO NOT release the `boot` button until the LED on the ESP32 starts blinking rapidly).
If the upload is unsuccessful, try closing all the serial monitors and other programs that are using the serial port. (Especially the computer side python program that is running the serial monitor)
The code will be uploaded to the ESP32 and it will start running.