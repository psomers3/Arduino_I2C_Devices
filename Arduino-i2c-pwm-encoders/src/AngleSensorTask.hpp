//
//  AngleSensorTask.hpp
//  Arduino-i2c-pwm-encoders
//
//  Created by Peter Somers on 2/17/20.
//  Copyright © 2020 Peter Somers. All rights reserved.
//

#ifndef AngleSensorTask_hpp
#define AngleSensorTask_hpp

#include "stdlib.h"
#include "lib/Arduino-Rotational-Sensor/AngleSensor.hpp"
#include <Task.h>  // MsToTaskTime, Task  -> from Task by Makuna

class AngleSensorTask : public Task
{
public:
    ///Constructor
    /**
     @param angle_sensor A pointer to an AngleSensor object that will be updated at the frequency of sampling_freq.
     @param sampling_freq The frequency in Hz that the AngleSensor should be updated.
     */
    AngleSensorTask(AngleSensor* angle_sensor, sampling_freq): Task(MsToTaskTime((1000/sampling_freq))), _angle_sensor(angle){};
private:
    AngleSensor* _angle_sensor;
    float _sampling_freq;
    
    virtual void OnUpdate(uint32_t deltaTime)
    {
        _angle_sensor->update_velocity(_sampling_freq)
    }
};

#endif /* AngleSensorTask_hpp */
