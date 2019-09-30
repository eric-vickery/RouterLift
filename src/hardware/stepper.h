#ifndef STEPPER_H_INCLUDED
#define STEPPER_H_INCLUDED

#include <stdint.h>
#include <SPI.h>
#include <Wire.h>

#include "StepperMonitor.h"

// set control constants and assign I/O lines
#define DIRECTION_PIN	53
#define STEP_PIN		51

#define STEPS_PER_INCH	4064
#define STEPS_PER_MM	160
#define FAST_DELAY		500
#define MEDIUM_DELAY	0
#define SLOW_DELAY		6000
// #define MEDIUM_DELAY	2000
// #define SLOW_DELAY		8000

#define MONITOR_NOTIFY_MAX_COUNT_FAST_SPEED		32
#define MONITOR_NOTIFY_MAX_COUNT_MEDIUM_SPEED	0
#define MONITOR_NOTIFY_MAX_COUNT_SLOW_SPEED		0

#define NUM_LOCATIONS_POSITIONS	5

typedef enum Units
{
	MM = 1,
	INCHES
} Units;

typedef enum Speed
{
	FAST = 0,
	MEDIUM,
	SLOW
} Speed;

class Stepper
{
public:
	static Stepper* getInstance();

	void moveUp(int steps, StepperMonitor* monitor = NULL);
	void moveDown(int steps, StepperMonitor* monitor = NULL);

	void moveToStoredPosition(int position, StepperMonitor* monitor = NULL);
	void moveToHomePosition(StepperMonitor* monitor = NULL);
	void moveToBitChangePosition(StepperMonitor* monitor = NULL);
	void performProbe(boolean setZero, StepperMonitor* monitor = NULL);

	void setSpeed(Speed newSpeed);
	Speed getSpeed();

	void setUnits(Units newUnits);
	Units getUnits();

	int convertDistanceToCount(Units units, float distance);
	float getCurrentAbsolutePosition(Units units);
	float getCurrentRelativePosition(Units units);

private:
	Stepper();

	float convertCountToDistance(Units units, int count);
	void move(int steps, boolean up, StepperMonitor* monitor = NULL);
	void moveToAbsolutePosition(int position, StepperMonitor* monitor = NULL);


private:
	static Stepper* instance;

	int absolutePositionCount;
	int relativePositionCount;
	int storedLocations[NUM_LOCATIONS_POSITIONS];
	Speed speed = FAST;
	unsigned long pulseTime = FAST_DELAY;
	int monitorMaxCount = MONITOR_NOTIFY_MAX_COUNT_FAST_SPEED;
	Units units;
};

// void initStepper();

// void move(int counts);
// void moveUp(int counts);
// void moveDown(int counts);

#endif