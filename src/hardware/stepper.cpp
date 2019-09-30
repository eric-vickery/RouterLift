#include <stdint.h>
#include <SPI.h>
#include <Wire.h>

#include "stepper.h"

Stepper* Stepper::instance = NULL;

Stepper* Stepper::getInstance()
{
	if (instance == NULL)
	{
		instance = new Stepper();
	}
	return instance;
}

Stepper::Stepper()
{
	pinMode(DIRECTION_PIN, OUTPUT);
	pinMode(STEP_PIN, OUTPUT);
	// Put code here to initialize the limit switches and probe
	absolutePositionCount = -1;
	relativePositionCount = -1;
	units = INCHES;

	for (int i = 0; i < NUM_LOCATIONS_POSITIONS; i++)
	{
		storedLocations[i] = -1;
	}
}

void Stepper::moveUp(int steps, StepperMonitor* monitor)
{
	digitalWrite(DIRECTION_PIN, HIGH);
	move(steps, true, monitor);
}

void Stepper::moveDown(int steps, StepperMonitor* monitor)
{
	digitalWrite(DIRECTION_PIN, LOW);
	move(steps, false, monitor);
}

void Stepper::moveToStoredPosition(int position, StepperMonitor* monitor)
{
	if (position >= 0 && position < NUM_LOCATIONS_POSITIONS)
	{
		int storedCount = storedLocations[position];
		if (storedCount != -1)
		{
			moveToAbsolutePosition(storedCount, monitor);
		}
	}
}

void Stepper::moveToHomePosition(StepperMonitor* monitor)
{
	// TODO Implement this
	absolutePositionCount = 0;
	relativePositionCount = 0;
}

void Stepper::moveToBitChangePosition(StepperMonitor* monitor)
{
	// TODO Implement this
	moveToAbsolutePosition(25000, monitor);
}

void Stepper::performProbe(boolean setZero, StepperMonitor* monitor)
{
	// TODO Implement this
	moveToAbsolutePosition(10000, monitor);
	if (setZero)
	{
		relativePositionCount = 0;
	}
}

void Stepper::setSpeed(Speed newSpeed)
{
	this->speed = newSpeed;

	switch (speed)
	{
		case FAST:
			pulseTime = FAST_DELAY;
			monitorMaxCount = MONITOR_NOTIFY_MAX_COUNT_FAST_SPEED;
			break;
	
		case MEDIUM:
			pulseTime = MEDIUM_DELAY;
			monitorMaxCount = MONITOR_NOTIFY_MAX_COUNT_MEDIUM_SPEED;
			break;
	
		case SLOW:
			pulseTime = SLOW_DELAY;
			monitorMaxCount = MONITOR_NOTIFY_MAX_COUNT_SLOW_SPEED;
			break;	
	}
}

Speed Stepper::getSpeed()
{
	return speed;
}

void Stepper::setUnits(Units newUnits)
{
	this->units = newUnits;
}

Units Stepper::getUnits()
{
	return units;
}

int Stepper::convertDistanceToCount(Units units, float distance)
{
	int count = 0;

	if (units == MM)
	{
		count = distance * STEPS_PER_MM;
	}
	else
	{
		count = distance * STEPS_PER_INCH;
	}

	return count;
}

float Stepper::getCurrentAbsolutePosition(Units units)
{
	return convertCountToDistance(units, absolutePositionCount);
}

float Stepper::getCurrentRelativePosition(Units units)
{
	return convertCountToDistance(units, relativePositionCount);
}

float Stepper::convertCountToDistance(Units units, int count)
{
	float position = -1;

	if (count != -1)
	{
		if (units == MM)
		{
			position = float(count) / STEPS_PER_MM;
		}
		else
		{
			position = float(count) / STEPS_PER_INCH;
		}
	}

	return position;
}

void Stepper::move(int steps, boolean up, StepperMonitor* monitor)
{
	int increment = up ? +1 : -1;
	int monitorNotifyCount = 0;

	while (steps)
	{
		digitalWrite(STEP_PIN, HIGH);
		delayMicroseconds(200);
		digitalWrite(STEP_PIN, LOW);
		steps--;
		absolutePositionCount += increment;
		relativePositionCount += increment;
		monitorNotifyCount++;
		if (monitor && monitorNotifyCount > monitorMaxCount)
		{
			monitor->stepperDidMove(relativePositionCount);
			monitorNotifyCount = 0;
		}
		else
		{
			if (monitorMaxCount == MONITOR_NOTIFY_MAX_COUNT_FAST_SPEED)
			{
				delayMicroseconds(pulseTime);
			}
		}
		
		if (monitorMaxCount != MONITOR_NOTIFY_MAX_COUNT_FAST_SPEED)
		{
			delayMicroseconds(pulseTime);
		}
	}
}

void Stepper::moveToAbsolutePosition(int position, StepperMonitor* monitor)
{
	int steps;

	if (position > absolutePositionCount)
	{
		steps = position - absolutePositionCount;
		moveUp(steps, monitor);
	}
	else
	{
		steps = absolutePositionCount -  position;
		moveDown(steps, monitor);
	}
	
}