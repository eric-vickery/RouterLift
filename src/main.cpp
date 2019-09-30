#include <stdint.h>
#include <SPI.h>
#include <Wire.h>
#include <avr/pgmspace.h>

#include <RA8875.h>
#include <fonts/designerBlock_36.c>

#define GSL1680_DEBUG_ERROR true
#define GSL1680_DEBUG_INFO true
#include "graphics/GSL1680.h"
#include "screens/Screen.h"
#include "screens/MainScreen.h"
#include "hardware/stepper.h"

//LCD:hardware SPI  CTP:hardware IIC
//Arduino DUE +ER-AS8875+ER-TFTM050A2-3
#define RA8875_INT 4
#define RA8875_CS 10 
#define RA8875_RESET 9
#define GSL1680_WAKE 6
#define GSL1680_INT  7    

// Adafruit_RA8875 tft = Adafruit_RA8875(RA8875_CS, RA8875_RESET);
RA8875 tft = RA8875(RA8875_CS, RA8875_RESET);
GSL1680 ts = GSL1680();

Button* buttons[2];
boolean buttonDown = false;
Screen* currentScreen;

void displayStartupScreen()
{
	tft.clearScreen(RA8875_BLACK);
	tft.setCursor(CENTER, CENTER);
	tft.setTextColor(RA8875_WHITE);
	tft.setFont(&designerBlock_36);
	tft.setFontScale(0);
	tft.println("Serenity Mountain Ranch");
	tft.setCursor(CENTER, (tft.height()/2)+20);
	tft.println("Router Table");
	tft.setFont(INT);
	tft.setFontScale(0);
	tft.setCursor(CENTER,tft.height() - 15);
	tft.println("Touch Screen initialization   Please wait");  
}

void setup() 
{
	pinMode(23, OUTPUT);
	digitalWrite(23, HIGH);
	
	pinMode     (5, OUTPUT);  
	digitalWrite(5, HIGH );
 
	Serial.begin(9600);
	Serial.println("RA8875 start");
	Wire.begin();

	tft.begin(RA8875_800x480);

	tft.displayOn(true);
	tft.GPIOX(true);
	tft.PWMout(1, 255);

	displayStartupScreen();

	Serial.println("Initializing GSL1680");
	ts.begin(GSL1680_WAKE, GSL1680_INT);
	Serial.println("Done initializing GSL1680");

	Stepper::getInstance();

	tft.clearScreen(RA8875_BLACK);

	currentScreen = new MainScreen(&tft, &ts);
}

void loop() 
{
	Screen* nextScreen = currentScreen->display(true);
	if (nextScreen != NULL)
	{
		currentScreen = nextScreen;
	}
}
