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
    _sensor = new AngleSensor(pinA, pinB);
}

void I2CEncoderDevice::process_msg(char *msg, uint16_t length)
{
    Cmd option = (Cmd)msg[0];
    switch (option) {
        case Cmd::GET_POSITION:
            return_position();
            break;
            
        case Cmd::GET_ANGLE:
            return_angle();
            break;
            
        case Cmd::GET_SPEED:
            return_speed();
            break;
            
        case Cmd::SET_DEGREES:
            set_degrees((bool)msg[1]);
            break;
            
        case Cmd::SET_RESOLUTION:
            set_resolution(*reinterpret_cast<uint16_t*>(&msg[1]));
            break;
        default:
            Wire.write(0xff);
            break;
    }
}

void I2CEncoderDevice::return_position()
{
    int32_t position = _sensor->get_position();
    Wire.write(reinterpret_cast<char*>(&position), sizeof(position));
}

void I2CEncoderDevice::return_speed()
{
    float speed = _sensor->get_velocity();
    Wire.write(reinterpret_cast<char*>(&speed), sizeof(speed));
}

void I2CEncoderDevice::return_angle()
{
    float angle = _sensor->return_angle();
    Wire.write(reinterpret_cast<char*>(&angle), sizeof(angle));
}

void I2CEncoderDevice::set_degrees(bool use_degrees)
{
    _sensor->set_degrees(use_degrees);
    Wire.write(0x01);
}

void I2CEncoderDevice::set_resolution(uint16_t pulses_per_rev)
{
    _sensor->set_pulses_per_rev(pulses_per_rev);
    Wire.write(0x01);
}
