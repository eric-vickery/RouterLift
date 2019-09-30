#ifndef RPM_GAUGE_H_INCLUDED
#define RPM_GAUGE_H_INCLUDED

#include <Arduino.h>
#include <RA8875.h>

#include "GraphicsBase.h"

#define TEXT_SIZE_SMALL			1
#define TEXT_SIZE_LARGE			2
#define ONE_K					1000

#define LABEL_RADIUS_INSET		20
#define INDICATOR_WIDTH			5
#define DIAL_LABEL_Y_OFFSET		20
#define DIAL_LABEL_X_OFFSET		8

#define MAJOR_TICK_COUNT		4
#define MAJOR_TICK_LENGTH		14
#define MINOR_TICK_COUNT		3
#define MINOR_TICK_LENGTH		7

#define DIAL_MAX_RPM 			30000

#define HALF_CIRCLE_DEGREES		180
#define PI_RADIANS				PI/HALF_CIRCLE_DEGREES

class RPMGauge : public GraphicsBase
{
public:
	RPMGauge();
	RPMGauge(RA8875* tft, int id, int x, int y, int radius, unsigned int borderColor, unsigned int fillColor, unsigned int textColor);

	void draw();
	void erase();

	void setRPM(int newRPM);

private:
	void drawRpmBanner(long rpm_value, unsigned int color);
	void drawDial(long rpm_value);
	void drawTickMarks();
	void drawTicks(const int ticks[], int tick_count, int tick_length);
	float getPercentMaxRpm(long value);
	float getCircleXWithLengthAndAngle(uint16_t radius, float angle);
	float getCircleYWithLengthAndAngle(uint16_t radius, float angle);
	void drawMajorTickLabels();
	void drawIndicatorHand(long rpm_value, unsigned int color);

private:
	const int MAJOR_TICKS[MAJOR_TICK_COUNT] = {0, 10000, 20000, 30000};
	const int MINOR_TICKS[MINOR_TICK_COUNT] = {5000, 15000, 25000};
	int dialCenterX;
	int dialCenterY;
	int indicatorLength;
	int labelRadius;
	int radius;
	int currentRPM;
};

#endif
