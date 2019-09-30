#ifndef MAIN_SCREEN_H_INCLUDED
#define MAIN_SCREEN_H_INCLUDED

#include <Arduino.h>
#include <RA8875.h>

#include "Screen.h"
#include "ShouldHomeScreen.h"
#include "../graphics/GSL1680.h"
#include "../graphics/RPMGauge.h"
#include "../hardware/stepper.h"

#define MS_HEIGHT_BUTTON_ID		1
#define MS_SPINDLE_BUTTON_ID	2

#define MS_INCH_HEIGHT_TEXT_ID	10
#define MS_MM_HEIGHT_TEXT_ID	11
#define MS_RPM_GAUGE_ID			12

///@class MainScreen
///@brief Class for the main screen of the app
class MainScreen: public Screen
{
public:
	/**
	@brief Default constructor for the Screen class
	@param tft A pointer to the RA8875 object for the screen.
	@param ts A pointer to the GSL1680 object for the touch screen.
	*/
	MainScreen(RA8875* tft, GSL1680* ts);

	void doProcessing();
	boolean handleButtonPressAction(int buttonID);

private:
	boolean buildStringsFromStepperPosition(boolean doErase);

private:
	Stepper* stepper;
	float currentPositionInInches;
	char inchesText[15];
	float currentPositionInMM;
	char mmText[15];
	TouchScreenString* inchesLabel;
	TouchScreenString* mmLabel;
	RPMGauge* rpmGauge;
	ShouldHomeScreen* shouldHomeScreen;
};

#endif
