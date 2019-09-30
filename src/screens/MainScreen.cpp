#include <Arduino.h>
#include <stdint.h>
#include <SPI.h>
#include <Wire.h>
#include <avr/dtostrf.h>

#include <RA8875.h>
#include <fonts/akashi_36.c>

#include "MainScreen.h"
#include "HeightScreen.h"
#include "SpindleScreen.h"
#include "ShouldHomeScreen.h"
#include "../graphics/GSL1680.h"
#include "../graphics/Buttons.h"
#include "../graphics/TouchScreenStrings.h"
#include "../graphics/RPMGauge.h"
#include "../utils/utils.h"

MainScreen::MainScreen(RA8875* tft, GSL1680* ts): Screen(tft, ts, "Main", false, NULL)
{
	shouldHomeScreen = NULL;
	stepper = Stepper::getInstance();

	Button* button = new Button(tft, MS_HEIGHT_BUTTON_ID, "Height Adjust", (tft->width()/4)-110, tft->height()-100, 220, 80, true, RA8875_WHITE, RA8875_BLUE, RA8875_WHITE);
	Screen* heightScreen = new HeightScreen(tft, ts, this);
	GraphicPressActions* pressActions = new GraphicPressActions();
	pressActions->screen = heightScreen;
	button->setPressAction(pressActions);
	addButton(button);
	button = new Button(tft, MS_SPINDLE_BUTTON_ID, "Speed Control", ((tft->width()/4)*3)-110, tft->height()-100, 220, 80, true, RA8875_WHITE, RA8875_BLUE, RA8875_WHITE);
	Screen* spindleScreen = new SpindleScreen(tft, ts, this);
	pressActions = new GraphicPressActions();
	pressActions->screen = spindleScreen;
	button->setPressAction(pressActions);
	addButton(button);

	buildStringsFromStepperPosition(false);

	TouchScreenString* label = new TouchScreenString(tft, UNNEEDED_ID, "Current Height", tft->width()/4, 108, true, 1, RA8875_CYAN);
	addGraphic(label);
	inchesLabel = new TouchScreenString(tft, MS_INCH_HEIGHT_TEXT_ID, inchesText, tft->width()/4, 180, true, stepper->getUnits() == INCHES ? 3 : 1, RA8875_CYAN);
	addGraphic(inchesLabel);
	mmLabel = new TouchScreenString(tft, MS_MM_HEIGHT_TEXT_ID, mmText, tft->width()/4, 260, true, stepper->getUnits() == MM ? 3 : 1, RA8875_CYAN);
	addGraphic(mmLabel);
	rpmGauge = new RPMGauge(tft, MS_RPM_GAUGE_ID, ((tft->width()/4)*3), 240, 100, RA8875_WHITE, RA8875_BLACK, RA8875_CYAN);
	rpmGauge->setRPM(12065);
	addGraphic(rpmGauge);

	// TEST CODE REMOVE BEFORE PRODUCTION
	randomSeed(analogRead(0));
}

boolean MainScreen::handleButtonPressAction(int buttonID)
{
	// Nothing to do at the moment
	return false;
}

void MainScreen::doProcessing()
{
	if (buildStringsFromStepperPosition(true))
	{
		Units units = stepper->getUnits();

		inchesLabel->setFontSize(units == INCHES ? 3 : 1);
		mmLabel->setFontSize(units == MM ? 3 : 1);
		inchesLabel->setText(inchesText);
		mmLabel->setText(mmText);
		inchesLabel->draw();
		mmLabel->draw();
	}

	// rpmGauge->erase();
	// TEST CODE REMOVE BEFORE PRODUCTION
	// rpmGauge->setRPM(random(24000));
	// rpmGauge->draw();
	if (stepper->getCurrentAbsolutePosition(INCHES) < 0.0)
	{
		Serial.println("We should home");
		if (shouldHomeScreen == NULL)
		{
			shouldHomeScreen = new ShouldHomeScreen(tft, ts, this);
		}
		setNewScreen(shouldHomeScreen);
	}
}

boolean MainScreen::buildStringsFromStepperPosition(boolean doErase)
{
	boolean retVal = false;
	char inchesPositionText[10];
	char mmPositionText[10];
	char tempInchesText[15];
	char tempMMText[15];
	currentPositionInInches = stepper->getCurrentRelativePosition(INCHES);
	currentPositionInMM = stepper->getCurrentRelativePosition(MM);
	
	dtostrf(currentPositionInInches, 7, 3, inchesPositionText);
	dtostrf(currentPositionInMM, 8, 2, mmPositionText);
	sprintf(tempInchesText, "%sin", inchesPositionText);
	sprintf(tempMMText, "%smm", mmPositionText);

	char* trimmedInchesText = trimString(tempInchesText);
	char* trimmedMMText = trimString(tempMMText);

	// if (strcmp(trimmedInchesText, inchesText) != 0)
	// {
	if (doErase)
	{
		inchesLabel->erase();
		mmLabel->erase();
	}
	strcpy(inchesText, trimmedInchesText);
	strcpy(mmText, trimmedMMText);
	retVal = true;
	// }
	return retVal;
}
