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
int DeviceManager::_message_size = 0;

DeviceManager::DeviceManager(uint16_t encoder_update_rate)
{
    _singleton_handler = this;
    DeviceManager::set_encoder_update_freq(encoder_update_rate);
}

void DeviceManager::recieve_msg(int how_many)
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
    uint8_t option = (uint8_t)_recieve_msg_buffer[0];
    uint8_t parsed_option = option & 0x0f;
    int device_id = (int)((option & 0xf0)>>4);
    
    switch ((MsgCmd)parsed_option) {
        
        case MsgCmd::NEW_ENCODER_DEVICE:
            _singleton_handler->add_encoder_device(&_recieve_msg_buffer[1], _message_size-1);
            break;
        case MsgCmd::CLEAR_DEVICES:
            clear_devices();
            Wire.write(0x00);
            break;
        case MsgCmd::FWD_TO_DEV:            
            if (device_id < _num_devices)
            {
                _devices[device_id]->process_msg(&_recieve_msg_buffer[1], _message_size-1);
            }
            break;
        case MsgCmd::SET_ENCODER_UPDATE_FREQ:
            DeviceManager::set_encoder_update_freq((float) *reinterpret_cast<uint16_t*>(&_recieve_msg_buffer[1]));
            
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
    _devices[_num_devices] = new I2CEncoderDevice(pinA, pinB);
    Wire.write(_num_devices);  // send device index back so program knows id of new device
    _num_devices++;
}

static void DeviceManager::set_encoder_update_freq(uint16_t freq)
{
    Timer1.setPeriod((1. / freq) * 1000000);
    Timer1.attachInterrupt(I2CEncoderDevice::update_all);
    AngleSensor::set_global_update_freq(freq);
}

void DeviceManager::add_pwm_device(char *msg, uint16_t length)
{
    
}

void DeviceManager::clear_devices()
{
    for (int i = 0; i < _num_devices; i++)
    {
        _devices[i]->remove_from_sensors();
        delete _devices[i];
    }
    _num_devices = 0;
}
