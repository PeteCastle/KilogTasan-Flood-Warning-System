#ifndef LCDManager_h
#define LCDManager_h
#include <Arduino.h>
#include "../libraries/LiquidCrystal_I2C.h"

class LCDManager : public LiquidCrystal_I2C{
    public: 
        LCDManager(uint8_t  address,  uint8_t colCount = 16, uint8_t rowCount=2);
        void printText(String attributeName, int value, int row);
        void printText(String attributeName, String value, int row);
        void printText(String value);
        void printText(String value, int row);
        void begin();
        void clear();
    private:
        uint8_t _address;
        uint8_t _colCount = 16;
        uint8_t _rowCount = 2;
};

#endif