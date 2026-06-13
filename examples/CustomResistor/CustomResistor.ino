/*
 * MIT License
 * Copyright (c) 2026 [Nicholas Limparis/Snoozebot3000]
 * (See the library source code for full license details)
 * * =========================================================
 * ALS-PT19 Custom Resistor & Environment Example
 * =========================================================
 * * This sketch demonstrates two advanced features:
 * 1. How to calibrate the sensor for an INCANDESCENT light source.
 * 2. How to use a custom load resistor (e.g., 7.5k Ohms instead of 10k).
 * * Hardware Hookup:
 * - Sensor VCC pin   -> Arduino 5V
 * - Sensor GND pin   -> Arduino GND
 * - Sensor OUT pin   -> Arduino Analog Pin A1
 * - 7.5k Ohm Resistor -> Connect between Sensor OUT and GND
 * * Note: The ALS-PT19 datasheet occasionally references a 7.5k Ohm load resistor for specific transient response profiles.
 * * Additionally, this example uses an alternate analog pin (A1) to demonstrate that the library can be used on any analog input.
 */

#include <ALS_PT19.h>

const int SENSOR_PIN = A1;

// Define your custom resistor value here in Ohms.
// The datasheet occasionally references 7.5k (7500 Ohms) for specific transient profiles.
const float RESISTOR_VALUE = 7500.0; 

// Initialize the sensor:
// - <LIGHTING_INCANDESCENT> applies a different lookup table to ignore the heavy IR
//   light put out by traditional warm lightbulbs.
// - Passing RESISTOR_VALUE tells the library to recalculate its internal math 
//   to perfectly match your custom hardware setup.
ALSPT19<LIGHTING_INCANDESCENT> lightSensor(SENSOR_PIN, RESISTOR_VALUE);

void setup() {
  Serial.begin(9600);
  
  // Prepare the analog pin
  lightSensor.begin();
  
  Serial.println("Incandescent ALS-PT19 Sensor Initialized.");
  Serial.println("Calibrated for a 7.5k Ohm resistor.");
}

void loop() {
  // Retrieve the compensated LUX reading
  float lux = lightSensor.readLux();
  
  Serial.print("Calibrated Reading: ");
  Serial.print(lux);
  Serial.println(" Lux");
  
  delay(1000); // Read once a second
}