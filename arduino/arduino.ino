#include <Wire.h>
#include <U8g2lib.h>

// Define the I2C pins for the OLED display with screen resolution 128x64
U8G2_SH1106_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);

void setup() {
    Serial.begin(9600);
    Serial.setTimeout(400);
    u8g2.begin();
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_ncenB08_tr);
    u8g2.drawStr(0,10,"OLED initialized");
    u8g2.sendBuffer();
}

void loop() {
    u8g2.print("Waiting for data...");
    if (Serial.available()) {
        String data = Serial.readString(); // Read the serial data

        // Extract CPU, Memory, GPU, and Temperature information from the serial data
        String cpuInfo = data.substring(data.indexOf("CPU:") + 4, data.indexOf(",MEM:"));
        String memInfo = data.substring(data.indexOf("MEM_RAW:") + 8, data.indexOf(",MEM_TOTAL:"));
        String memTotalInfo = data.substring(data.indexOf("MEM_TOTAL:") + 10, data.indexOf(",GPU:"));
        String gpuInfo = data.substring(data.indexOf("GPU:") + 4, data.indexOf(",TEMP:"));
        String tempInfo = data.substring(data.indexOf("TEMP:") + 5, data.indexOf(",TIME:"));
        String timeInfo = data.substring(data.indexOf("TIME:") + 5);

        // Display CPU, Memory, GPU, and Temperature information on the OLED screen
        u8g2.clearBuffer();
        u8g2.setCursor(0,10);
        u8g2.print("CPU:" + cpuInfo + "%");
        u8g2.setCursor(0,25);
        u8g2.print("MEM:" + memInfo + "GB/" + memTotalInfo + "GB");
        u8g2.setCursor(0,40);
        u8g2.print("GPU:" + gpuInfo + "%" + " TEMP:" + tempInfo + "°C");
        u8g2.setCursor(0,64);
        u8g2.print(timeInfo);
        u8g2.sendBuffer();
    }
}