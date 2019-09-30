#include <Arduino.h>
#include <stdint.h>
#include <math.h>
#include <SPI.h>
#include <Wire.h>
#include <avr/pgmspace.h>
#include <RA8875.h>

#include "Keypad.h"
#include "Buttons.h"
#include "TouchScreenStrings.h"

/*
----------------------------------------------------------------
                        Keypad
----------------------------------------------------------------
*/

Keypad::Keypad() : xCursor(20)
{
}

void Keypad::drawKeypad()
{

    const int xstartButton[] = {15, 95, 175};           // x-min for keypads
    const int ystartButton[] = {65, 127, 189, 251};     // y-min for keypads
    // const int xendButton[] = {65, 145, 225};            // x-max for keypads
    // const int yendButton[] = {115, 177, 239, 301};      // y-min for keypads
    const int xstartButtonText[] = {35, 115, 195};      // x-coordinate for keypad numbers
    const int ystartButtonText[] = {85, 145, 209, 271}; // y-coordinate for keypad numbers
    const int widthButton = 50;
    const int heightButton = widthButton;
    char textKeypad[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', 'C', '0', 'E'}; // Text for the buttons

    // Sets the xstart, ystart, width, and height of the textblock and then draws it
    // Rectangle textBlock(15, 15, 216, 35, RA8875_WHITE, RA8875_BLACK);
    // textBlock.draw();

    // Sets the coordinates and sizes of the keypad buttons, and sets all the values for the buttons' text
    const int noRows = 4;
    const int noColumns = 3;
    for(int y = 0; y < noRows; y++) {
        for(int x = 0; x < noColumns; x++) {
            int buttonNumber = noColumns * y + x;
            button[buttonNumber].setValues(xstartButton[x], ystartButton[y], widthButton, heightButton);
            // buttonText[buttonNumber].setValues(textKeypad[buttonNumber], xstartButtonText[x], ystartButtonText[y], 1, RA8875_WHITE);
    }
  }

    // Draws the keypads and their text
    for(int x = 0; x < 12; x++) {
        button[x].draw();
        buttonText[x].draw();
  }
}

void Keypad::displayButton(const int userInput)
{
  // Only allow valid buttons to be displayed to prevent errors
  if (userInput >= 0 && userInput <= 12) {
      button[userInput].buttonDisplay();
      buttonText[userInput].textButtonDisplay();
  }
}

int Keypad::getButtonNumber(int xInput, int yInput)
{
    for (int i = 0; i < 12; i++) {
        if ((xInput > button[i].getXStart() && xInput < button[i].getXEnd())
            && (yInput > button[i].getYStart() && yInput < button[i].getYEnd()))
                return i;
    }

    // If no button is presed, return -1 signifying no button was pressed
    return -1;
}


void Keypad::displayText(char buttonText)
{
  // Ensures text input is not beyond width of screen and only a valid button.
  if (xCursor > 200) {
    // If true, it resets the input and notifies user.
    clearTextBlock();

   tft->setCursor(65, 30);
   tft->setTextColor(RA8875_WHITE);
   tft->setFontScale(1);
   tft->print("Too many inputs");

    delay(2000);
    clearTextBlock();
  } 
  else 
  {
      tft->setCursor(25, 2);
      tft->setTextColor(RA8875_WHITE);
      tft->setFontScale(2);
      tft->print(buttonText);
      xCursor += 20;
  }
}

void Keypad::clearTextBlock()
{
    tft->fillRect(16, 16, 214, 33, RA8875_BLACK);
    delay(10);
    xCursor = 20;
}

void Keypad::setTFT(RA8875 *newTFT)
{
  tft = newTFT;
  for (int i = 0; i < 12; i++)
  {
      button[i].setTFT(newTFT);
      buttonText[i].setTFT(newTFT);
  }
  // textBlock.setTFT(newTFT);
}
