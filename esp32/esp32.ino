#include <Wire.h>
#include <U8g2lib.h>

#define SDA_PIN 21
#define SCL_PIN 22

// Define the I2C pins for the OLED display with screen resolution 128x64
U8G2_SH1106_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL_PIN, /* data=*/ SDA_PIN, /* reset=*/ U8X8_PIN_NONE);

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(400);
  u8g2.begin();
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.drawStr(0, 10, "OLED initialized");
  u8g2.sendBuffer();
}

void loop() {
  u8g2.print("Waiting for data...");
  if (Serial.available()) {
    String data = Serial.readString();
    if (data.indexOf("INFO:") != -1) { // Wait for the start of the serial data (INFO:)

      // Extract CPU, Memory, GPU, and Temperature information from the serial data
      String cpuInfo = data.substring(data.indexOf("CPU:") + 4, data.indexOf(",MEM_RAW:"));
      String memInfo = data.substring(data.indexOf("MEM_RAW:") + 8, data.indexOf(",MEM_TOTAL:"));
      String memTotalInfo = data.substring(data.indexOf("MEM_TOTAL:") + 10, data.indexOf(",GPU:"));
      String gpuInfo = data.substring(data.indexOf("GPU:") + 4, data.indexOf(",TEMP:"));
      String tempInfo = data.substring(data.indexOf("TEMP:") + 5, data.indexOf(",TIME:"));
      String timeInfo = data.substring(data.indexOf("TIME:") + 5, data.indexOf(",VOL:"));
      String volInfo = data.substring(data.indexOf("VOL:") + 4);

      // Display CPU, Memory, GPU, and Temperature information on the OLED screen
      u8g2.clearBuffer();
      u8g2.setCursor(0, 10);
      u8g2.print("CPU:" + cpuInfo + "%");
      u8g2.setCursor(0, 25);
      u8g2.print("MEM:" + memInfo + "GB/" + memTotalInfo + "GB");
      u8g2.setCursor(0, 40);
      u8g2.print("GPU:" + gpuInfo + "%" + " TEMP:" + tempInfo + "°C");
      u8g2.setCursor(0, 64);
      u8g2.print(timeInfo);
      u8g2.setCursor(64, 64);
      u8g2.print("VOL:" + volInfo + "%");
      u8g2.sendBuffer();
    } else if (data.indexOf("DISPLAY_OFF") != -1) { // Clear the OLED screen
      u8g2.clearBuffer();
      u8g2.sendBuffer();
    } else if (data.indexOf("REBOOT") != -1) { // Reboot the ESP32
      ESP.restart();
    } else if (data.indexOf("SHUTDOWN") != -1) { // Shutdown the ESP32
      esp_deep_sleep_start();
    } else {
      Serial.println("Invalid command");
    }
  }
}
