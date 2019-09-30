#include <Arduino.h>
#include <stdint.h>
#include <SPI.h>
#include <Wire.h>
#include <avr/dtostrf.h>

#include <RA8875.h>
#include <fonts/akashi_36.c>

#include "HeightScreen.h"
#include "../hardware/stepper.h"
#include "../graphics/GSL1680.h"
#include "../graphics/Buttons.h"
#include "../graphics/TouchScreenStrings.h"
#include "../utils/utils.h"

HeightScreen::HeightScreen(RA8875* tft, GSL1680* ts, Screen* previousScreen): Screen(tft, ts, "Height", true, previousScreen)
{
	stepper = Stepper::getInstance();

	currentIncrement = 0;

	Button* button = new Button(tft, HS_UP_BUTTON_ID, "Up", 55, 80, 240, 90, true, RA8875_WHITE, RA8875_BLUE, RA8875_WHITE);
	GraphicPressActions* pressActions = new GraphicPressActions();
	pressActions->handlerObject = this;
	button->setPressAction(pressActions);
	addButton(button);
	button = new Button(tft, HS_DOWN_BUTTON_ID, "Down", 55, 80 + 110, 240, 90, true, RA8875_WHITE, RA8875_BLUE, RA8875_WHITE);
	pressActions = new GraphicPressActions();
	pressActions->handlerObject = this;
	button->setPressAction(pressActions);
	addButton(button);
	button = new Button(tft, HS_HOME_BUTTON_ID, "Home", 450, tft->height() - 60, 130, 50, true, RA8875_WHITE, RA8875_BLUE, RA8875_WHITE);
	pressActions = new GraphicPressActions();
	pressActions->handlerObject = this;
	button->setPressAction(pressActions);
	addButton(button);
	button = new Button(tft, HS_BITCHANGE_BUTTON_ID, "Bit Change", 590, tft->height() - 60, 200, 50, true, RA8875_WHITE, RA8875_BLUE, RA8875_WHITE);
	pressActions = new GraphicPressActions();
	pressActions->handlerObject = this;
	button->setPressAction(pressActions);
	addButton(button);
	button = new Button(tft, HS_STORE_BUTTON_ID, "Store", 450, tft->height() - 130, 130, 50, true, RA8875_WHITE, RA8875_BLUE, RA8875_WHITE);
	pressActions = new GraphicPressActions();
	pressActions->handlerObject = this;
	button->setPressAction(pressActions);
	addButton(button);
	button = new Button(tft, HS_RECALL_BUTTON_ID, "Recall", 590, tft->height() - 130, 160, 50, true, RA8875_WHITE, RA8875_BLUE, RA8875_WHITE);
	pressActions = new GraphicPressActions();
	pressActions->handlerObject = this;
	button->setPressAction(pressActions);
	addButton(button);
	incrementsButton = new Button(tft, HS_INCREMENT_BUTTON_ID, stepper->getUnits() == INCHES ? inchIncrementsText[currentIncrement] : mmIncrementsText[currentIncrement], 20, tft->height() - 180, 150, 50, true, RA8875_WHITE, RA8875_BLUE, RA8875_WHITE);
	pressActions = new GraphicPressActions();
	pressActions->handlerObject = this;
	incrementsButton->setPressAction(pressActions);
	addButton(incrementsButton);
	stepper->setSpeed(FAST);
	speedButton = new Button(tft, HS_SPEED_BUTTON_ID, "Fast", 20, tft->height() - 120, 150, 50, true, RA8875_WHITE, RA8875_BLUE, RA8875_WHITE);
	pressActions = new GraphicPressActions();
	pressActions->handlerObject = this;
	speedButton->setPressAction(pressActions);
	addButton(speedButton);
	unitsButton = new Button(tft, HS_UNIT_BUTTON_ID, stepper->getUnits() == MM ? "MM" : "Inch", 20, tft->height() - 60, 150, 50, true, RA8875_WHITE, RA8875_BLUE, RA8875_WHITE);
	pressActions = new GraphicPressActions();
	pressActions->handlerObject = this;
	unitsButton->setPressAction(pressActions);
	addButton(unitsButton);

	buildStringsFromStepperPosition(false);

	TouchScreenString* label = new TouchScreenString(tft, UNNEEDED_ID, "Current Height", ((tft->width()/4)*3), 90, true, 1, RA8875_CYAN);
	addGraphic(label);
	// inchesLabel = new TouchScreenString(tft, HS_INCH_HEIGHT_TEXT_ID, inchesText, 360, 130, stepper->getUnits() == INCHES ? 1 : 0, RA8875_CYAN, &akashi_36);
	inchesLabel = new TouchScreenString(tft, HS_INCH_HEIGHT_TEXT_ID, inchesText, ((tft->width()/4)*3), 160, true, stepper->getUnits() == INCHES ? 3 : 1, RA8875_CYAN);
	addGraphic(inchesLabel);
	// mmLabel = new TouchScreenString(tft, HS_MM_HEIGHT_TEXT_ID, mmText, 335, 210, stepper->getUnits() == MM ? 1 : 0, RA8875_CYAN, &akashi_36);
	mmLabel = new TouchScreenString(tft, HS_MM_HEIGHT_TEXT_ID, mmText, ((tft->width()/4)*3), 220, true, stepper->getUnits() == MM ? 3 : 1, RA8875_CYAN);
	addGraphic(mmLabel);
	label = new TouchScreenString(tft, UNNEEDED_ID, "Position", 585, tft->height() - 170, true, 1, RA8875_CYAN);
	addGraphic(label);

	changedButtons = 0;
}

boolean HeightScreen::handleButtonPressAction(int buttonID)
{
	switch(buttonID)
	{
		case HS_UP_BUTTON_ID:
			stepper->moveUp(calculateCountsToMove(), this);
			break;
		
		case HS_DOWN_BUTTON_ID:
			stepper->moveDown(calculateCountsToMove(), this);
			break;
		
		case HS_HOME_BUTTON_ID:
			stepper->moveToHomePosition(this);
			break;

		case HS_BITCHANGE_BUTTON_ID:
			stepper->moveToBitChangePosition(this);
			break;
		
		case HS_STORE_BUTTON_ID:
			break;
		
		case HS_RECALL_BUTTON_ID:
			break;

		case HS_INCREMENT_BUTTON_ID:
			moveIncrement();
			changedButtons |= HS_INCREMENT_BUTTON_ID;
			break;

		case HS_SPEED_BUTTON_ID:
			changeSpeed();
			changedButtons |= HS_SPEED_BUTTON_ID;
			break;

		case HS_UNIT_BUTTON_ID:
			changeUnits();
			changedButtons |= HS_UNIT_BUTTON_ID;
			changedButtons |= HS_INCREMENT_BUTTON_ID;
			break;
	}
	return false;
}

void HeightScreen::doProcessing()
{
	redrawChangedButtons();

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
}

void HeightScreen::redrawChangedButtons()
{
	if (changedButtons && HS_INCREMENT_BUTTON_ID)
	{
		incrementsButton->draw();
	}
	if (changedButtons && HS_SPEED_BUTTON_ID)
	{
		speedButton->draw();
	}
	if (changedButtons && HS_UNIT_BUTTON_ID)
	{
		unitsButton->draw();
	}
	changedButtons = 0;
}

boolean HeightScreen::buildStringsFromStepperPosition(boolean doErase)
{
	boolean retVal = false;
	char inchesPositionText[10];
	char tempInchesText[15];
	char mmPositionText[10];
	char tempMMText[15];
	char* trimmedInchesText;
	char* trimmedMMText;

	currentPositionInInches = stepper->getCurrentRelativePosition(INCHES);
	dtostrf(currentPositionInInches, 7, 3, inchesPositionText);
	sprintf(tempInchesText, "%sin", inchesPositionText);
	trimmedInchesText = trimString(tempInchesText);

	currentPositionInMM = stepper->getCurrentRelativePosition(MM);
	dtostrf(currentPositionInMM, 8, 2, mmPositionText);
	sprintf(tempMMText, "%smm", mmPositionText);
	trimmedMMText = trimString(tempMMText);

	if (doErase)
	{
		inchesLabel->erase();
		mmLabel->erase();
	}
	strcpy(inchesText, trimmedInchesText);
	strcpy(mmText, trimmedMMText);
	retVal = true;

	return retVal;
}

void HeightScreen::stepperDidMove(int currentPosition)
{
	if (buildStringsFromStepperPosition(true))
	{
		inchesLabel->setText(inchesText);
		mmLabel->setText(mmText);
		inchesLabel->draw();
		mmLabel->draw();
	}
}

int HeightScreen::calculateCountsToMove()
{
	Units units = stepper->getUnits();

	if (units == INCHES)
	{
		return inchIncrementCounts[currentIncrement];
	}
	else
	{
		return mmIncrementCounts[currentIncrement];
	}
}

void HeightScreen::moveIncrement()
{
	Units units = stepper->getUnits();

	currentIncrement++;

	if (currentIncrement >= HS_NUM_INCREMENTS)
	{
		currentIncrement = 0;
	}

	incrementsButton->setText(units == INCHES ? inchIncrementsText[currentIncrement] : mmIncrementsText[currentIncrement]);
}

void HeightScreen::changeUnits()
{
	Units units = stepper->getUnits();

	if (units == MM)
	{
		units = INCHES;
	}
	else
	{
		units = MM;
	}
	unitsButton->setText(units == MM ? "MM" : "Inch");
	stepper->setUnits(units);
	incrementsButton->setText(units == INCHES ? inchIncrementsText[currentIncrement] : mmIncrementsText[currentIncrement]);
	doProcessing();
}

void HeightScreen::changeSpeed()
{
	switch(stepper->getSpeed())
	{
		case FAST:
			stepper->setSpeed(MEDIUM);
			speedButton->setText("Med");
			break;

		case MEDIUM:
			stepper->setSpeed(SLOW);
			speedButton->setText("Slow");
			break;

		case SLOW:
			stepper->setSpeed(FAST);
			speedButton->setText("Fast");
			break;
	}
}