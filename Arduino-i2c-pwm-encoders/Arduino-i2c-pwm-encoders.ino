//
// Arduino-i2c-pwm-encoders
//
// Turn Arduino into PWM Driver and Quadrature Decoder with I2C communication
// Developed with [embedXcode](https://embedXcode.weebly.com)
//
// Author 		Peter Somers
// 				Peter Somers
//
// Date			2/17/20 10:12 AM
// Version		<#version#>
//
// Copyright	© Peter Somers, 2020
// Licence		<#licence#>
//
// See         ReadMe.txt for references
//


// Core library for code-sense - IDE-based
// !!! Help: http://bit.ly/2AdU7cu
#if defined(ENERGIA) // LaunchPad specific
#include "Energia.h"
#elif defined(TEENSYDUINO) // Teensy specific
#include "Arduino.h"
#elif defined(ESP8266) // ESP8266 specific
#include "Arduino.h"
#elif defined(ARDUINO) // Arduino 1.8 specific
#include "Arduino.h"
#else // error
#error Platform not defined
#endif // end IDE


#include "src/DeviceManager/DeviceManager.hpp"
#include "Wire.h"

#define I2C_ADDR 4

DeviceManager manager(20);

void recieve_test(int howmany);

void setup() {
    Wire.begin(I2C_ADDR); // join i2c bus with address
    Wire.onReceive(manager.recieve_msg);
    Wire.onRequest(manager.parse_message); // register event
    Serial.begin(9600); // open the serial port at 9600 bps:
}

void loop() {}
