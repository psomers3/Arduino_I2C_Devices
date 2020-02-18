//
//  DeviceManager.cpp
//  Arduino-i2c-pwm-encoders
//
//  Created by Peter Somers on 2/17/20.
//  Copyright © 2020 Peter Somers. All rights reserved.
//

#include "DeviceManager.hpp"
#include "TimerOne.h"


DeviceManager* DeviceManager::_singleton_handler = 0;
I2CEncoderDevice* DeviceManager::_devices[MAX_NUM_SENSORS];
char DeviceManager::_recieve_msg_buffer[128];
uint8_t DeviceManager::_num_devices = 0;
uint16_t DeviceManager::_message_size = 0;

DeviceManager::DeviceManager(uint16_t encoder_update_rate)
{
    _singleton_handler = this;
    Timer1.initialize((1. / encoder_update_rate) * 1000000);
    Timer1.attachInterrupt(I2CEncoderDevice::update_all);
}

static void DeviceManager::recieve_msg(int how_many)
{
    _message_size = how_many;
    for (int i=0; i<how_many;i++)
    {
        byte chunk = Wire.read();
        _recieve_msg_buffer[i] = chunk;
    }
}

void DeviceManager::parse_message()
{
    MsgCmd option = (MsgCmd)_recieve_msg_buffer[0];
    switch (option) {
        
        case MsgCmd::NEW_ENCODER_DEVICE:
            _singleton_handler->add_encoder_device(&_recieve_msg_buffer[1], _message_size-1);
            break;
        
        case MsgCmd::FWD_TO_DEV:
            int dev_id =_recieve_msg_buffer[1];
            
            if (_recieve_msg_buffer[1] < _num_devices)
            {
                _devices[dev_id]->process_msg(&_recieve_msg_buffer[2], _message_size-2);
            }
            break;
            
        default:
            // just send something
            Wire.write(0xff);
            break;
    }
}
void DeviceManager::add_encoder_device(char* msg, uint16_t length)
{
    /* decipher rest of message for pin configuration...
     Should be in the form:
     pinA + pinB
     */
    uint8_t pinA = (uint8_t)msg[0];
    uint8_t pinB = (uint8_t)msg[1];
    I2CEncoderDevice new_sensor(pinA, pinB);
    _devices[_num_devices] = &new_sensor;

    Wire.write(_num_devices);  // send device index back so program knows id of new device
    _num_devices++;
}

void DeviceManager::add_pwm_device(char *msg, uint16_t length)
{
    
}
