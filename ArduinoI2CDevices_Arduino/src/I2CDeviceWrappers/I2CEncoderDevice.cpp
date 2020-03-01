//
//  I2CEncoderDevice.cpp
//  Arduino-i2c-pwm-encoders
//
//  Created by Peter Somers on 2/18/20.
//  Copyright © 2020 Peter Somers. All rights reserved.
//

#include "I2CEncoderDevice.hpp"

I2CEncoderDevice::I2CEncoderDevice(uint8_t pinA, uint8_t pinB)
{
    _sensor = new AngleSensor(pinA, pinB); // allocate new AngleSensor on heap, could be faster to not put on heap
}

void I2CEncoderDevice::process_msg(char *msg, uint16_t length)
{
    Cmd option = (Cmd)msg[0]; // get the first byte of the msg containing the command
    switch (option) {
        case Cmd::GET_POSITION:
            _float_extract_space = _sensor->get_position();
            Wire.write(reinterpret_cast<char*>(&_float_extract_space), 4);  // write to I2C bus
            break;
            
        case Cmd::GET_ANGLE:
            _float_extract_space = _sensor->return_angle();
            Wire.write(reinterpret_cast<char*>(&_float_extract_space), 4); // write to I2C bus
            break;
            
        case Cmd::GET_SPEED:
            _float_extract_space = _sensor->get_velocity();
            Wire.write(reinterpret_cast<char*>(&_float_extract_space), 4); // write to I2C bus
            break;
            
        case Cmd::SET_DEGREES:
            _sensor->set_degrees((bool)msg[1]);
            Wire.write(0x01); // send arbitrary confirmation byte back to I2C bus

            break;
            
        case Cmd::SET_RESOLUTION:
            // Assumes there is 2 bytes after the command byte containing a uint16_t value.
            // This operation takes a reference to the 2nd byte of the msg, reinterprets it as a pointer to a uint16_t
            // and then de-references the pointer to pass the actual value to the function.
            _sensor->set_pulses_per_rev(*reinterpret_cast<uint16_t*>(&msg[1]));
            Wire.write(0x01);  // send arbitrary confirmation byte back to I2C bus
            break;
            
        default:
            Wire.write(0xff);
            break;
    }
}
