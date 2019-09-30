#include <Arduino.h>
#include <stdint.h>
#include <SPI.h>
#include <Wire.h>
#include <avr/dtostrf.h>

#include <RA8875.h>
#include <fonts/akashi_36.c>

#include "ShouldHomeScreen.h"
#include "../hardware/stepper.h"
#include "../graphics/GSL1680.h"
#include "../graphics/Buttons.h"
#include "../graphics/TouchScreenStrings.h"

ShouldHomeScreen::ShouldHomeScreen(RA8875* tft, GSL1680* ts, Screen* previousScreen): Screen(tft, ts, "Home Needed", false, previousScreen)
{
	stepper = Stepper::getInstance();

	Button* button = new Button(tft, HS_YES_BUTTON_ID, "Yes", 250, 200, 100, 60, true, RA8875_WHITE, RA8875_BLUE, RA8875_WHITE);
	GraphicPressActions* pressActions = new GraphicPressActions();
	pressActions->handlerObject = this;
	button->setPressAction(pressActions);
	addButton(button);
	button = new Button(tft, HS_NO_BUTTON_ID, "No", 450, 200, 100, 60, true, RA8875_WHITE, RA8875_BLUE, RA8875_WHITE);
	pressActions = new GraphicPressActions();
	pressActions->handlerObject = this;
	button->setPressAction(pressActions);
	addButton(button);

	TouchScreenString* label = new TouchScreenString(tft, UNNEEDED_ID, "Spindle not homed.", tft->width()/2, 90, true, 1, RA8875_WHITE);
	addGraphic(label);
	label = new TouchScreenString(tft, UNNEEDED_ID, "Should a home be performed now?", tft->width()/2, 120, true, 1, RA8875_WHITE);
	addGraphic(label);
}

boolean ShouldHomeScreen::handleButtonPressAction(int buttonID)
{
	switch(buttonID)
	{
		case HS_YES_BUTTON_ID:
			doHome();
			break;
		
		case HS_NO_BUTTON_ID:
			// Do nothing
			break;
	}
	return true;
}

void ShouldHomeScreen::doProcessing()
{
}

void ShouldHomeScreen::doHome()
{
	stepper->moveToHomePosition();
}
