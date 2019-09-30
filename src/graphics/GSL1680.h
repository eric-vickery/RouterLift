#ifndef GSL1680_H
#define GSL1680_H

#include <stdint.h>

class GSL1680 {
    public:
        GSL1680 ();
        void      begin(uint8_t WAKE, uint8_t INTRPT);
        uint8_t   dataread();
        uint8_t   readFingerID(int NB);
        uint32_t  readFingerX(int NB);
        uint32_t  readFingerY(int NB);
        boolean   hasData();

    private:
        void      clear_reg();
        void      reset();
        void      loadfw();
        void      startchip();
        void      sleep();
        void      datasend(uint8_t REG, uint8_t DATA[], uint16_t NB);
        uint8_t   i2cWrite(uint8_t regAddr, uint8_t *val, uint16_t cnt);
        void      printI2CError(uint8_t retVal, uint8_t regVal);
        uint8_t   intPin;
        uint8_t   wakePin;
};

struct Finger {
    uint8_t fingerID;
    uint32_t x;
    uint32_t y;
};

struct Touch_event {
    uint8_t NBfingers;
    struct Finger fingers[5];
};

#endif
