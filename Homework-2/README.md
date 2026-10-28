# Homework 2: ESP32 PWM and OLED Display

## Description
This project uses ESP32 and PlatformIO in Visual Studio Code to control two LEDs using PWM. Task 1 implements LED fading at different frequencies, while Task 2 adds an OLED display to show the live duty-cycle percentages and PWM frequencies.

## Task 1: LED Fading
- LED 1: GPIO 2, 5000 Hz
- LED 2: GPIO 18, 1000 Hz
- 8-bit PWM resolution
- Both LEDs fade in and out continuously.

## Task 2: OLED Display
An SSD1306 OLED display (128 × 64) shows the current duty-cycle percentage and PWM frequency of both LEDs. The display updates automatically as the LEDs fade.

## Hardware Connections
- LED 1: GPIO 2
- LED 2: GPIO 18
- OLED SDA: GPIO 21
- OLED SCL: GPIO 22
- OLED VCC: 3.3V
- OLED GND: GND
- Use a 220Ω resistor for each LED.

## Software and Libraries
- Visual Studio Code
- PlatformIO IDE
- Arduino Framework (C++)
- Wokwi Simulator
- Adafruit SSD1306
- Adafruit GFX Library

## Testing
The project is tested using Wokwi and the Serial Monitor. Both LEDs fade at their configured frequencies, and the OLED displays their duty-cycle percentages and PWM frequencies.

## Conclusion
This project demonstrates PWM control, LED fading, and OLED interfacing using ESP32 in VS Code with PlatformIO.