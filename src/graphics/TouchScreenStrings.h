#ifndef TOUCHSCREENSTRINGS_H
#define TOUCHSCREENSTRINGS_H

#include <Arduino.h>

#include "GraphicsBase.h"

/*
----------------------------------------------------------------
				  TouchScreenText Superclass
----------------------------------------------------------------
*/

///@class TouchScreenText
///@brief Base class for drawing text to the touch screen.
class TouchScreenText : public GraphicsBase
{
public:
	/*!
	@brief Default constructor for the TouchScreenText superclass
	*/
	TouchScreenText();

	/*!
	@class TouchScreenText
	@brief Parameter constructor for the TouchScreenText superclass
	@param  otherTouchScreenText The text that is to be copied.
	*/
	TouchScreenText(TouchScreenText& otherTouchScreenText);

	/*!
	@brief Parameter constructor for the TouchScreenText superclass
	@param tft A pointer to the RA8875 object where the text is to be drawn
	@param id The id of the text, used to identify the text in a press handler
	@param  xStart    The x-coordinate for the text instance.
	@param  yStart    The y-coordinate for the text instance.
	@param  fontSize  The font size of the text instance.
	@param  textColor     The color of the text instance. Default color is WHITE.
	@param  font A pointer to a tFont to use when drawing the text
	*/
	TouchScreenText(RA8875 *tft, int id, int xStart, int yStart, int fontSize, unsigned int textColor, const tFont* font = NULL);

	 /*!
	 @brief Sets the font size of the text.
	 @param  fontSize  The font size of the text instance.
	 */
	 void setFontSize(int fontSize = 2);

	 /*!
	 @brief Gets the font size of the text.
	 @return The font size for the text instance.
	 */
	 int getFontSize();

protected:
	int fontSize; /// Size of the text
	const tFont* font;
};

/*
----------------------------------------------------------------
				  TouchScreenString Subclass
----------------------------------------------------------------
*/

/*!
@class TouchScreenString
@brief Subclass of TouchScreenText. Abstract class for drawing strings to the touch screen.
*/
class TouchScreenString : public TouchScreenText 
{
 public:
	/*!
	@brief Default constructor for the TouchScreenString subclass.
	@param  text The text that is to be drawn.
	*/
	TouchScreenString();

	/*!
	@brief Copy constructor for the TouchScreenString superclass
	@param  otherTouchScreenString The string instance that is to be copied.
	*/
	TouchScreenString(TouchScreenString& otherTouchScreenString);

	/*!
	@brief Parameter constructor for the TouchScreenString superclass.
	@param tft A pointer to the RA8875 object where the text is to be drawn
	@param id The id of the text, used to identify the text in a press handler
	@param  text The text that is to be drawn.
	@param  xStart The x-coordinate for the text instance.
	@param  yStart The y-coordinate for the text instance.
	@param  fontSize The font size of the text instance.
	@param  textColor The color of the text instance.
	@param  font A pointer to a tFont to use when drawing the text
	*/
	TouchScreenString(RA8875* tft, int id, const char* text, int xStart, int yStart, int fontSize, unsigned int textColor, const tFont* font = NULL);

	/*!
	@brief Parameter constructor for the TouchScreenString superclass.
	@param tft A pointer to the RA8875 object where the text is to be drawn
	@param id The id of the text, used to identify the text in a press handler
	@param  text The text that is to be drawn.
	@param  xStart The x-coordinate for the text instance.
	@param  yStart The y-coordinate for the text instance.
	@param  center True if the text should be centered around xStart, false otherwise
	@param  fontSize The font size of the text instance.
	@param  textColor The color of the text instance.
	*/
	TouchScreenString(RA8875* tft, int id, const char* text, int xStart, int yStart, boolean centered, int fontSize, unsigned int textColor);

	 /*!
	 @brief Sets the text of the string instance.
	 @param  text The text that is to be drawn.
	 */
	 void setText(const char* text);

	 /*!
	 @brief Returns the text of the string instace.
	 @return text The text that is to be drawn.
	 */
	 const char* getText();

	 void textDisplay();

	 void draw();

	 void erase();

private:
	Point2D calculateTextPosition(int x, int y, int fontScale, const char* textString);

 private:
	 const char* text;
	 boolean centered;
	 int centeredX;
	 int centeredY;
};

/*
----------------------------------------------------------------
				  TouchScreenChar Subclass
----------------------------------------------------------------
*/

///@class TouchScreenChar
///@brief Subclass of TouchScreenText.  Abstract class used for drawing char's to the touch screen.
class TouchScreenChar : public TouchScreenText 
{
 public:
	 /*!
	 @brief Default constructor for the TouchScreenString subclass.
	 */
	 TouchScreenChar();

	 /*!
	 @brief Copy constructor for the TouchScreenString subclass.
	 @param  otherTouchScreenChar The char instance that is being copied.
	 */
	 TouchScreenChar(TouchScreenChar& otherTouchScreenChar);

	/*!
	@brief Parameter constructor for the TouchScreenString superclass.
	@param tft A pointer to the RA8875 object where the text is to be drawn
	@param id The id of the text, used to identify the text in a press handler
	@param  text The char that is to be drawn.
	@param  xStart The x-coordinate for the char instance.
	@param  yStart The y-coordinate for the char instance.
	@param  fontSize The font size of the char instance.
	@param  textColor The color of the char instance.
	*/
	TouchScreenChar(RA8875* tft, int id, char text, int xStart, int yStart, int fontSize, unsigned int textColor);

	 /*!
	 @brief Sets the text of the char instance.
	 @param  text The char that is to be drawn.
	 */
	 void setText(char text);

	 /*!
	 @brief Returns the char of the char instance.
	 @return The char that is to be drawn.
	 */
	 const char getText();

	 void textButtonDisplay();
	 
	 void draw();

	 void erase();

 private:
	 char text;
};

#endif
