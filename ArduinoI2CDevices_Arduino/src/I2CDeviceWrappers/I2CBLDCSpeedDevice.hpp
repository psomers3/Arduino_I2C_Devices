//
//  I2CBLDCSpeedDevice.hpp
//  Arduino-i2c-pwm-encoders
//
//  Created by Peter Somers on 2/18/20.
//  Copyright © 2020 Peter Somers. All rights reserved.
//

#ifndef I2CBLDCSpeedDevice_hpp
#define I2CBLDCSpeedDevice_hpp

#include <stdlib.h>
#include "Wire.h"
#include "I2CDeviceWrapper.hpp"
#include "../Arduino-Rotational-Sensor/BLDCSpeedSensor.hpp"


class I2CBLDCSpeedDevice : public I2CDeviceWrapper
{
public:
    enum class Cmd : uint16_t
    {
        GET_SPEED = 0x01,     // Return angular velocity in either deg/sec or rad/sec depending on setting
    };
    
    /// Constructor.
    /** Creates an encoder device on the ardiuno. Pin A or Pin B needs to be connected to an interrupt enabled pin.
     * @param pinA Pin number of the A signal from encoder.
     *
     * @param pinB Pin number of the B signal from encoder.
     */
    I2CBLDCSpeedDevice(uint8_t pinA, uint8_t pinB, uint8_t pinC);
    
    /// Function to process message from I2C bus master device.
    /**
     * @param msg A byte array containing the message from the Master Device.
     *
     * @param length Length of the message in bytes.
     */
    void process_msg(char* msg, uint16_t length);
    
    /// A clean-up function that is called to remove allocated memory from the heap. Should be called before going out of scope.
    void remove_from_sensors(){_sensor->remove_from_sensors(); delete _sensor;}
    
private:
    BLDCSpeedSensor* _sensor;  // Actual BLDCSpeed sensor
    
    // The following variable is just for constant allocated memory to extract values from _sensor
    float _float_extract_space;
};


#endif /* I2CBLDCSpeedDevice_hpp */
