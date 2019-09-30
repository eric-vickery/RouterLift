#include <Arduino.h>
#include <Wire.h>

#include <stdint.h>

#include "GSL1680.h"
#include "gslX680firmware.h"

// Registres
#define I2CADDR 0x40
#define DATA_REG 0x80
#define STATUS_REG 0xE0
#define PAGE_REG 0xF0

//CONST
#define BUFSIZE 32

#ifndef GLS1680_DEBUG_ERROR
#define GSL1680_DEBUG_ERROR true  //(false == serial debug error output off, true == serial debug error output on)
#endif
#ifndef GLS1680_DEBUG_INFO
#define GSL1680_DEBUG_INFO true  //(false == serial debug informational output off, true == serial debug informational output on)
#endif

#define SERIAL_ERROR if(GSL1680_DEBUG_ERROR)Serial
#define SERIAL_INFORMATION if(GSL1680_DEBUG_INFO)Serial

struct Touch_event ts_event;

GSL1680::GSL1680() { }

void GSL1680::begin(uint8_t WAKE, uint8_t INTRPT)
{
    intPin = INTRPT;
    wakePin = WAKE;
    SERIAL_INFORMATION.println("GSL1680: Start boot up sequence");
    pinMode(WAKE, OUTPUT);
    digitalWrite(WAKE, LOW);
    pinMode(INTRPT, INPUT_PULLUP);
    delay(10);

    SERIAL_INFORMATION.println("Toggle Wake");
	digitalWrite(WAKE, HIGH);
	delay(50);
	digitalWrite(WAKE, LOW);
	delay(50);
	digitalWrite(WAKE, HIGH);
	delay(30);

    Wire.begin();

    // CTP startup sequence
	SERIAL_INFORMATION.println("Clear reg");
	clear_reg();
	SERIAL_INFORMATION.println("Reset chip");
	reset();
	SERIAL_INFORMATION.println("GSL1680: Load FW");
	loadfw();
	//startup_chip();
	SERIAL_INFORMATION.println("Reset chip 2");
	reset();
	SERIAL_INFORMATION.println("Startup Chip");
    startchip();
	SERIAL_INFORMATION.println("GSL1680: Boot up complete");
}

void GSL1680::clear_reg()
{
    uint8_t REG[4] = {STATUS_REG, DATA_REG, 0xE4, STATUS_REG};
    uint8_t DATA[4] = {0x88, 0x03, 0x04, 0x00};
    uint8_t TIMER[4] = {20, 5, 5, 20};

    int i;
    for (i = 0; i < 4; ++i) {
        printI2CError(i2cWrite(REG[i], &DATA[i], 1), REG[i]);
        delay(TIMER[i]);
    }
}

void GSL1680::reset()
{
    uint8_t REG[2] = {STATUS_REG, 0xE4};
    uint8_t DATA[2] = {0x88, 0x04};
    uint8_t TIMER[2] = {20, 10};

    int i;
    for (i = 0; i < 2; ++i) {
        printI2CError(i2cWrite(REG[i], &DATA[i], 1), REG[i]);
        delay(TIMER[i]);
    }

    uint8_t regVal = 0xBC;
    uint8_t DATA_2[4] = {0};

    printI2CError(i2cWrite(regVal, DATA_2, 4), regVal);
    delay(10);
}

void GSL1680::loadfw()
{
    uint8_t addr;
    uint8_t Wrbuf[4];
    size_t source_len = sizeof(GSLX680_FW) / sizeof(struct fw_data);

    for (size_t source_line = 0; source_line < source_len; source_line++) {
        addr = GSLX680_FW[source_line].offset;
        Wrbuf[0] = (char)(GSLX680_FW[source_line].val & 0x000000ff);
        Wrbuf[1] = (char)((GSLX680_FW[source_line].val & 0x0000ff00) >> 8);
        Wrbuf[2] = (char)((GSLX680_FW[source_line].val & 0x00ff0000) >> 16);
        Wrbuf[3] = (char)((GSLX680_FW[source_line].val & 0xff000000) >> 24);

        printI2CError(i2cWrite(addr, Wrbuf, 4), addr);
    }
}

void GSL1680::startchip()
{
    uint8_t data[1] = {0x00};

    printI2CError(i2cWrite(STATUS_REG, data, 1), STATUS_REG);
}

void GSL1680::sleep()
{
}

uint8_t GSL1680::i2cWrite(uint8_t regAddr, uint8_t *val, uint16_t cnt)
{
    uint16_t i = 0;
	
    Wire.beginTransmission(I2CADDR);
    Wire.write(regAddr);
    for(i = 0; i < cnt; i++, val++)
    {		
        Wire.write(*val);
    }
    uint8_t retVal = Wire.endTransmission(true); 

    return retVal;
}

void GSL1680::printI2CError(uint8_t retVal, uint8_t regVal)
{
    if (retVal != 0)
    {
        SERIAL_ERROR.print("i2c write error: "); SERIAL_ERROR.print(retVal); SERIAL_ERROR.print(" "); SERIAL_ERROR.println(regVal, HEX);
    }
}

uint8_t GSL1680::dataread()
{
    uint8_t TOUCHRECDATA[24] = {0};

    Wire.beginTransmission(I2CADDR);

    Wire.write(DATA_REG);

    int n = Wire.endTransmission(true);
    if (n != 0) 
    {
        SERIAL_ERROR.print("i2c write error: "); SERIAL_ERROR.print(n); SERIAL_ERROR.print(" "); SERIAL_ERROR.println(DATA_REG, HEX);
    }

    n = Wire.requestFrom(I2CADDR, 24);
    if (n != 24) 
    {
        SERIAL_ERROR.print("i2c read error: did not get expected count "); SERIAL_ERROR.print(n); SERIAL_ERROR.print("/"); SERIAL_ERROR.println("24");
    }

    for(int i = 0; i<n; i++) 
    {
        TOUCHRECDATA[i] = Wire.read();
    }

    ts_event.NBfingers = TOUCHRECDATA[0];
    for(int i = 0; i<ts_event.NBfingers; i++) 
    {
        ts_event.fingers[i].x = ( (((uint32_t)TOUCHRECDATA[(i*4)+5])<<8) | (uint32_t)TOUCHRECDATA[(i*4)+4] ) & 0x00000FFF; // 12 bits of X coord
        ts_event.fingers[i].y = ( (((uint32_t)TOUCHRECDATA[(i*4)+7])<<8) | (uint32_t)TOUCHRECDATA[(i*4)+6] ) & 0x00000FFF;
        ts_event.fingers[i].fingerID = (uint32_t)TOUCHRECDATA[(i*4)+7] >> 4; // finger that did the touch
    }

    return ts_event.NBfingers;
}

uint8_t GSL1680::readFingerID(int NB)
{
    return ts_event.fingers[NB].fingerID;
}

uint32_t GSL1680::readFingerX(int NB)
{
    return ts_event.fingers[NB].x;
}

uint32_t GSL1680::readFingerY(int NB)
{
    return ts_event.fingers[NB].y;
}

boolean GSL1680::hasData()
{
    return digitalRead(intPin) == HIGH;
}
