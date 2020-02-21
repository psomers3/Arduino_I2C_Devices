//
//  I2CServoDevice.cpp
//  ArduinoI2CDevices_Arduino
//
//  Created by Peter Somers on 2/21/20.
//  Copyright © 2020 Peter Somers. All rights reserved.
//

#include "I2CServoDevice.hpp"

I2CServoDevice::I2CServoDevice(uint8_t pin):_pin(pin){}


void I2CServoDevice::process_msg(char *msg, uint16_t length)
{
    Cmd option = (Cmd)msg[0]; // get the first byte of the msg containing the command
    switch (option) {
        case Cmd::WRITE:
            // Assumes there is 2 bytes after the command byte containing a uint16_t value.
            // This operation takes a reference to the 2nd byte of the msg, reinterprets it as a pointer to a uint16_t
            // and then de-references the pointer to pass the actual value to the function.
            //
            // Also divides by 100 to shift decimal place 2 spots
            Serial.print((*reinterpret_cast<uint16_t*>(&msg[1]))/100.);
            _servo.write((*reinterpret_cast<uint16_t*>(&msg[1]))/100);
            break;
            
        case Cmd::WRITE_MS:
            // Assumes there is 2 bytes after the command byte containing a uint16_t value.
            // This operation takes a reference to the 2nd byte of the msg, reinterprets it as a pointer to a uint16_t
            // and then de-references the pointer to pass the actual value to the function.
            _servo.writeMicroseconds(*reinterpret_cast<uint16_t*>(&msg[1]));
            Wire.write(0x01);  // send confirmation message
            break;
            
        case Cmd::ATTACH:
            _servo.attach(_pin);
            Wire.write(0x01);  // send confirmation message
            break;
            
        case Cmd::DETACH:
            _servo.detach();
            Wire.write(0x01);  // send confirmation message
            break;
            
        default:
            Wire.write(0xff);  // send garbage otherwise
            break;
    }
}
