#include <Arduino.h>
#include <stdint.h>
#include <math.h>
#include <SPI.h>
#include <Wire.h>
#include <avr/pgmspace.h>
#include <RA8875.h>

#include "TouchScreenStrings.h"

/*
----------------------------------------------------------------
						 TouchScreenText Superclass
----------------------------------------------------------------
*/

TouchScreenText::TouchScreenText() : GraphicsBase(NULL, 0, 0, RA8875_BLACK)
{
	fontSize = 1;
}

TouchScreenText::TouchScreenText(TouchScreenText& otherTouchScreenText) : GraphicsBase(otherTouchScreenText)
{
	fontSize = otherTouchScreenText.fontSize;
	textColor = otherTouchScreenText.textColor;
	font = otherTouchScreenText.font;
}

TouchScreenText::TouchScreenText(RA8875* tft, int id, int xStart, int yStart, int fontSize, unsigned int textColor, const tFont* font)
: GraphicsBase(tft, id, xStart, yStart, textColor)
{
	this->fontSize = fontSize;
	this->font = font;
}

void TouchScreenText::setFontSize(int fontSize)
{
	this->fontSize = fontSize;
}

int TouchScreenText::getFontSize()
{
	return fontSize;
}

/*
----------------------------------------------------------------
		Functions for the TouchScreenString Subclass
----------------------------------------------------------------
*/

TouchScreenString::TouchScreenString() : TouchScreenText()
{
	this->text = "";
	this->centered = false;
	this->centeredX = -1;
	this->centeredY = -1;
}

TouchScreenString::TouchScreenString(TouchScreenString& otherTouchScreenString) : TouchScreenText(otherTouchScreenString)
{
	this->text = otherTouchScreenString.text;
	this->centered = otherTouchScreenString.centered;
	this->centeredX = otherTouchScreenString.centeredX;
	this->centeredY = otherTouchScreenString.centeredY;
}

TouchScreenString::TouchScreenString(RA8875* tft, int id, const char* text, int xStart, int yStart, int fontSize, unsigned int textColor, const tFont* font)
	 : TouchScreenText(tft, id, xStart, yStart, fontSize, textColor, font)
{
	this->text = text;
	this->centered = false;
	this->centeredX = -1;
	this->centeredY = -1;
}

TouchScreenString::TouchScreenString(RA8875* tft, int id, const char* text, int xStart, int yStart, boolean centered, int fontSize, unsigned int textColor)
	 : TouchScreenText(tft, id, xStart, yStart, fontSize, textColor)
{
	this->text = text;
	this->centered = centered;
	if (centered)
	{
		this->centeredX = xStart;
		this->centeredY = yStart;
		Point2D newStart = calculateTextPosition(xStart, yStart, fontSize, text);
	    // Serial.print("Start position is X:"); Serial.print(newStart.x); Serial.print(" Y:"); Serial.println(newStart.y);
		setXYStart(newStart.x, newStart.y);
	}
}

Point2D TouchScreenString::calculateTextPosition(int x, int y, int fontScale, const char* textString)
{
	int stringLength = strlen(textString);
	tft->setFontScale(fontScale);
    uint8_t fontWidth = tft->getFontWidth();
    // Serial.print("Font width is "); Serial.println(fontWidth);
    uint8_t fontHeight = tft->getFontHeight();
    // Serial.print("Font height is "); Serial.println(fontHeight);
	int heightGap = fontHeight / 2;
	int stringWidth = stringLength * fontWidth;
    // Serial.print("String width is "); Serial.println(stringWidth);
    // Serial.print("String length is "); Serial.println(stringLength);
    // Serial.print("String is "); Serial.println(textString);
	int widthGap = stringWidth / 2;

	Point2D textPosition;
	textPosition.x = x - widthGap;
	textPosition.y = y - heightGap;

	return textPosition;
}

void TouchScreenString::setText(const char* text)
{
	this->text = text;
	if (centered)
	{
		Point2D newStart = calculateTextPosition(centeredX, centeredY, fontSize, text);
	    // Serial.print("Updated start position is X:"); Serial.print(newStart.x); Serial.print(" Y:"); Serial.println(newStart.y);
		setXYStart(newStart.x, newStart.y);
	}
}

const char* TouchScreenString::getText()
{
	return text;
}

void TouchScreenString::textDisplay()
{
	setTextColor(RA8875_RED);
	draw();
	delay(100);
	setTextColor(RA8875_WHITE);
	draw();
}

void TouchScreenString::draw()
{
	if (font != NULL)
	{
		tft->setFont(font);
	}
	tft->setCursor(x, y);
	tft->setTextColor(textColor);
	tft->setFontScale(fontSize);
	tft->println(text);
	if (font != NULL)
	{
		tft->setFont(INT);
	}
}

void TouchScreenString::erase()
{
	if (font != NULL)
	{
		tft->setFont(font);
	}
	tft->setCursor(x, y);
	tft->setTextColor(RA8875_BLACK);
	tft->setFontScale(fontSize);
	tft->println(text);
	if (font != NULL)
	{
		tft->setFont(INT);
	}
}

/*
----------------------------------------------------------------
		Functions for the TouchScreenChar Subclass
----------------------------------------------------------------
*/

TouchScreenChar::TouchScreenChar() : TouchScreenText()
{
	this->text = 0;
}

TouchScreenChar::TouchScreenChar(TouchScreenChar& otherTouchScreenChar) : TouchScreenText(otherTouchScreenChar)
{
	text = otherTouchScreenChar.text;
}

TouchScreenChar::TouchScreenChar(RA8875* tft, int id, char text, int xStart, int yStart, int fontSize, unsigned int textColor)
	 : TouchScreenText(tft, id, xStart, yStart, fontSize, textColor)
{
	this->text = text;
}

void TouchScreenChar::setText(char text)
{
	text = text;
}

const char TouchScreenChar::getText()
{
	return text;
}

void TouchScreenChar::textButtonDisplay()
{
	setTextColor(RA8875_RED);
	draw();
	delay(100);
	setTextColor(RA8875_WHITE);
	draw();
}

void TouchScreenChar::draw()
{
	tft->setCursor(x, y);
	tft->setTextColor(textColor);
	tft->setFontScale(fontSize);
	tft->println(text);
}

void TouchScreenChar::erase()
{
	tft->setCursor(x, y);
	tft->setTextColor(RA8875_BLACK);
	tft->setFontScale(fontSize);
	tft->println(text);
}
