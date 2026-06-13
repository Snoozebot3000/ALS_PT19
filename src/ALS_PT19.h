/*
 * MIT License
 * Copyright (c) 2026 [Nicholas Limparis/Snoozebot3000]
 * * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef ALS_PT19_H
#define ALS_PT19_H

#include <Arduino.h>

// Only explicitly include the AVR PROGMEM header if we are compiling for an 8-bit AVR chip (Uno R3, Mega, Nano).
// All other modern 32-bit architectures (ESP32, ESP8266, SAMD, nRF, Renesas) handle PROGMEM natively via Arduino.h.
#if defined(__AVR__)
  #include <avr/pgmspace.h>
#endif

/**
 * @enum LightEnv
 * @brief Selects the lighting environment to compensate for IR sensitivity.
 * * The ALS-PT19 is highly sensitive to the Infrared (IR) light emitted by 
 * incandescent bulbs. Choosing the correct environment ensures accurate LUX readings.
 */
enum LightEnv {
    LIGHTING_FLUORESCENT, // Use for Fluorescent tubes, white LEDs, and daylight
    LIGHTING_INCANDESCENT // Use for traditional warm incandescent/halogen bulbs
};

/**
 * @brief Lookup tables stored in Flash Memory (PROGMEM).
 * * Storing these in PROGMEM saves valuable SRAM on microcontrollers like the Arduino Uno.
 * These tables are calibrated based on a 10k Ohm load resistor and map ADC readings 
 * (in steps of 128) directly to LUX values.
 */
const float LUX_TABLE_FLUO[9] PROGMEM = {
    0.0, 417.1, 834.1, 1251.2, 1668.3, 2085.4, 2502.4, 2919.5, 3336.6
};

const float LUX_TABLE_INCAND[9] PROGMEM = {
    0.0, 120.3, 240.6, 360.9, 481.2, 601.5, 721.9, 842.2, 962.5
};

/**
 * @class ALSPT19
 * @brief Driver for the Everlight ALS-PT19 Ambient Light Sensor.
 * * Uses a C++ template so the compiler can highly optimize the code by 
 * completely removing the math for the lighting environment you aren't using.
 */
template <LightEnv Env>
class ALSPT19 {
private:
    uint8_t _pin;               // The analog pin connected to the sensor
    float _resistorMultiplier;  // Scales the reading if a non-10k resistor is used

public:
    /**
     * @brief Constructor for the ALS-PT19 sensor.
     * * @param pin The Arduino Analog pin connected to the sensor's OUT pin (e.g., A0).
     * @param rLoad The value of the pull-down resistor in Ohms. Defaults to 10000.0 (10k).
     */
    ALSPT19(uint8_t pin, float rLoad = 10000.0) : _pin(pin) {
        // Calculate the hardware scaling factor once during setup to save time later.
        // If a 10k resistor is used, this multiplier is exactly 1.0.
        _resistorMultiplier = 10000.0 / rLoad;
    }

    /**
     * @brief Initializes the sensor pin. Call this inside your setup() function.
     */
    void begin() {
        pinMode(_pin, INPUT);
    }

    /**
     * @brief Reads the sensor and calculates the current illuminance.
     * * @return float The ambient light level in LUX.
     */
    float readLux() {
        int adcVal = analogRead(_pin);
        
        // Clamp the ADC value between 0 and 1023 to prevent memory overflow
        if (adcVal < 0) adcVal = 0;
        if (adcVal > 1023) adcVal = 1023;

        // Determine which two points in our lookup table we are between.
        // Bit-shifting right by 7 is a mathematically ultra-fast way to divide by 128.
        uint8_t index = adcVal >> 7; 
        
        // Find exactly how far we are past the lower point (the remainder).
        // Bitwise AND 127 is an ultra-fast way to calculate (adcVal % 128).
        uint8_t remainder = adcVal & 127; 

        float y0, y1;

        // Fetch the appropriate upper and lower bounds from Flash memory
        if (Env == LIGHTING_FLUORESCENT) {
            y0 = pgm_read_float(&LUX_TABLE_FLUO[index]);
            y1 = pgm_read_float(&LUX_TABLE_FLUO[index + 1]);
        } else {
            y0 = pgm_read_float(&LUX_TABLE_INCAND[index]);
            y1 = pgm_read_float(&LUX_TABLE_INCAND[index + 1]);
        }

        // Linear Interpolation: Estimate the exact LUX value between the two points
        float baseLux = y0 + (y1 - y0) * (remainder / 128.0);
        
        // Apply the custom resistor scaling factor and return the final value
        return baseLux * _resistorMultiplier;
    }
};

#endif