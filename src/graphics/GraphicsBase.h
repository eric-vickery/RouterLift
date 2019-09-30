#ifndef GRAPHICS_BASE_H
#define GRAPHICS_BASE_H

#include <Arduino.h>
#include <RA8875.h>

struct Point2D
{
	int x;
	int y;
};

struct GraphicPressActions
{
	void* screen;
	void* handlerObject;
};

///@class GraphicsBase
///@brief Base class for drawing text to the touch screen.
class GraphicsBase
{
public:
	/*!
	@class GraphicsBase
	@brief Parameter constructor for the GraphicsBase superclass
	@param  otherGraphicsBase The text that is to be copied.
	*/
	GraphicsBase(GraphicsBase& otherGraphicsBase);

	/*!
	@brief Parameter constructor for the GraphicsBase superclass
	@param tft A pointer to the RA8875 object where the graphic is to be drawn
	@param id The id of the graphic, used to identify the graphic in a press handler
	@param  xStart The x-coordinate for the graphic.
	@param  yStart The y-coordinate for the graphic.
	@param  textColor The color of the text instance. Default color is WHITE.
	*/
	GraphicsBase(RA8875 *tft, int id, int xStart, int yStart, unsigned int textColor = RA8875_WHITE, unsigned int borderColor = RA8875_WHITE, unsigned int fillColor = RA8875_BLACK);

	///@brief Sets the x and y starting position.
	void setXYStart(int xStart, int yStart);

	///@brief Sets the color of the text.
	void setTextColor(unsigned int textColor = RA8875_WHITE);

	///@brief Sets the border color of the button
	void setBorderColor(unsigned int borderColor = RA8875_WHITE);

	///@brief Sets the fill color of the button
	void setFillColor(unsigned int borderColor = RA8875_BLACK);

	///@brief Gets the id of the graphic.
	int getID();

	///@brief Gets the xcoordinate of the graphic.
	int getXStart();

	///@brief Gets the ycoordinate of the graphic.
	int getYStart();

	///@brief Gets the font color of the text.
	unsigned int getTextColor();

	///@brief Gets the border color of the button
	unsigned int getBorderColor();

	///@brief Gets the fill color of the button
	unsigned int getFillColor();

	///@brief Uses the TFT library to draw the graphic.
	virtual void draw() = 0;

	///@brief Erases the graphic
	virtual void erase() = 0;

	void setTFT(RA8875 *newTFT);
	void setPressAction(GraphicPressActions* newActions);
	GraphicPressActions* getPressAction();

protected:
	RA8875 *tft;
	int id;
	int x;
	int y;
	unsigned int borderColor;
	unsigned int fillColor;
	unsigned int textColor;
	GraphicPressActions* pressAction = NULL;
};

#endif
