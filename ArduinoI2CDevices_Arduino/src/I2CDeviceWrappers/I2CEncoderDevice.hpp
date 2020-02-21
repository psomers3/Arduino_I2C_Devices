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


class I2CEncoderDevice : public I2CDeviceWrapper
{
public:
    enum class Cmd : uint16_t
    {
        GET_POSITION = 0x00,  // Return position in encoder count
        GET_SPEED = 0x01,     // Return angular velocity in either deg/sec or rad/sec depending on setting
        GET_ANGLE = 0x02,     // Return angle of encoder shaft in deg or rad depending on setting
        ZERO = 0x03,          // Set the angle of the encoder to zero
        SET_DEGREES = 0x05,   // Set whether or not to return degrees or radian measurements
        SET_RESOLUTION = 0x06 // Set pulses per revolution of the encoder
    };
    
    /// Constructor.
    /** Creates an encoder device on the ardiuno. Pin A or Pin B needs to be connected to an interrupt enabled pin.
     * @param pinA Pin number of the A signal from encoder.
     *
     * @param pinB Pin number of the B signal from encoder.
     */
    I2CEncoderDevice(uint8_t pinA, uint8_t pinB);
    
    /// Function to process message from I2C bus master device.
    /**
     * @param msg A byte array containing the message from the Master Device.
     *
     * @param length Length of the message in bytes.
     */
    void process_msg(char* msg, uint16_t length);
    
    /// Global update function.
    /** This function is attached to an interrupt to be updated at constant intervals. The member values such as
     ** velocity and position are updated in this function.
     */
    static void update_all(){AngleSensor::update_all();}
    
    /// A clean-up function that is called to remove allocated memory from the heap. Should be called before going out of scope.
    void remove_from_sensors(){_sensor->remove_from_sensors(); delete _sensor;}
    
private:
    AngleSensor* _sensor;  // Actual Encoder sensor
    
    /// Sends position to I2C bus
    void return_position();
    
    /// Sends angular speed to I2C bus
    void return_speed();
    
    /// Sends encoder shaft angle to I2C bus
    void return_angle();
    
    /// Sets _sensor shaft angle to zero
    void zero();
    
    /// Sets units of the device
    /**
     * @param use_degrees When true, sets the device to degree units, when false, radians
     */
    void set_degrees(bool use_degrees);
    
    /// Sets the resolution of the encoder
    /**
     * @param pulses_per_rev Number of pulses of the encoder for one full revolution from one channel (A or B)
     */
    void set_resolution(uint16_t pulses_per_rev);

};


#endif /* I2CEncoderDevice_hpp */
