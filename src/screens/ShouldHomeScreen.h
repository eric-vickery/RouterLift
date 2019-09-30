#ifndef SHOULD_HOME_SCREEN_H_INCLUDED
#define SHOULD_HOME_SCREEN_H_INCLUDED

#include <Arduino.h>
#include <RA8875.h>

#include "Screen.h"
#include "../graphics/GSL1680.h"
#include "../hardware/stepper.h"

#define HS_YES_BUTTON_ID	0x0001
#define HS_NO_BUTTON_ID		0x0002

///@class ShouldHomeScreen
///@brief Class for the height adjustment screen
class ShouldHomeScreen: public Screen
{
public:
	/**
	@brief Default constructor for the ShouldHomeScreen class
	@param tft A pointer to the RA8875 object for the screen.
	@param ts A pointer to the GSL1680 object for the touch screen.
	@param previousScreen This is the screen that should be shown when the back button is pressed. Default is null.
	*/
	ShouldHomeScreen(RA8875* tft, GSL1680* ts, Screen* previousScreen = NULL);
	
	void doProcessing();
	boolean handleButtonPressAction(int buttonID);

private:
	void doHome();

private:
	Stepper* stepper;
	Button* yesButton;
	Button* noButton;
};

#endif
