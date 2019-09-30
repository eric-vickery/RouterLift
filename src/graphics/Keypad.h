#ifndef KEYPAD_H
#define KEYPAD_H

#include <Arduino.h>
#include "TouchScreenStrings.h"
#include "Buttons.h"

/*
----------------------------------------------------------------
                        Keypad
----------------------------------------------------------------
*/

///@class Keypad
///@brief Class for handling all the TFT Touch Screen keypad functions
class Keypad 
{
 public:
    ///@brief Default constructor for the Keypad class
    Keypad();

    ///@brief Draws the keypad to the scree
    void drawKeypad();

    ///@brief Highlights the button when pressed
    ///@param  userInput    The button that was selected by the user.
    void displayButton(const int userInput);

    /**
    @brief Gets the button number that was pressed.  Returns -1 if no button was pressed.
    @param  xInput   The x-coordinate of the user's input.
    @param  yInput   The y-coordinate of the user's input.
    */
    int getButtonNumber(int xInput, int yInput);

    /**
    @brief Displays the text to the screen
    @param  buttonText  The text for the button
    */
    void displayText(char buttonText);

    ///@brief Clears the textblock and resets the cursor to the original position.
    void clearTextBlock();

   void setTFT(RA8875 *newTFT);

 private:
    Button button[12];
    TouchScreenChar buttonText[12];
   //  Rectangle textBlock;
    int xCursor; ///< Used for positioning the text on the screen
    RA8875 *tft;
};

#endif
