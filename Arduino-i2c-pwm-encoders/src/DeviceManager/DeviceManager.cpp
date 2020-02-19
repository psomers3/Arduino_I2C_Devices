//
//  DeviceManager.cpp
//  Arduino-i2c-pwm-encoders
//
//  Created by Peter Somers on 2/17/20.
//  Copyright © 2020 Peter Somers. All rights reserved.
//

#include "DeviceManager.hpp"
#include "TimerOne.h"

/* Initialize static variables */
DeviceManager* DeviceManager::_singleton_handler = 0;
I2CDeviceWrapper* DeviceManager::_devices[MAX_NUM_SENSORS];
char DeviceManager::_recieve_msg_buffer[128];
uint8_t DeviceManager::_num_devices = 0;
int DeviceManager::_message_size = 0;

// SHOULD ONLY NEED TO CREATE ONE DEVICEMANAGER
DeviceManager::DeviceManager(uint16_t encoder_update_rate)
{
    _singleton_handler = this; // give static member pointer to this object
    DeviceManager::set_encoder_update_freq(encoder_update_rate);  // set global update rate for encoders
}

void DeviceManager::recieve_msg(int how_many)
{
    _message_size = how_many;
    for (int i=0; i<how_many;i++) // read in each byte from I2C buffer and store it in member buffer
    {
        byte chunk = Wire.read();
        _recieve_msg_buffer[i] = chunk;
    }
}

void DeviceManager::parse_message()
{
    // First 4 bits contain the device ID that should get a command if applicable.
    // Second 4 bits contain the overall command that is checked in the switch case below.
    uint8_t option = (uint8_t)_recieve_msg_buffer[0]; // pull first byte
    uint8_t parsed_option = option & 0x0f; // pull value of second 4 bits
    int device_id = (int)((option & 0xf0)>>4);  // pull values of first 4 bits
    
    // Decide what needs to happen.
    switch ((MsgCmd)parsed_option) {
        
        case MsgCmd::NEW_ENCODER_DEVICE:  // create new encoder
            _singleton_handler->add_encoder_device(&_recieve_msg_buffer[1], _message_size-1);
            break;
            
        case MsgCmd::CLEAR_DEVICES:  // clear all devices
            clear_devices();
            Wire.write(0x01); // send a random confirmation byte because master is expecting it
            break;
            
        case MsgCmd::FWD_TO_DEV:
            if (device_id < _num_devices) // check if device id is valid
                _devices[device_id]->process_msg(&_recieve_msg_buffer[1], _message_size-1);
            else
                Wire.write(0xff); // send bad byte if not because master expects something
            break;
            
        case MsgCmd::SET_ENCODER_UPDATE_FREQ: // update global encoder update rate
            DeviceManager::set_encoder_update_freq((float) *reinterpret_cast<uint16_t*>(&_recieve_msg_buffer[1]));
            break;
            
        default:
            // just send something because master is probably expecting it
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
    _devices[_num_devices] = new I2CEncoderDevice(pinA, pinB);  // create an encoder device
    Wire.write(_num_devices);  // send device index back so program knows id of new device
    _num_devices++;  // increment number of devices
}

static void DeviceManager::set_encoder_update_freq(uint16_t freq)
{
    Timer1.setPeriod((1. / freq) * 1000000); // update timer1 period
    AngleSensor::set_global_update_freq(freq); // update values in AngleSensor class
}

//void DeviceManager::add_pwm_device(char *msg, uint16_t length)
//{}


void DeviceManager::clear_devices()
{
    for (int i = 0; i < _num_devices; i++)
    {
        _devices[i]->remove_from_sensors(); // run function to take care of any cleanup.
        // ^ this is done because I had issues getting the destructors to play along.
        delete _devices[i];  // deallocate memory from heap
    }
    _num_devices = 0;
}
