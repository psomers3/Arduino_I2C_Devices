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
    //Serial.print(how_many);
    //Serial.print(" bytes recieved: ");
    _message_size = how_many;
    
    for (int i=0; i<how_many;i++) // read in each byte from I2C buffer and store it in member buffer
    {
        byte chunk = Wire.read();
        //Serial.print(chunk);
        //Serial.print(" ");
        _recieve_msg_buffer[i] = chunk;
    }
    //Serial.print("\n");
}

void DeviceManager::parse_message()
{
    int device_id;
    // First 4 bits contain the device ID that should get a command if applicable.
    // Second 4 bits contain the overall command that is checked in the switch case below.
    uint8_t option = (uint8_t)_recieve_msg_buffer[0]; // pull first byte
    // Decide what needs to happen.

    switch ((MsgCmd)option) {
            
        case MsgCmd::NEW_SERVO_DEVICE:
            _singleton_handler->add_servo_device(&_recieve_msg_buffer[1], _message_size-1);
            break;
        
        case MsgCmd::NEW_ENCODER_DEVICE:  // create new encoder
            _singleton_handler->add_encoder_device(&_recieve_msg_buffer[1], _message_size-1);
            break;
        
        case MsgCmd::NEW_BLDC_ENCODER_DEVICE:  // create new encoder
            _singleton_handler->add_BLDC_encoder_device(&_recieve_msg_buffer[1], _message_size-1);
            break;
            
        case MsgCmd::CLEAR_DEVICES:  // clear all devices
            clear_devices();
            Wire.write(0xff);
            //Serial.print("clearing devices\n");
            break;
            
        case MsgCmd::FWD_TO_DEV:
            device_id = (int)_recieve_msg_buffer[1];

            if (device_id < _num_devices) // check if device id is valid
                _devices[device_id]->process_msg(&_recieve_msg_buffer[2], _message_size-2);
            else
                Wire.write(0xff); // send bad byte if not because master expects something
            break;
            
        case MsgCmd::SET_ENCODER_UPDATE_FREQ: // update global encoder update rate
            DeviceManager::set_encoder_update_freq((float) *reinterpret_cast<uint16_t*>(&_recieve_msg_buffer[1]));
            Wire.write(0xff);
            break;
            
        default:
            // just send something because master is probably expecting it
            Wire.write(0xff);
            break;
    }
}

void DeviceManager::add_BLDC_encoder_device(char* msg, uint16_t length)
{
    /* decipher rest of message for pin configuration...
     Should be in the form:
     pinA + pinB
     */
    Serial.print("making bldc\n");

    uint8_t pinA = (uint8_t)msg[0];
    uint8_t pinB = (uint8_t)msg[1];
    uint8_t pinC = (uint8_t)msg[2];
    _devices[_num_devices] = new I2CBLDCSpeedDevice(pinA, pinB, pinC);  // create an encoder device
    Wire.write((uint8_t)_num_devices);  // send device index back so program knows id of new device
    _num_devices++;  // increment number of devices
}

void DeviceManager::add_encoder_device(char* msg, uint16_t length)
{
    /* decipher rest of message for pin configuration...
     Should be in the form:
     pinA + pinB
     */
    Serial.print("making encoder\n");

    uint8_t pinA = (uint8_t)msg[0];
    uint8_t pinB = (uint8_t)msg[1];
    _devices[_num_devices] = new I2CEncoderDevice(pinA, pinB);  // create an encoder device
    Wire.write((uint8_t)_num_devices);  // send device index back so program knows id of new device
    _num_devices++;  // increment number of devices
}

static void DeviceManager::set_encoder_update_freq(uint16_t freq)
{
    Timer1.setPeriod((1. / freq) * 1000000); // update timer1 period
    Timer1.attachInterrupt(encoder_update);
    AngleSensor::set_global_update_freq(freq); // update values in AngleSensor class
    BLDCAngleSensor::set_global_update_freq(freq);
}

static void DeviceManager::encoder_update()
{
    I2CEncoderDevice::update_all();
    I2CBLDCEncoderDevice::update_all();
}

void DeviceManager::add_servo_device(char *msg, uint16_t length)
{
    Serial.print("making servo\n");

    uint8_t pin = (uint8_t)msg[0];  // get pin number from msg
    _devices[_num_devices] = new I2CServoDevice(pin);  // create Servo device
    Wire.write(_num_devices);  // send device index back so program knows id of new device
    _num_devices++;  // increment number of devices}
}

void DeviceManager::clear_devices()
{
    for (int i = 0; i < _num_devices; i++)
    {
        _devices[i]->remove_from_sensors(); // run function to take care of any cleanup.
        // ^ this is done because I had issues getting the destructors to play along.
        delete _devices[i];  // deallocate memory from heap
    }
    Wire.write(0xff);
    _num_devices = 0;
}
