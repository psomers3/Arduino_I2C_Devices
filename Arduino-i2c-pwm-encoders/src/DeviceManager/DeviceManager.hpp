//
//  DeviceManager.hpp
//  Arduino-i2c-pwm-encoders
//
//  Created by Peter Somers on 2/17/20.
//  Copyright © 2020 Peter Somers. All rights reserved.
//

#ifndef DeviceManager_hpp
#define DeviceManager_hpp

#include "stdlib.h"
#include "Wire.h"
#include "../I2CDeviceWrappers/I2CDeviceWrapper.hpp"
#include "../I2CDeviceWrappers/I2CEncoderDevice.hpp"

#define MAX_NUM_SENSORS 10

enum class MsgCmd : uint8_t
{
    NEW_PWM_DEVICE = 0x00,        // CREATE A NEW PWM DEVICE
    NEW_ENCODER_DEVICE = 0x01,    // CREATE A NEW PWM DEVICE
    CLEAR_DEVICES = 0X02,         // DELETE ALL STORED DEVICES
    FWD_TO_DEV = 0x04             // PASS MSG ON TO DEVICE TO HANDLE
};

/**
 A simulated i2C device that will manage passing commands to "devices" on the Arduino.
 
 */
class DeviceManager
{

public:
    /// Constructor
    /**
     */
    DeviceManager(uint16_t encoder_update_rate);
    
    // recieve an I2C message and decide what to do with it
    static void recieve_msg(int how_many);
    static void parse_message();
    
private:
    static DeviceManager* _singleton_handler;
    void add_pwm_device(char* msg, uint16_t length);  // add a pwm device
    void add_encoder_device(char* msg, uint16_t length);
    static char _recieve_msg_buffer[];
    static I2CEncoderDevice* _devices[];
    static uint8_t _num_devices;
    static uint16_t _message_size;
    
};



#endif /* DeviceManager_hpp */
