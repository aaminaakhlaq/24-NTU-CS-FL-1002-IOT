#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define OLED_ADDRESS 0x3C

Adafruit_SSD1306 display(
    SCREEN_WIDTH,
    SCREEN_HEIGHT,
    &Wire,
    OLED_RESET
);

// LED GPIO pins
constexpr uint8_t LED1_PIN = 2;
constexpr uint8_t LED2_PIN = 18;

// PWM channel numbers
constexpr uint8_t LED1_CHANNEL = 0;
constexpr uint8_t LED2_CHANNEL = 8;

// PWM frequencies
constexpr uint32_t LED1_FREQUENCY = 5000;
constexpr uint32_t LED2_FREQUENCY = 1000;

// PWM resolution
constexpr uint8_t PWM_RESOLUTION = 8;
constexpr uint16_t PWM_MAX_DUTY = 255;

// Current duty values
uint16_t led1Duty = 0;
uint16_t led2Duty = 0;

// Fading directions
int8_t led1Direction = 1;
int8_t led2Direction = 1;

// Independent timing
unsigned long previousLED1Time = 0;
unsigned long previousLED2Time = 0;
unsigned long previousOLEDTime = 0;
unsigned long previousPrintTime = 0;

constexpr unsigned long LED1_INTERVAL = 10;
constexpr unsigned long LED2_INTERVAL = 5;
constexpr unsigned long OLED_INTERVAL = 100;

// Convert actual PWM duty value to percentage
float getDutyPercent(uint16_t duty)
{
    return (duty * 100.0f) / PWM_MAX_DUTY;
}

// Update OLED with current PWM values
void updateOLED()
{
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);

    display.setCursor(0, 0);
    display.println("DUAL LED PWM STATUS");

    display.drawLine(0, 10, 127, 10, SSD1306_WHITE);

    display.setCursor(0, 14);
    display.print("LED1: ");
    display.print(getDutyPercent(led1Duty), 1);
    display.println("%");

    display.setCursor(0, 24);
    display.print("LED1 Freq: ");
    display.print(LED1_FREQUENCY);
    display.println(" Hz");

    display.setCursor(0, 36);
    display.print("LED2: ");
    display.print(getDutyPercent(led2Duty), 1);
    display.println("%");

    display.setCursor(0, 46);
    display.print("LED2 Freq: ");
    display.print(LED2_FREQUENCY);
    display.println(" Hz");

    display.display();
}

void setup()
{
    Serial.begin(115200);

    // Initialize I2C OLED
    Wire.begin(21, 22);

    if (!display.begin(
            SSD1306_SWITCHCAPVCC,
            OLED_ADDRESS))
    {
        Serial.println("OLED initialization failed!");
        while (true)
        {
            delay(1000);
        }
    }

    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println("Initializing PWM...");
    display.display();

    // Configure LED1: 5000 Hz
    ledcSetup(
        LED1_CHANNEL,
        LED1_FREQUENCY,
        PWM_RESOLUTION
    );
    ledcAttachPin(LED1_PIN, LED1_CHANNEL);

    // Configure LED2: 1000 Hz
    ledcSetup(
        LED2_CHANNEL,
        LED2_FREQUENCY,
        PWM_RESOLUTION
    );
    ledcAttachPin(LED2_PIN, LED2_CHANNEL);

    // Start both LEDs at minimum brightness
    ledcWrite(LED1_CHANNEL, led1Duty);
    ledcWrite(LED2_CHANNEL, led2Duty);

    Serial.println("Dual LED PWM Fade Started");
    Serial.println("LED1: 5000 Hz | LED2: 1000 Hz");

    updateOLED();
}

void loop()
{
    unsigned long currentTime = millis();

    // Fade LED1 independently
    if (currentTime - previousLED1Time >= LED1_INTERVAL)
    {
        previousLED1Time = currentTime;

        led1Duty += led1Direction;

        if (led1Duty >= PWM_MAX_DUTY)
        {
            led1Duty = PWM_MAX_DUTY;
            led1Direction = -1;
        }
        else if (led1Duty == 0)
        {
            led1Direction = 1;
        }

        ledcWrite(LED1_CHANNEL, led1Duty);
    }

    // Fade LED2 independently
    if (currentTime - previousLED2Time >= LED2_INTERVAL)
    {
        previousLED2Time = currentTime;

        led2Duty += led2Direction;

        if (led2Duty >= PWM_MAX_DUTY)
        {
            led2Duty = PWM_MAX_DUTY;
            led2Direction = -1;
        }
        else if (led2Duty == 0)
        {
            led2Direction = 1;
        }

        ledcWrite(LED2_CHANNEL, led2Duty);
    }

    // Refresh OLED every 100 milliseconds
    if (currentTime - previousOLEDTime >= OLED_INTERVAL)
    {
        previousOLEDTime = currentTime;
        updateOLED();
    }

    // Serial output for testing
    if (currentTime - previousPrintTime >= 500)
    {
        previousPrintTime = currentTime;

        Serial.printf(
            "LED1: %.1f%% | 5000 Hz || LED2: %.1f%% | 1000 Hz\n",
            getDutyPercent(led1Duty),
            getDutyPercent(led2Duty)
        );
    }
}