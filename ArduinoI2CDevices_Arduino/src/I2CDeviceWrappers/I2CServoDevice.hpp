//
//  I2CServoDevice.hpp
//  ArduinoI2CDevices_Arduino
//
//  Created by Peter Somers on 2/21/20.
//  Copyright © 2020 Peter Somers. All rights reserved.
//

#ifndef I2CServoDevice_hpp
#define I2CServoDevice_hpp

#include <stdlib.h>
#include "I2CDeviceWrapper.hpp"
#include "Wire.h"
#include <Servo.h>
#include <Arduino.h>

class I2CServoDevice: public I2CDeviceWrapper
{
public:
    enum class Cmd : uint16_t
    {
        WRITE = 0x00,      // Send angle to servo
        WRITE_MS = 0x01,   // set microseconds of pulse
        ATTACH = 0x02,     // start servo signal
        DETACH = 0x03,     // stop the servo signal
    };
    
    /// Constructor
    I2CServoDevice(uint8_t pin);
    
    /// process message from I2C bus
    void process_msg(char* msg, uint16_t length);
    
    void remove_from_sensors(){_servo.detach();};
    
private:
    Servo _servo;  // Arduino Servo object
    uint8_t _pin;  // pin number for output signal
};

#endif /* I2CServoDevice_hpp */
