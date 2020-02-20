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


// Enum class containing the possible commands to come from the Master Device
enum class MsgCmd : uint8_t
{
    NEW_PWM_DEVICE = 0x00,         // CREATE A NEW PWM DEVICE
    NEW_ENCODER_DEVICE = 0x01,     // CREATE A NEW ENCODER DEVICE
    CLEAR_DEVICES = 0X02,          // DELETE ALL STORED DEVICES
    FWD_TO_DEV = 0x04,             // PASS MSG ON TO DEVICE TO HANDLE
    SET_ENCODER_UPDATE_FREQ = 0x05 // SET GLOBAL ENCODER UPDATE RATE IN Hz
};

/**
 A simulated i2C device that will manage passing commands to "devices" on the Arduino.
 
 */
class DeviceManager
{

public:
    /// Constructor
    /**
     * @param encoder_update_rate Global update rate for any encoders.
     */
    DeviceManager(uint16_t encoder_update_rate);
    
    /// Recieve an I2C message and decide what to do with it.
    /** This function is called by the OnReceipt() function from the Wire.h lib
     * @param how_many The number of bytes of the recieved message
     */
    static void recieve_msg(int how_many);
    
    /// The main switching function to decide what to do with the recieved message.
    /** This function should be called by OnRequest() from the Wire.h lib
     */
    static void parse_message();
    
    /// Sets the global update rate of any encoders that have been created
    /**
     * @param freq The gloabal update rate in Hz. Unfortunately, I do not have a good way to implement this
     * in a non-integer fashion due to message size limits from the partnering python library.
     */
    static void set_encoder_update_freq(uint16_t freq);
    
    /// Removes all virtual devices that have been configured on the Arduino.
    /** This is best done at the start of any script from the master device
     */
    static void clear_devices();
    
private:
    static DeviceManager* _singleton_handler; // a static reference to itself as there should not be more than 1 instance
    static char _recieve_msg_buffer[];  // a buffer to hold I2C messages
    static I2CDeviceWrapper* _devices[];  // a buffer to store any created devices
    static uint8_t _num_devices;  // number of devices in _devices
    static int _message_size;  // total size of the recieved I2C message
    
    /// This function adds a pwm device.
    //void add_pwm_device(char* msg, uint16_t length);
    
    /// This function adds an encoder device
    /**
     * @param msg This a pointer to the starting point of the recieved I2C message that is required for
     *            specifying the encoder. (i.e. pin A and pin B)
     * @param length The length of msg.
     */
    void add_encoder_device(char* msg, uint16_t length);
    

};



#endif /* DeviceManager_hpp */
