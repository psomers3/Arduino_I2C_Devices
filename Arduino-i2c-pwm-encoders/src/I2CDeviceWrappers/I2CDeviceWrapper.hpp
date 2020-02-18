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

class I2CDeviceWrapper
{
    
public:
    virtual void process_msg(char* msg, uint16_t length);
};


#endif /* I2CDeviceWrapper_hpp */
