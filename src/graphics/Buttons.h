#ifndef BUTTONS_H_INCLUDED
#define BUTTONS_H_INCLUDED

#include <Arduino.h>
#include <RA8875.h>

#include "GraphicsBase.h"
#include "TouchScreenStrings.h"

///@class Button
///@brief Class for drawing rectangular buttons
class Button : public GraphicsBase
{
public:
	///@brief Default constructor for the Button class
	Button();

	/**
	@brief Parameter constructor for the button class.  This constructor assumes the user does not want any button text or that it will be created later.
	@param tft A pointer to the RA8875 object where the button is to be drawn
	@param id The id of the button, used to identify the button in a press handler
	@param xStart The starting x-coordinate of the button.
	@param yStart The starting y-coordinate of the button.
	@param width The width of the button.
	@param height The height of the button.
	@param rounded True if the button should have rounded corners, false otherwise. Default is false.
	@param borderColor The border color of the button. Default color is white.
	@param fillColor The fill color of the button. Default color is black.
	*/
	Button(RA8875* tft,
		   int id, 
		   int xStart,
		   int yStart,
		   int width,
		   int height,
		   boolean rounded = false,
		   unsigned int borderColor = RA8875_WHITE,
		   unsigned int fillColor = RA8875_BLACK);

	/**
	@brief Parameter constructor for the button class.  This constructor allows the user to specify the button's text.
	@param tft A pointer to the RA8875 object where the button is to be drawn
	@param id The id of the button, used to identify the button in a press handler
	@param buttonTextString The text for the button.
	@param xStart The starting x-coordinate of the button.
	@param yStart The starting y-coordinate of the button.
	@param width The width of the button.
	@param height The height of the button.
	@param rounded True if the button should have rounded corners, false otherwise. Default is false.
	@param borderColor The border color of the button. Default color is white.
	@param fillColor The fill color of the button. Default color is black.
	@param textColor The color of the button's text.  Default is white.
	*/
	Button(RA8875* tft,
		   int id,
		   const char* buttonTextString,
		   int xStart,
		   int yStart,
		   int width,
		   int height,
		   boolean rounded = false,
		   unsigned int borderColor = RA8875_WHITE,
		   unsigned int fillColor = RA8875_BLACK,
		   unsigned int textColor = RA8875_WHITE);

	/**
	@brief Sets the values of the button instance.
	@param xStart The starting x-coordinate of the button.
	@param yStart The starting y-coordinate of the button.
	@param width The width of the button.
	@param height The height of the button.
	*/
	void setValues(int xStart, int yStart, int width, int height);

	/**
	@brief Sets the values for the button's text.
	@param buttonTextString   The text for the button.
	@param xStart The x-coordinate for the text.
	@param yStart The y-coordinate for the text.
	@param fontSize The font size for the text.
	@param textColor The font color for the text.  Default is white.
	*/
	void setTextValues(const char* buttonTextString, int xStart, int yStart, int fontSize, const unsigned int textColor = RA8875_WHITE);

	/**
	@brief Sets the text for the button.
	@param buttonTextString   The text for the button.
	*/
	void setText(const char* buttonTextString);

	/**
	@brief Sets the coordinates for the text
	@param xStart The x-coordinate for the text.
	@param yStart The y-coordinate for the text.
	*/
	void setTextCoord(int xStart, int yStart);

	/**
	@brief Sets the font size for the text.
	@param fontSize The font size for the text.
	*/
	void setFontSize(int fontSize);

	/**
	@brief Sets the size of the button
	@param width      The width of the button.
	@param height     The height of the button.
	*/
	void setSize(int width, int height);

	/**
	@brief Set the radius used for the corner rounding if it is a rounded button
	@param newRadius The radius of the corners
	*/
	void setRadius(int newRadius);

	///@brief Gets the right bound x-coordinate
	int getXEnd();

	///@brief Gets the lower bound y-coordinate
	int getYEnd();

	///@brief Gets the width of the button
	int getWidth();

	///@brief Gets the height of the button
	int getHeight();

	///@brief Gets the button's text
	const char* getText();

	///@brief Gets the font size for the button's text
	int getFontSize();

	int getTextXStart();

	int getTextYStart();

	/**
	@brief Determines if the button was pressed
	@param xScreen The x-coordinate of the user's input.
	@param yScreen The y-coordinate of the user's input.
	@return Returns True if the user pressed the button within the boundaries.  False otherwise.
	*/
	bool isPressed(int xScreen, int yScreen);

	///@brief Draws the button
	void draw();

	///@brief Erases the button
	void erase();

	///@brief Fills the button
	void fill();

	/**
	@brief Displays the button when pressed.
	@param highlightColor The color that button will display when pressed.  Default is red.
	*/
	void buttonDisplay(unsigned int highlightColor = RA8875_RED);

private:
	void createButtonText(const char* buttonTextString, unsigned int buttonTextColor);
	Point2D calculateTextPosition(const char* buttonTextString);
	int calculateFontScale(int stringLength);

private:
	int width;
	int height;
	int radius;
	boolean rounded;
	TouchScreenString* buttonText;
};

/*!
@class RadioButton
@brief Class for drawing a radio button to the Arduino touch screen.
*/
// class RadioButton {
//  public:
// 	/*!
// 	@brief Constructor of the RadioButton class.
// 	@param xStart     The x-coordinate of the RadioButton.
// 	@param yStart     The y-coordinate of the RadioButton.
// 	@param borderColor    The border color of the RadioButton.
// 	@param fillColor      The fill color of the RadioButton.
// 	*/
// 	RadioButton(int xStart, int yStart, const unsigned int borderColor, const unsigned int fillColor);

// 	/*!
// 	@brief Set the xstart and ystart of the RadioButton.
// 	@param xStart     The x-coordinate of the RadioButton.
// 	@param yStart     The y-coordinate of the RadioButton.
// 	*/
// 	void setCenter(int xStart, int yStart);

// 	/*!
// 	@brief Sets the border color of the RadioButton.
// 	@param borderColor    The border color of the RadioButton.
// 	*/
// 	void setBorderColor(const unsigned int borderColor);

// 	/*!
// 	@brief Sets the fill color of the RadioButton.
// 	@param fillColor      The fill color of the RadioButton.
// 	*/
// 	void setFillColor(const unsigned int fillColor);

// 	///@brief Gets the x-coordinate of the RadioButton.
// 	int getXStart();

// 	///@brief Gets the y-coordinate of the RadioButton.
// 	int getYStart();

// 	///@brief Gets the border color of the RadioButton.
// 	const unsigned int getBorderColor();

// 	///@brief Gets the fill color of the RadioButton.
// 	const unsigned int getFillColor();

// 	/*!
// 	@brief Determines if the user's input was within the RadioButton's boundaries.
// 	@param xInput   The x-coordinate of the user's input.
// 	@param yInput   The y-coordinate of the user's input.
// 	@return Returns true if the RadioButton was pressed; false otherwise.
// 	*/
// 	bool isButtonPressed(int xInput, int yInput);

// 	///@brief Resets the button's state and then draws it.  If button is true, it is set to false and vice versa.
// 	void resetButtonState();

// 	///@brief Returns the button's current state.
// 	bool getButtonState();

// 	///@brief Sets the buttons state
// 	void setButtonState(bool newValue);

// 	///@brief Draws the RadioButton to the screen based on the button's state.
// 	void draw();

//  private:
// 	Circle radioButton;
// 	unsigned int savedFillColor; // Used for saving the fill color
// 	bool buttonState; // Saves the state of the button (On/Off)
// };

#endif // TOUCHSCREENBUTTON_H_INCLUDED
