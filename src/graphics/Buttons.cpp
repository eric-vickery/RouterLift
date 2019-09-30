#include <Arduino.h>
#include <stdint.h>
#include <math.h>
#include <SPI.h>
#include <Wire.h>
#include <RA8875.h>

#include "Buttons.h"
#include "TouchScreenStrings.h"
#include "GSL1680.h"

#define DEFAULT_RADIUS 10

#define TITLE_TEXT_ID	1

Button::Button() : GraphicsBase(NULL, 0, 0, 0)
{
	width = 0;
	height = 0;
	radius = 0;
	rounded = false;
}

Button::Button(RA8875* tft, int id, int xStart, int yStart, int width, int height, boolean rounded, unsigned int borderColor, unsigned int fillColor)
: GraphicsBase(tft, id, xStart, yStart, RA8875_WHITE, borderColor, fillColor)
{
	this->width = width;
	this->height = height;
	this->radius = DEFAULT_RADIUS;
	this->rounded = rounded;
	this->buttonText = NULL;
}

Button::Button(RA8875* tft, int id, const char* buttonTextString, int xStart, int yStart, int width, int height, boolean rounded, unsigned int borderColor, unsigned int fillColor, unsigned int textColor)
: GraphicsBase(tft, id, xStart, yStart, textColor, borderColor, fillColor)
{
	this->width = width;
	this->height = height;
	this->radius = DEFAULT_RADIUS;
	this->rounded = rounded;
	createButtonText(buttonTextString, textColor);
}

void Button::createButtonText(const char* buttonTextString, unsigned int buttonTextColor)
{
	int stringLength = strlen(buttonTextString);
	int fontScale = calculateFontScale(stringLength);
	Point2D textPosition = calculateTextPosition(buttonTextString);

	buttonText = new TouchScreenString(tft, TITLE_TEXT_ID, buttonTextString, textPosition.x, textPosition.y, fontScale, buttonTextColor);
}

Point2D Button::calculateTextPosition(const char* buttonTextString)
{
	int stringLength = strlen(buttonTextString);
	int fontScale = calculateFontScale(stringLength);
    // Serial.print("Font scale is "); Serial.println(fontScale);
	tft->setFontScale(fontScale);
    uint8_t fontWidth = tft->getFontWidth();
    uint8_t fontHeight = tft->getFontHeight();
	int heightGap = (height - fontHeight)/2;
	int stringWidth = stringLength * fontWidth;
	int widthGap = (width - stringWidth)/2;

	Point2D textPosition;
	textPosition.x = x + widthGap;
	textPosition.y = y + heightGap;

	return textPosition;
}

int Button::calculateFontScale(int stringLength)
{
	int fontScale = 0;

	tft->setFontScale(0);
    uint8_t fontWidth = tft->getFontWidth();
    uint8_t fontHeight = tft->getFontHeight();
	int stringWidth = stringLength * fontWidth;
	int fontHeightRatio = height / fontHeight;
	int fontWidthRatio = width / stringWidth;

	if (fontHeightRatio >= 4 && fontWidthRatio >= 4)
	{
		fontScale = 3;
	}
	else if (fontHeightRatio >= 3 && fontWidthRatio >= 3)
	{
		fontScale = 2;
	}
	else if (fontHeightRatio >= 2 && fontWidthRatio >= 2)
	{
		fontScale = 1;
	}

	return fontScale;
}

void Button::setValues(int xStart, int yStart, int width, int height)
{
	x = xStart;
	y = yStart;
	width = width;
	height = height;
}

void Button::setTextValues(const char* buttonTextString, int xStart, int yStart, int fontSize, const unsigned int textColor)
{
	if (buttonText != NULL)
	{
		buttonText->setText(buttonTextString);
		buttonText->setXYStart(xStart, yStart);
		buttonText->setFontSize(fontSize);
		buttonText->setTextColor(textColor);
	}
}

void Button::setText(const char* buttonTextString)
{
	if (buttonText != NULL)
	{
		int stringLength = strlen(buttonTextString);
		int fontScale = calculateFontScale(stringLength);
		Point2D textPosition = calculateTextPosition(buttonTextString);

		setTextValues(buttonTextString, textPosition.x, textPosition.y, fontScale, textColor);
	}
}

void Button::setTextCoord(int xStart, int yStart)
{
	if (buttonText != NULL)
	{
		buttonText->setXYStart(xStart, yStart);
	}
}

void Button::setFontSize(int fontSize)
{
	if (buttonText != NULL)
	{
		buttonText->setFontSize(fontSize);
	}
}

void Button::setSize(int width, int height)
{
	this->width = width;
	this->height = height;

	// Update Text placement
	if (buttonText != NULL)
	{
	}
}

void Button::setRadius(int newRadius)
{
	radius = newRadius;
}

int Button::getXEnd()
{
	return x + width;
}

int Button::getYEnd()
{
	return y + height;
}

int Button::getWidth()
{
	return width;
}

int Button::getHeight()
{
	return height;
}

const char* Button::getText()
{
	const char* text;

	if (buttonText != NULL)
	{
		text = buttonText->getText();
	}
	else
	{
		text = "";
	}
	
	return text;
}

int Button::getTextXStart()
{
	int buttonX = 0;

	if (buttonText != NULL)
	{
		buttonX = buttonText->getXStart();
	}
	
	return buttonX;
}

int Button::getTextYStart()
{
	int buttonY = 0;

	if (buttonText != NULL)
	{
		buttonY = buttonText->getYStart();
	}
	
	return buttonY;
}

int Button::getFontSize()
{
	int fontSize = 0;

	if (buttonText != NULL)
	{
		fontSize = buttonText->getFontSize();
	}
	
	return fontSize;
}

bool Button::isPressed(int xInput, int yInput)
{
	return ((xInput > getXStart() && xInput < getXEnd()) && (yInput > getYStart() && yInput < getYEnd()));
}

void Button::draw()
{
	fill();

	if (rounded)
	{
		tft->drawRoundRect(x, y, width, height, radius, borderColor);
	}
	else
	{
		tft->drawRect(x, y, width, height, borderColor);
	}
	if (buttonText != NULL)
	{
		buttonText->draw();
	}
}

void Button::erase()
{
	// Implement this
}

void Button::fill()
{
	if (rounded)
	{
		tft->fillRoundRect(x, y, width, height, radius, fillColor);
	}
	else
	{
		tft->fillRect(x, y, width, height, fillColor);
	}
}

void Button::buttonDisplay(unsigned int highlightColor)
{
	unsigned int originalBorderColor = getBorderColor(); ///< Saves the original border color
	unsigned int originalTextColor = buttonText != NULL ? buttonText->getTextColor() : RA8875_BLACK; ///< Saves the original text color
	setBorderColor(highlightColor);

	if (buttonText != NULL)
	{
		buttonText->setTextColor(highlightColor);
	}

	draw();
	delay(100);
	setBorderColor(originalBorderColor);

	if (buttonText != NULL)
	{
		buttonText->setTextColor(originalTextColor);
	}

	draw();
}

/*
----------------------------------------------------------------
						RadioButton Class
----------------------------------------------------------------
*/
// RadioButton::RadioButton(int xStart, int yStart, const unsigned int borderColor, const unsigned int fillColor)
// 	: radioButton(xStart, yStart, 10, borderColor, fillColor), savedFillColor(fillColor), buttonState(false)
// {

// }

// void RadioButton::setCenter(int xStart, int myStart)
// {
// 	radioButton.setCenter(xStart, myStart);
// }

// void RadioButton::setBorderColor(const unsigned int borderColor)
// {
// 	radioButton.setBorderColor(borderColor);
// }

// void RadioButton::setFillColor(const unsigned int fillColor)
// {
// 	radioButton.setFillColor(fillColor);
// 	savedFillColor = fillColor;
// }

// int RadioButton::getXStart()
// {
// 	return radioButton.getXCoord();
// }

// int RadioButton::getYStart()
// {
// 	return radioButton.getYCoord();
// }

// const unsigned int RadioButton::getBorderColor()
// {
// 	return radioButton.getBorderColor();
// }

// const unsigned int RadioButton::getFillColor()
// {
// 	return savedFillColor;
// }

// bool RadioButton::isButtonPressed(int xInput, int yInput)
// {
// 	// Button is pressed if the input falls within the boundaries of the button
// 	return (pow(xInput - radioButton.getXCoord(),2) + pow(yInput - radioButton.getYCoord(),2) <= pow(radioButton.getRadius(), 2));
// }

// void RadioButton::draw()
// {
// 	if (buttonState) {
// 		radioButton.draw();
// 		radioButton.setFillColor(savedFillColor);
// 		radioButton.fill();
// 	} else {
// 		radioButton.draw();
// 		radioButton.setFillColor(0x0000);
// 		radioButton.fill();
// 	}
// }

// void RadioButton::resetButtonState()
// {
// 	buttonState = !buttonState;
// 	draw();
// }

// bool RadioButton::getButtonState()
// {
// 	return buttonState;
// }

// void RadioButton::setButtonState(bool newValue)
// {
// 	buttonState = newValue;
// }
