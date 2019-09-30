#include <Arduino.h>
#include <stdint.h>
#include <math.h>
#include <SPI.h>
#include <Wire.h>
#include <RA8875.h>

#include "RPMGauge.h"

RPMGauge::RPMGauge() : GraphicsBase(NULL, 0, 0, 0)
{
	radius = 0;
	currentRPM = 0;
}

RPMGauge::RPMGauge(RA8875* tft, int id, int x, int y, int radius, unsigned int borderColor, unsigned int fillColor, unsigned int textColor)
: GraphicsBase(tft, id, x, y, textColor, borderColor, fillColor)
{
	this->radius = radius;
	this->dialCenterX = x;
	this->dialCenterY = y;
	this->indicatorLength = radius - 5;
	this->labelRadius = radius + LABEL_RADIUS_INSET;
	this->currentRPM = 0;
}

void RPMGauge::setRPM(int newRPM)
{
	this->currentRPM = newRPM;
}

void RPMGauge::draw()
{
	drawRpmBanner(currentRPM, textColor);
	drawDial(currentRPM);
}

void RPMGauge::erase()
{
	drawRpmBanner(currentRPM, fillColor);
	drawIndicatorHand(currentRPM, fillColor);
}

void RPMGauge::drawRpmBanner(long rpm_value, unsigned int color)
{
	tft->setCursor(x - radius - 20, y + 30);
	tft->setFontScale(2);
	tft->setTextColor(textColor);
	tft->print("RPM: ");
	tft->setTextColor(color);
	tft->print((long)rpm_value);
}

void RPMGauge::drawDial(long rpm_value)
{
	tft->drawArc(dialCenterX, dialCenterY, radius, 1, 270, 90, borderColor);
	drawTickMarks();
	drawMajorTickLabels();
	drawIndicatorHand(rpm_value, borderColor);
}

void RPMGauge::drawTickMarks() 
{
	drawTicks(MAJOR_TICKS, MAJOR_TICK_COUNT, MAJOR_TICK_LENGTH);
	drawTicks(MINOR_TICKS, MINOR_TICK_COUNT, MINOR_TICK_LENGTH);
}

void RPMGauge::drawTicks(const int ticks[], int tick_count, int tick_length) 
{
	for (int tick_index = 0; tick_index < tick_count; tick_index++) 
	{
		long rpm_tick_value = ticks[tick_index];
		float tick_angle = (HALF_CIRCLE_DEGREES * getPercentMaxRpm(rpm_tick_value)) + HALF_CIRCLE_DEGREES;
		uint16_t dial_x = getCircleXWithLengthAndAngle(radius - 1, tick_angle);
		uint16_t dial_y = getCircleYWithLengthAndAngle(radius - 1, tick_angle);
		uint16_t tick_x = getCircleXWithLengthAndAngle(radius - tick_length, tick_angle);
		uint16_t tick_y = getCircleYWithLengthAndAngle(radius - tick_length, tick_angle);
		tft->drawLine(dial_x, dial_y, tick_x, tick_y, borderColor);
	}
}

float RPMGauge::getPercentMaxRpm(long value) 
{
	float ret_value = (value * 1.0)/(DIAL_MAX_RPM * 1.0);
	return ret_value;
}

float RPMGauge::getCircleXWithLengthAndAngle(uint16_t radius, float angle) 
{
	return dialCenterX + radius * cos(angle*PI_RADIANS);
}

float RPMGauge::getCircleYWithLengthAndAngle(uint16_t radius, float angle) 
{
	return dialCenterY + radius * sin(angle*PI_RADIANS);
}

void RPMGauge::drawMajorTickLabels() 
{
	tft->setFontScale(1);
	for (int label_index = 0; label_index < MAJOR_TICK_COUNT; label_index++) 
	{
		int rpm_tick_value = MAJOR_TICKS[label_index];
		float tick_angle = (HALF_CIRCLE_DEGREES	* getPercentMaxRpm(rpm_tick_value)) + HALF_CIRCLE_DEGREES;
		uint16_t dial_x = getCircleXWithLengthAndAngle(labelRadius, tick_angle);
		uint16_t dial_y = getCircleYWithLengthAndAngle(labelRadius, tick_angle);
		tft->setCursor(dial_x - DIAL_LABEL_X_OFFSET, dial_y - DIAL_LABEL_Y_OFFSET);
		int label_value = rpm_tick_value / ONE_K;
		tft->setTextColor(borderColor);
		tft->print(label_value);
	}
}

void RPMGauge::drawIndicatorHand(long rpm_value, unsigned int color) 
{
	float indicator_angle = (HALF_CIRCLE_DEGREES * getPercentMaxRpm(rpm_value)) + HALF_CIRCLE_DEGREES;
	uint16_t indicator_top_x = getCircleXWithLengthAndAngle(indicatorLength, indicator_angle);
	uint16_t indicator_top_y = getCircleYWithLengthAndAngle(indicatorLength, indicator_angle);

	tft->drawTriangle(dialCenterX - INDICATOR_WIDTH / 2,
						 dialCenterY, dialCenterX + INDICATOR_WIDTH / 2,
						 dialCenterY,
						 indicator_top_x, 
						 indicator_top_y, 
						 color);
}
