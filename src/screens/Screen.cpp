#include <Arduino.h>
#include <stdint.h>
#include <SPI.h>
#include <Wire.h>

#include <RA8875.h>

#include "Screen.h"
#include "../graphics/GSL1680.h"
#include "../graphics/Buttons.h"
#include "../graphics/GraphicsBase.h"

#define TITLE_AREA_HEIGHT   50
#define BACK_BUTTON_WIDTH   80

Screen::Screen(RA8875* tft, GSL1680* ts)
{
	this->tft = tft;
	this->ts = ts;
	initializeButtonsArray();
	this->backButton = NULL;
	this->title = NULL;
	this->previousScreen = NULL;
	this->newScreen = NULL;
	initializeGraphicsArray();
}

Screen::Screen(RA8875* tft, GSL1680* ts, const char* title, boolean hasBackButton, Screen* previousScreen)
{
	this->tft = tft;
	this->ts = ts;

	initializeButtonsArray();
	initializeGraphicsArray();

	this->previousScreen = previousScreen;
	this->newScreen = NULL;

	if (hasBackButton)
	{
		createBackButton(previousScreen);
	}
	else
	{
		backButton = NULL;
	}
	
	if (strlen(title) > 0)
	{
		createTitle(title);
	}
	else
	{
		this->title = NULL;
	}
}

void Screen::initializeButtonsArray()
{
	for (int i = 0; i < NUM_BUTTONS; i++)
	{
		buttons[i] = NULL;
	}
}

void Screen::initializeGraphicsArray()
{
	for (int i = 0; i < NUM_GRAPHICS; i++)
	{
		graphics[i] = NULL;
	}
}

void Screen::createBackButton(Screen* previousScreen)
{
	backButton = new Button(tft, BACK_BUTTON_ID, "Back", 0, 0, BACK_BUTTON_WIDTH, TITLE_AREA_HEIGHT, true, RA8875_WHITE, RA8875_BLUE, RA8875_WHITE);
	GraphicPressActions* pressActions = new GraphicPressActions();
	pressActions->screen = previousScreen;
	backButton->setPressAction(pressActions);
}

void Screen::createTitle(const char* titleText)
{
	int stringLength = strlen(titleText);
	int fontScale = 1;
	tft->setFontScale(fontScale);
	uint8_t fontWidth = tft->getFontWidth();
	uint8_t fontHeight = tft->getFontHeight();
	int heightGap = (TITLE_AREA_HEIGHT - fontHeight)/2;
	int stringWidth = stringLength * fontWidth;
	int widthGap = ((tft->width() - stringWidth) / 2);

	title = new TouchScreenString(tft, TITLE_TEXT_ID, titleText, widthGap, heightGap, fontScale, RA8875_WHITE);
}

Screen* Screen::display(boolean clearScreen)
{
	newScreen = NULL;
	
	if (clearScreen)
	{
		tft->clearScreen(RA8875_BLACK);
	}
	drawButtons();
	drawGraphics();
	drawTitle();
	drawBackButton();

	unsigned long startTime = millis();

	while(1)
	{
		if (ts->hasData())
		{
			int numFingers = ts->dataread();
			delay(50);

			if(numFingers == 1 && !buttonDown)
			{
				if (backButton != 0 && backButton->isPressed(ts->readFingerX(0), ts->readFingerY(0)))
				{
					backButton->buttonDisplay();
					GraphicPressActions* pressActions = backButton->getPressAction();
					if (pressActions->screen != NULL)
					{
						return (Screen*)pressActions->screen;
					}
				}

				for (int i = 0; i < NUM_BUTTONS; i++)
				{
					if (buttons[i] != 0 && buttons[i]->isPressed(ts->readFingerX(0), ts->readFingerY(0)))
					{
						// Serial.print("Button was pressed with label "); Serial.println(buttons[i]->getText());
						GraphicPressActions* pressActions = buttons[i]->getPressAction();
						if (pressActions != NULL)
						{
							buttons[i]->buttonDisplay();
							if (pressActions->handlerObject != NULL)
							{
								if (((Screen*)pressActions->handlerObject)->handleButtonPressAction(buttons[i]->getID()))
								{
									if (previousScreen != NULL)
									{
										return previousScreen;
									}
								}
							}

							if (pressActions->screen != NULL)
							{
								return (Screen*)pressActions->screen;
							}
						}
					}
				}
			}
		}

		if (millis() - startTime > PROCESSING_DELAY_MILLIS)
		{
			doProcessing();

			// Reset the timer
			startTime = millis();
		}

		// See if we need to change to a new screen
		if (newScreen != NULL)
		{
			return newScreen;
		}
	}
	return NULL;
}

void Screen::drawButtons()
{
	for (int i = 0; i < NUM_BUTTONS; i++)
	{
		if (buttons[i] != NULL)
		{
			buttons[i]->draw();
		}
	}
}

void Screen::eraseButtons()
{
	for (int i = 0; i < NUM_BUTTONS; i++)
	{
		if (buttons[i] != NULL)
		{
			buttons[i]->erase();
		}
	}
}

void Screen::drawGraphics()
{
	for (int i = 0; i < NUM_GRAPHICS; i++)
	{
		if (graphics[i] != NULL)
		{
			graphics[i]->draw();
		}
	}
}

void Screen::eraseGraphics()
{
	for (int i = 0; i < NUM_GRAPHICS; i++)
	{
		if (graphics[i] != NULL)
		{
			graphics[i]->erase();
		}
	}
}

void Screen::drawTitle()
{
	if (title != NULL)
	{
		title->draw();
		tft->drawFastHLine(0, TITLE_AREA_HEIGHT + 10, tft->width(), RA8875_WHITE);
	}
}

void Screen::drawBackButton()
{
	if (backButton != NULL)
	{
		backButton->draw();
	}
}

void Screen::setTitle(const char* newTitle)
{
	title->setText(newTitle);
}

void Screen::addButton(Button* newButton)
{
	// Find the first available slot and set it to the newButton
	for (int i = 0; i < NUM_BUTTONS; i++)
	{
		if (buttons[i] == 0)
		{
			buttons[i] = newButton;
			return;
		}
	}
}

void Screen::addGraphic(GraphicsBase* newGraphic)
{
	// Find the first available slot and set it to the newGraphic
	for (int i = 0; i < NUM_GRAPHICS; i++)
	{
		if (graphics[i] == 0)
		{
			graphics[i] = newGraphic;
			return;
		}
	}
}

void Screen::setNewScreen(Screen* newScreen)
{
	this->newScreen = newScreen;
}