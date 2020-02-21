//
//  I2CDeviceWrapper.hpp
//  Index
//
//  Created by Peter Somers on 2/18/20.
//  Copyright © 2020 Peter Somers. All rights reserved.
//

#ifndef I2CDeviceWrapper_hpp
#define I2CDeviceWrapper_hpp

#include <stdlib.h>
#include <stdint.h>

/// A base class for inheriting simulated devices on the Arduino.
/** All virtual functions MUST be implemented in their inherited classes.
 */
class I2CDeviceWrapper
{
public:
    /// Virtual Destructor.
    virtual ~I2CDeviceWrapper(){}
    
    /// A function to process a recieved message from the I2C bus
    /**
     * @param msg A byte array containing the message
     *
     * @param length The length of msg array.
     */
    virtual void process_msg(char* msg, uint16_t length);
    
    /// A function that is called when the devices are cleared from the Arduino.
    /** This is not virtual in case a sub-class does not need to do anything special upon removal.
     */
    void remove_from_sensors(){};
};


#endif /* I2CDeviceWrapper_hpp */
