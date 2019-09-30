#ifndef SCREEN_H_INCLUDED
#define SCREEN_H_INCLUDED

#include <Arduino.h>
#include <RA8875.h>

#include "../graphics/GSL1680.h"
#include "../graphics/Buttons.h"
#include "../graphics/GraphicsBase.h"

#define UNNEEDED_ID		0
#define BACK_BUTTON_ID  100
#define TITLE_TEXT_ID   101

#define NUM_BUTTONS     10
#define NUM_GRAPHICS    20

#define PROCESSING_DELAY_MILLIS     500

///@class Screen
///@brief Abstract class for drawing screens using buttons, labels, etc...
class Screen
{
    public:
        /**
        @brief Default constructor for the Screen class
        @param tft A pointer to the RA8875 object for the screen.
        @param ts A pointer to the GSL1680 object for the touch screen.
        */
        Screen(RA8875* tft, GSL1680* ts);

        /**
        @brief Parameter constructor for the Screen class.
        @param tft A pointer to the RA8875 object for the screen.
        @param ts A pointer to the GSL1680 object for the touch screen.
        @param title The title of the screen.
        @param hasBackButton True if the back button should be shown, false otherwise. Default is false.
        @param previousScreen If hasBackButton is true then this is the screen that should be shown when the back button is pressed. Default is null.
        */
        Screen(RA8875* tft, GSL1680* ts, const char* title, boolean hasBackButton = false, Screen* previousScreen = NULL);

        /**
        @brief Displays the screen and looks for button presses.
        @return Returns a pointer to the screen associated with the button that was pressed
        */
        Screen* display(boolean clearScreen);

        /**
        @brief Sets the title of the screen.
        @param newTitle The new title for the screen.
        */
        void setTitle(const char* newTitle);

        /**
        @brief Adds a new button to the screen. If all the button slots are full it will just ignore the new button.
        @param newButton The button to add.
        */
        void addButton(Button* newButton);

        /**
        @brief Adds a new graphic to the screen. If all the graphic slots are full it will just ignore the new graphic.
        @param newGraphic The graphic to add.
        */
        void addGraphic(GraphicsBase* newGraphic);

        virtual void doProcessing() = 0;

        /**
        @brief Called to let the screen handle button presses. Must be implemented by sub-classes
        @param buttonID The ID of the button that was pressed
        @return true if the handled button should act like a back button and return to the previous screen, false otherwise. This provides dialog like functionality.
        */
        virtual boolean handleButtonPressAction(int buttonID) = 0;

    protected:
        void drawButtons();
        void eraseButtons();
        void drawGraphics();
        void eraseGraphics();
        void drawTitle();
        void drawBackButton();
        void initializeButtonsArray();
        void initializeGraphicsArray();
        void createBackButton(Screen* previousScreen);
        void createTitle(const char* titleText);
        void setNewScreen(Screen* newScreen);


    protected:
        RA8875* tft;
        GSL1680* ts;
        Button* buttons[NUM_BUTTONS];
        Button* backButton;
        Screen* previousScreen;
        TouchScreenString* title;
        GraphicsBase* graphics[NUM_GRAPHICS];
        boolean buttonDown = false;
        Screen* newScreen;
};

#endif
