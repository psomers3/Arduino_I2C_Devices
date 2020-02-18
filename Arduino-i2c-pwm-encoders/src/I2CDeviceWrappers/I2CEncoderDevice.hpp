//
//  I2CEncoderDevice.hpp
//  Arduino-i2c-pwm-encoders
//
//  Created by Peter Somers on 2/18/20.
//  Copyright © 2020 Peter Somers. All rights reserved.
//

#ifndef I2CEncoderDevice_hpp
#define I2CEncoderDevice_hpp

#include <stdlib.h>
#include "Wire.h"
#include "I2CDeviceWrapper.hpp"
#include "../Arduino-Rotational-Sensor/AngleSensor.hpp"



class I2CEncoderDevice
{
public:
    enum class Cmd : uint16_t
    {
        GET_POSITION = 0x00,
        GET_SPEED = 0x01,
        ZERO = 0x02,
        SET_ANGLE = 0x03,
    };
    
    I2CEncoderDevice(uint8_t pinA, uint8_t pinB);
    void process_msg(char* msg, uint16_t length);
    static void update_all(){AngleSensor::update_all();}
    
private:
    AngleSensor* _sensor;
    void return_position();
    void return_speed();
    void zero();
    void set_angle();
};


#endif /* I2CEncoderDevice_hpp */
