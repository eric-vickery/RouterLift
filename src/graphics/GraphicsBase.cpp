#include <Arduino.h>
#include <stdint.h>
#include <math.h>
#include <SPI.h>
#include <Wire.h>
#include <avr/pgmspace.h>
#include <RA8875.h>

#include "GraphicsBase.h"

GraphicsBase::GraphicsBase(GraphicsBase& otherGraphicsBase)
{
	tft = otherGraphicsBase.tft;
	id = otherGraphicsBase.id;
	x = otherGraphicsBase.x;
	y = otherGraphicsBase.y;
	textColor = otherGraphicsBase.textColor;
	borderColor = otherGraphicsBase.borderColor;
	fillColor = otherGraphicsBase.fillColor;
	pressAction = otherGraphicsBase.pressAction;
}

GraphicsBase::GraphicsBase(RA8875 *tft, int id, int xStart, int yStart, unsigned int textColor, unsigned int borderColor, unsigned int fillColor)
{
	this->tft = tft;
	this->id = id;
	this->x = xStart;
	this->y = yStart;
	this->textColor = textColor;
	this->borderColor = borderColor;
	this->fillColor = fillColor;
}

void GraphicsBase::setXYStart(int xStart, int yStart)
{
	this->x = xStart;
	this->y = yStart;
}

void GraphicsBase::setTextColor(unsigned int textColor)
{
	this->textColor = textColor;
}

void GraphicsBase::setBorderColor(unsigned int borderColor)
{
	this->borderColor = borderColor;
}

void GraphicsBase::setFillColor(unsigned int borderColor)
{
	this->fillColor = fillColor;
}

int GraphicsBase::getID()
{
	return id;
}

int GraphicsBase::getXStart()
{
	return x;
}

int GraphicsBase::getYStart()
{
	return y;
}

unsigned int GraphicsBase::getTextColor()
{
	return textColor;
}

unsigned int GraphicsBase::getBorderColor()
{
	return borderColor;
}

unsigned int GraphicsBase::getFillColor()
{
	return fillColor;
}

void GraphicsBase::setTFT(RA8875 *newTFT)
{
	this->tft = tft;
}

void GraphicsBase::setPressAction(GraphicPressActions* newAction)
{
	this->pressAction = newAction;
}

GraphicPressActions* GraphicsBase::getPressAction()
{
	return pressAction;
}
