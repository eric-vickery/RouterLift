#include <Arduino.h>
#include <stdint.h>
#include <SPI.h>
#include <Wire.h>
#include <avr/dtostrf.h>

#include <RA8875.h>
#include <fonts/akashi_36.c>

#include "SpindleScreen.h"
#include "../graphics/GSL1680.h"
#include "../graphics/Buttons.h"
#include "../graphics/TouchScreenStrings.h"
#include "../graphics/RPMGauge.h"

SpindleScreen::SpindleScreen(RA8875* tft, GSL1680* ts, Screen* previousScreen): Screen(tft, ts, "Speed Control", true, previousScreen)
{
	Button* button = new Button(tft, SS_START_BUTTON_ID, "Start", 20, 90, 230, 160, true, RA8875_WHITE, RA8875_GREEN, RA8875_BLACK);
	GraphicPressActions* pressActions = new GraphicPressActions();
	pressActions->handlerObject = this;
	button->setPressAction(pressActions);
	addButton(button);
	button = new Button(tft, SS_STOP_BUTTON_ID, "Stop", 20, 270, 230, 160, true, RA8875_WHITE, RA8875_RED, RA8875_BLACK);
	pressActions = new GraphicPressActions();
	pressActions->handlerObject = this;
	button->setPressAction(pressActions);
	addButton(button);
	button = new Button(tft, SS_INCREASE_BUTTON_ID, "Faster", 550, 90, 230, 160, true, RA8875_WHITE, RA8875_BLUE, RA8875_WHITE);
	pressActions = new GraphicPressActions();
	pressActions->handlerObject = this;
	button->setPressAction(pressActions);
	addButton(button);
	button = new Button(tft, SS_DECREASE_BUTTON_ID, "Slower", 550, 270, 230, 160, true, RA8875_WHITE, RA8875_BLUE, RA8875_WHITE);
	pressActions = new GraphicPressActions();
	pressActions->handlerObject = this;
	button->setPressAction(pressActions);
	addButton(button);

	rpmGauge = new RPMGauge(tft, SS_RPM_GAUGE_ID, (tft->width()/2), 240, 100, RA8875_WHITE, RA8875_BLACK, RA8875_CYAN);
	rpmGauge->setRPM(12065);
	addGraphic(rpmGauge);

	// TEST CODE REMOVE BEFORE PRODUCTION
	randomSeed(analogRead(0));
}

boolean SpindleScreen::handleButtonPressAction(int buttonID)
{
	// Nothing to do at the moment
	return false;
}

void SpindleScreen::doProcessing()
{
	rpmGauge->erase();
	// TEST CODE REMOVE BEFORE PRODUCTION
	rpmGauge->setRPM(random(24000));
	rpmGauge->draw();
}
