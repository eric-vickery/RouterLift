#ifndef STEPPER_MONITOR_H_INCLUDED
#define STEPPER_MONITOR_H_INCLUDED

#include <Arduino.h>

///@class StepperMonitor
///@brief Abstract class for drawing StepperMonitors using buttons, labels, etc...
class StepperMonitor
{
    public:
        /**
        @brief Called to let the StepperMonitor know when the stepper moved. Must be implemented by sub-classes
        @param currentPosition The current position of the stepper so that the callie can update labels or anything else it may want to check
        */
        virtual void stepperDidMove(int currentPosition) = 0;
};

#endif
