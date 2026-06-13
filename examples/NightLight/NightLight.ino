/*
 * MIT License
 * Copyright (c) 2026 [Nicholas Limparis/Snoozebot3000]
 * (See the library source code for full license details)
 * * =========================================================
 * ALS-PT19 Automatic Night Light Example
 * =========================================================
 * * This sketch acts as a simple smart-home device. It monitors 
 * the room's light level and automatically turns on the Arduino's 
 * built-in LED when the room goes dark.
 * * Hardware Hookup:
 * - Sensor VCC pin  -> Arduino 5V
 * - Sensor GND pin  -> Arduino GND
 * - Sensor OUT pin  -> Arduino Analog Pin A0
 * - 10k Ohm Resistor -> Connect between Sensor OUT and GND
 */

#include <ALS_PT19.h>

// Board-specific pin definitions 
// Adjust for your hardware. The ESP32 and ESP8266 have different 
// pinouts for analog input and built-in LEDs.
#if defined(ESP32)
  const int SENSOR_PIN = 35;   // ESP32 analog input (GPIO35/ADC1_CH7)
  const int LED_PIN = 2;        // ESP32 built-in LED (GPIO2)
#elif defined(ESP8266)
  const int SENSOR_PIN = A0;    // ESP8266 analog input
  const int LED_PIN = 2;        // ESP8266 built-in LED (GPIO2)
#else
  // Default: Arduino AVR boards (Uno, Nano, Mega)
  const int SENSOR_PIN = A0;
  const int LED_PIN = LED_BUILTIN;
#endif


// Set the threshold for what you consider "dark". 
// A typical living room at night with lights off is usually below 50 Lux.
const float DARK_THRESHOLD = 50.0; 

// Initialize the sensor for standard indoor lighting with a 10k resistor
ALSPT19<LIGHTING_FLUORESCENT> lightSensor(SENSOR_PIN);

void setup() {
  Serial.begin(9600);
  
  // Prepare the sensor pin
  lightSensor.begin();
  
  // Set the LED pin to act as an output so we can turn it on/off
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW); // Start with the LED turned off
  
  Serial.println("Night Light System Active.");
}

void loop() {
  // Get the current light level in the room
  float lux = lightSensor.readLux();
  
  Serial.print("Current Lux: ");
  Serial.print(lux);
  
  // Check if the current light level has dropped below our threshold
  if (lux < DARK_THRESHOLD) {
    
    // It's dark! Turn the LED ON.
    digitalWrite(LED_PIN, HIGH); 
    Serial.println(" -> It is dark. LED turned ON.");
    
  } else {
    
    // It's bright enough! Turn the LED OFF.
    digitalWrite(LED_PIN, LOW);  
    Serial.println(" -> It is bright. LED turned OFF.");
    
  }
  
  delay(1000); // Wait 1 second before checking again
}