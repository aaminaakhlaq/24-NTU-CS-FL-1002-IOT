
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDRESS 0x3C

#define OLED_SDA 21
#define OLED_SCL 22

// Push button settings
#define BUTTON_PIN 18
const unsigned long DEBOUNCE_INTERVAL = 50;

// Create OLED display object
Adafruit_SSD1306 display(
    SCREEN_WIDTH,
    SCREEN_HEIGHT,
    &Wire,
    -1
);

// Button and counter variables
int pressCount = 0;

int lastButtonReading = HIGH;
int stableButtonState = HIGH;

unsigned long lastDebounceTime = 0;


// Update the OLED screen
void updateDisplay() {
    display.clearDisplay();

    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);

    display.setCursor(0, 0);
    display.println("ESP32 BUTTON STATUS");
    display.println("--------------------");

    display.print("Button: ");

    if (stableButtonState == LOW) {
        display.println("PRESSED");
    } else {
        display.println("RELEASED");
    }

    display.println();
    display.print("Press count: ");
    display.println(pressCount);

    // Send the new content to the OLED
    display.display();
}


void setup() {
    Serial.begin(115200);

    // Released = HIGH, pressed = LOW
    pinMode(BUTTON_PIN, INPUT_PULLUP);

    // Initialize I2C communication
    Wire.begin(OLED_SDA, OLED_SCL);

    // Initialize the OLED
    if (!display.begin(
            SSD1306_SWITCHCAPVCC,
            OLED_ADDRESS)) {

        Serial.println("OLED initialization failed!");
        Serial.println("Check wiring and I2C address.");

        while (true) {
            // Stop if the display cannot initialize
        }
    }

    // Show the initial screen
    updateDisplay();

    Serial.println("ESP32 OLED project started.");
    Serial.println("Press count: 0");
}


void loop() {
    // Read the physical button
    int currentReading = digitalRead(BUTTON_PIN);

    // If the reading changes, restart the debounce timer
    if (currentReading != lastButtonReading) {
        lastDebounceTime = millis();
        lastButtonReading = currentReading;
    }

    // Accept a reading only after 50 ms of stability
    if (millis() - lastDebounceTime >= DEBOUNCE_INTERVAL) {

        // Check whether the stable state has changed
        if (currentReading != stableButtonState) {
            stableButtonState = currentReading;

            // Count only a new press, not the release
            if (stableButtonState == LOW) {
                pressCount++;

                Serial.print("Accepted press. Count: ");
                Serial.println(pressCount);
            } else {
                Serial.println("Button released.");
            }

            // Refresh the display after a state change
            updateDisplay();
        }
    }
}
