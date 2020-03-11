
// Arduino-i2c-pwm-encoders
//
// Turn Arduino into PWM Driver and Quadrature Decoder with I2C communication
//
// Author 		Peter Somers
//
// Date			2/17/20 10:12 AM
//
// Copyright	© Peter Somers, 2020

#include "Arduino.h"
#include "./src/DeviceManager/DeviceManager.hpp"
#include "Wire.h"

#define I2C_ADDR 4  // Statically set Arduino actual I2C address

// Create the class that manages the I2C messages initialized with 20 Hz Encoder update timer
DeviceManager manager(20);

void setup() {
    Wire.begin(I2C_ADDR); // join i2c bus with address
    Wire.onReceive(manager.recieve_msg); // pull mesage from I2C
    Wire.onRequest(manager.parse_message); // parse message from I2C
    //Serial.begin(57600);
    //Serial.print("connected\n");
}
void loop() {}
