#ifndef HEIGHT_SCREEN_H_INCLUDED
#define HEIGHT_SCREEN_H_INCLUDED

#include <Arduino.h>
#include <RA8875.h>

#include "Screen.h"
#include "../graphics/GSL1680.h"
#include "../hardware/stepper.h"
#include "../hardware/StepperMonitor.h"


#define HS_UP_BUTTON_ID			0x0001
#define HS_DOWN_BUTTON_ID		0x0002
#define HS_HOME_BUTTON_ID		0x0004
#define HS_BITCHANGE_BUTTON_ID	0x0008
#define HS_INCREMENT_BUTTON_ID	0x0010
#define HS_SPEED_BUTTON_ID		0x0020
#define HS_UNIT_BUTTON_ID		0x0040
#define HS_STORE_BUTTON_ID		0x0080
#define HS_RECALL_BUTTON_ID		0x0100

#define HS_INCH_HEIGHT_TEXT_ID	11
#define HS_MM_HEIGHT_TEXT_ID	12

#define HS_NUM_INCREMENTS		5

///@class HeightScreen
///@brief Class for the height adjustment screen
class HeightScreen: public Screen, StepperMonitor
{
public:
	/**
	@brief Default constructor for the HeightScreen class
	@param tft A pointer to the RA8875 object for the screen.
	@param ts A pointer to the GSL1680 object for the touch screen.
	@param previousScreen This is the screen that should be shown when the back button is pressed. Default is null.
	*/
	HeightScreen(RA8875* tft, GSL1680* ts, Screen* previousScreen = NULL);

	void doProcessing();
	boolean handleButtonPressAction(int buttonID);
	void stepperDidMove(int currentPosition);

private:
	boolean buildStringsFromStepperPosition(boolean doErase);
	int calculateCountsToMove();
	void moveIncrement();
	void changeUnits();
	void changeSpeed();
	void redrawChangedButtons();

private:
	Stepper* stepper;
	float currentPositionInInches;
	char inchesText[15];
	float currentPositionInMM;
	char mmText[15];
	TouchScreenString* inchesLabel;
	TouchScreenString* mmLabel;
	Button* incrementsButton;
	Button* speedButton;
	Button* unitsButton;
	unsigned int changedButtons;
	int currentIncrement;
	const int inchIncrementCounts[HS_NUM_INCREMENTS] = {127, 254, 508, 1016, 2032};
	const char* inchIncrementsText[HS_NUM_INCREMENTS] = {"1/32", "1/16", "1/8", "1/4", "1/2"};
	const int mmIncrementCounts[HS_NUM_INCREMENTS] = {80, 160, 480, 800, 1600};
	const char* mmIncrementsText[HS_NUM_INCREMENTS] = {"0.5", "1", "3", "5", "10"};
};

#endif
