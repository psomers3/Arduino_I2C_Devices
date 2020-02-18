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
    _sensor = &AngleSensor(pinA, pinB);
}

void I2CEncoderDevice::process_msg(char *msg, uint16_t length)
{
    Cmd option = (Cmd)msg[0];
    switch (option) {
        case Cmd::GET_POSITION:
            return_position();
            break;
        case Cmd::GET_SPEED:
            return_speed();
            break;
        default:
            Wire.write(0xff);
            break;
    }
}

void I2CEncoderDevice::return_position()
{
    uint32_t position = _sensor->get_position();
    Wire.write(reinterpret_cast<char*>(&position), sizeof(position));
}

void I2CEncoderDevice::return_speed()
{
    float speed = _sensor->get_velocity();
    //Serial.print(speed);
    //Serial.print("\n");
    Wire.write(reinterpret_cast<char*>(&speed), sizeof(speed));
}
