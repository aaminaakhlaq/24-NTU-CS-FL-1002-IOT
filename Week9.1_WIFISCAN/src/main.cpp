#include <WiFi.h>

void setup() {

    Serial.begin(115200);

    WiFi.mode(WIFI_STA);

    WiFi.disconnect();

    delay(1000);

}

void loop() {

    Serial.println("Scanning WiFi networks...");

    int count = WiFi.scanNetworks();

    if (count == 0) {

        Serial.println("No networks found");

    }

    else {

        Serial.print("Networks found: ");

        Serial.println(count);

    }

    for (int i = 0; i < count; i++) {

        Serial.print("SSID: ");

        Serial.println(WiFi.SSID(i));

        Serial.print("RSSI: ");

        Serial.println(WiFi.RSSI(i));

        Serial.print("Channel: ");

        Serial.println(WiFi.channel(i));

        Serial.println("----------------");

    }

    WiFi.scanDelete();

    delay(5000);

}