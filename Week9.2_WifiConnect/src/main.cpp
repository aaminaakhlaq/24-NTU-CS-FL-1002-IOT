#include <WiFi.h>

const char* ssid = "Redmi 10A";
const char* password = "A12A45is";

void setup() {

    Serial.begin(115200);

    WiFi.mode(WIFI_STA);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.println("Connecting...");
    }

    Serial.println("Connected!");

    Serial.println(WiFi.localIP());
}

void loop() {

}