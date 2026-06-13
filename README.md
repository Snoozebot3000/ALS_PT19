# ALS-PT19 Ambient Light Sensor Library for Arduino

![Version](https://img.shields.io/badge/version-1.0.0-blue.svg)
![License: MIT](https://img.shields.io/badge/License-MIT-green.svg)

A fast, highly-optimized, and memory-efficient Arduino library for the **Everlight ALS-PT19** ambient light sensor. 

The ALS-PT19 is an excellent, low-cost phototransistor that generates an analog current proportional to ambient illuminance. This library reads that analog output and converts it into a precise **LUX** value. 

Because the sensor is highly sensitive to the Infrared (IR) spectrum, its output changes drastically depending on the light source. This library solves that by offering **compile-time environmental calibration** for Fluorescent/LED vs. Incandescent lighting, ensuring accurate readings without wasting processing power.

## Features
* **Compile-Time Optimization:** Uses C++ templates to select the lighting environment. The compiler completely drops the math and logic for the unused environment, saving flash memory and execution time.
* **SRAM Friendly:** Lookup tables are stored entirely in `PROGMEM` (Flash memory), keeping your precious SRAM free for other tasks.
* **Lightning Fast Math:** Uses bit-shifting and linear interpolation instead of expensive floating-point division to calculate precise LUX values.
* **Hardware Flexible:** Calibrated for a standard 10kΩ pull-down resistor out of the box, but easily accepts custom resistor values (like 7.5kΩ) via the constructor.

## Hardware Hookup

The ALS-PT19 requires a simple pull-down resistor to convert its output current into a readable voltage. 

| Sensor Pin | Connection |
| :--- | :--- |
| **VCC** | Arduino 5V (or 3.3V) |
| **GND** | Arduino GND |
| **OUT** | Arduino Analog Pin (e.g., `A0`) |

**Important:** You must place a pull-down load resistor between the `OUT` pin and `GND`. 
* **10kΩ** is the standard recommended value (and the default for this library).
* If you use a different resistor, you must specify it in your code (see Advanced Usage).

## Installation

**Option 1: Manual ZIP Installation**
1. Click the green **Code** button on this repository and select **Download ZIP**.
2. Open the Arduino IDE.
3. Navigate to **Sketch** -> **Include Library** -> **Add .ZIP Library...**
4. Select the downloaded ZIP file.

**Option 2: Library Manager (If published)**
1. Open the Arduino IDE.
2. Go to **Tools** -> **Manage Libraries...**
3. Search for `ALS_PT19`.
4. Click **Install**.

## Quick Start (Basic Usage)

```cpp
#include <ALS_PT19.h>

// Initialize the sensor on pin A0 for standard indoor/daylight lighting
ALSPT19<LIGHTING_FLUORESCENT> lightSensor(A0);

void setup() {
  Serial.begin(9600);
  lightSensor.begin();
}

void loop() {
  float lux = lightSensor.readLux();
  
  Serial.print("Ambient Light: ");
  Serial.print(lux);
  Serial.println(" LUX");
  
  delay(1000);
}

```

# License

This library is licensed under the MIT License. You are free to use it in personal, educational, and commercial projects.
