//
//  I2CBLDCSpeedDevice.cpp
//  Arduino-i2c-pwm-encoders
//
//  Created by Peter Somers on 2/18/20.
//  Copyright © 2020 Peter Somers. All rights reserved.
//

#include "I2CBLDCSpeedDevice.hpp"

I2CBLDCSpeedDevice::I2CBLDCSpeedDevice(uint8_t pinA, uint8_t pinB, uint8_t pinC)
{
    _sensor = new BLDCSpeedSensor(pinA, pinB, pinC); // allocate new SpeedSensor on heap, could be faster to not put on heap
}

void I2CBLDCSpeedDevice::process_msg(char *msg, uint16_t length)
{
    Cmd option = (Cmd)msg[0]; // get the first byte of the msg containing the command
    
    switch (option) {
        case Cmd::GET_SPEED:
            _float_extract_space = _sensor->get_speed();
            Wire.write(reinterpret_cast<char*>(&_float_extract_space), sizeof(_float_extract_space)); // write to I2C bus
            break;
        default:
            Serial.print("default\n");
            Wire.write(0xff);
            break;
    }
}
