# ALS-PT19 Ambient Light Sensor Library for Arduino

![Version](https://img.shields.io/badge/version-1.0.0-blue.svg)
![License: MIT](https://img.shields.io/badge/License-MIT-green.svg)
[![Compile Sketches](https://github.com/Snoozebot3000/ALS_PT19/actions/workflows/compile.yml/badge.svg)](https://github.com/Snoozebot3000/ALS_PT19/actions/workflows/compile.yml)
[![Arduino Lint](https://github.com/Snoozebot3000/ALS_PT19/actions/workflows/lint.yml/badge.svg)](https://github.com/Snoozebot3000/ALS_PT19/actions/workflows/lint.yml)

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

### Important:
You must place a pull-down load resistor between the `OUT` pin and `GND`.

* **10kΩ** is the standard recommended value (and the default for this library).
* If you use a different resistor, you must specify it in your code (see Advanced Usage).

## Installation
<!-- Comment: This is following the standard Arduino library installation instructions. The "Option 2: Library Manager" section can be left in place even if the library isn't published there yet, as it provides a clear path for future users once it is available.
-->

### Option 1: Manual ZIP Installation

1. Click the green **Code** button on this repository and select **Download ZIP**.
2. Open the Arduino IDE.
3. Navigate to **Sketch** -> **Include Library** -> **Add .ZIP Library...**
4. Select the downloaded ZIP file.

### Option 2: Library Manager (If published)

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

# API Reference
## Environments (Template Parameter)

You must provide one of these two flags inside the angle brackets < > when you create the sensor object:

* LIGHTING_FLUORESCENT: Use this for standard white LEDs, fluorescent tubes, and natural daylight.

* LIGHTING_INCANDESCENT: Use this for traditional warm incandescent or halogen bulbs, which emit heavy infrared (IR) light.

## Constructor

```C++

ALSPT19<Environment> sensorName(uint8_t pin, float rLoad = 10000.0);
```

* pin: The analog pin connected to the sensor's OUT pin.

* rLoad (Optional): The value of your pull-down resistor in Ohms. If left blank, it defaults to 10000.0 (10kΩ).

## Methods

* void begin(): Configures the analog pin. Call this inside your setup() function.

* float readLux(): Reads the analog pin, performs interpolation, applies resistor scaling, and returns the current illuminance in LUX.

## Advanced Usage (Custom Resistors)

If your specific hardware design uses a different resistor (for example, a 7.5kΩ resistor to change the transient profile), you can pass that exact value to the library so it can mathematically correct the LUX output.

```C++

#include <ALS_PT19.h>

// Tell the library you are using a 7500 Ohm resistor
ALSPT19<LIGHTING_FLUORESCENT> lightSensor(A1, 7500.0);

void setup() {
  lightSensor.begin();
}
// readLux() will automatically scale its output!
```

## Examples Included

Check the examples/ folder in the Arduino IDE (File -> Examples -> ALS_PT19) for more details:

* BasicRead: Standard usage with a 10k resistor.

* CustomResistor: How to calibrate for an incandescent environment and a non-standard resistor.

* NightLight: A smart-home style example that turns on an LED when the room goes dark.

## License

This library is licensed under the MIT License. You are free to use it in personal, educational, and commercial projects.
