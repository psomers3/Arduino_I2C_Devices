//
//  I2CServoDevice.cpp
//  ArduinoI2CDevices_Arduino
//
//  Created by Peter Somers on 2/21/20.
//  Copyright © 2020 Peter Somers. All rights reserved.
//

#include "I2CServoDevice.hpp"

I2CServoDevice::I2CServoDevice(uint8_t pin):_pin(pin), _last_ms_cmd(0), _last_cmd(90){}


void I2CServoDevice::process_msg(char *msg, uint16_t length)
{
    Cmd option = (Cmd)msg[0]; // get the first byte of the msg containing the command
    uint16_t new_ms_cmd;  // variable to hold extracted MS write command
    float new_write_cmd;  // variable to hold extracted write command
    //Serial.print("option: ");
    //Serial.print((uint8_t)option, HEX);
    //Serial.print("\n");
    
    switch (option)
    {
        case Cmd::WRITE:
            //Serial.print("write\n");
            // Assumes there is 4 bytes after the command byte containing a float value.
            // This operation takes a reference to the 2nd byte of the msg, reinterprets it as a pointer to a float
            // and then de-references the pointer to pass the actual value to the function.
            new_write_cmd = *reinterpret_cast<float*>(&msg[1]);
            // only update command if new command is different
            if (new_write_cmd != _last_cmd)
            {
                _servo.writeMicroseconds(new_write_cmd);
                _last_cmd = new_write_cmd;
            }
            Wire.write(0xff);  // send garbage
            break;
            
        case Cmd::WRITE_MS:
            //Serial.print("write_MS\n");
            // Assumes there is 2 bytes after the command byte containing a uint16_t value.
            // This operation takes a reference to the 2nd byte of the msg, reinterprets it as a pointer to a uint16_t
            // and then de-references the pointer to pass the actual value to the function.
            new_ms_cmd = *reinterpret_cast<uint16_t*>(&msg[1]);
            // only update command if new command is different
            if (new_ms_cmd != _last_ms_cmd)
            {
                _servo.writeMicroseconds(new_ms_cmd);
                _last_ms_cmd = new_ms_cmd;
            }
            Wire.write(0xff);  // send garbage
            break;
            
        case Cmd::ATTACH:
            //Serial.print("attaching\n");
            _servo.attach(_pin);
            Wire.write(0xff);  // send garbage
            break;
            
        case Cmd::DETACH:
            //Serial.print("detach\n");
            _servo.detach();
            Wire.write(0xff);  // send garbage
            break;
            
        default:
            //Serial.print("got garbage\n");
            Wire.write(0xff);  // send garbage
            break;
    }
}
