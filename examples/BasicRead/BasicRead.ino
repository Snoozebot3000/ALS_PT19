/*
 * MIT License
 * Copyright (c) 2026 [Nicholas Limparis/Snoozebot3000]
 * (See the library source code for full license details)
 * * =========================================================
 * ALS-PT19 Basic Read Example
 * =========================================================
 * * This sketch demonstrates the simplest way to read ambient 
 * light in Lux using the default 10k Ohm load resistor.
 * * Hardware Hookup:
 * - Sensor VCC pin  -> Arduino 5V (or 3.3V)
 * - Sensor GND pin  -> Arduino GND
 * - Sensor OUT pin  -> Arduino Analog Pin A0
 * - 10k Ohm Resistor -> Connect between Sensor OUT and GND
 */

#include <ALS_PT19.h>

// Initialize the sensor on Analog Pin A0.
// 
// <LIGHTING_FLUORESCENT> tells the library we are using this under 
// standard indoor white lighting or daylight.
// 
// Because we didn't provide a second argument, the library assumes
// we are using the standard 10,000 Ohm (10k) pull-down resistor.
// For testing an Adafruit ALS-PT19 breakout was used.
// This breakout includes the 10k resistor, so we can just use the default.
ALSPT19<LIGHTING_FLUORESCENT> lightSensor(A0);

void setup() {
  // Start the serial monitor so we can see the output
  Serial.begin(9600);
  
  // Wait for the serial port to connect (needed for native USB boards like Leonardo)
  while (!Serial); 
  
  // Prepare the analog pin to read the sensor
  lightSensor.begin();
  
  Serial.println("ALS-PT19 Sensor Initialized.");
  Serial.println("Reading light levels...");
}

void loop() {
  // readLux() fetches the analog voltage, applies the lookup table,
  // and returns a precise floating-point Lux value.
  float lux = lightSensor.readLux();
  
  // Print the result to the Serial Monitor
  Serial.print("Ambient Light Level: ");
  Serial.print(lux);
  Serial.println(" Lux");
  
  // Wait 1 second before taking the next reading
  delay(1000); 
}