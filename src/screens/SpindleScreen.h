#ifndef SPINDLE_SCREEN_H_INCLUDED
#define SPINDLE_SCREEN_H_INCLUDED

#include <Arduino.h>
#include <RA8875.h>

#include "Screen.h"
#include "../graphics/GSL1680.h"
#include "../graphics/RPMGauge.h"
#include "../hardware/stepper.h"

#define SS_START_BUTTON_ID		1
#define SS_STOP_BUTTON_ID		2
#define SS_INCREASE_BUTTON_ID	3
#define SS_DECREASE_BUTTON_ID	4

#define SS_RPM_GAUGE_ID			10

///@class SpindleScreen
///@brief Class for the spindle screen of the app
class SpindleScreen: public Screen
{
public:
	/**
	@brief Default constructor for the Screen class
	@param tft A pointer to the RA8875 object for the screen.
	@param ts A pointer to the GSL1680 object for the touch screen.
	*/
	SpindleScreen(RA8875* tft, GSL1680* ts, Screen* previousScreen = NULL);

	void doProcessing();
	boolean handleButtonPressAction(int buttonID);

private:

private:
	RPMGauge* rpmGauge;
};

#endif
