#ifndef LCDManager_h
#define LCDManager_h
#include <Arduino.h>
#include "../libraries/LiquidCrystal_I2C.h"

class LCDManager : public LiquidCrystal_I2C{
    public: 
        LCDManager();
        void printText(String attributeName, int value, int row);
        void printText(String attributeName, String value, int row);
        void printText(String value);
        void printText(String value, int row);
        void begin();
        void clear();
    private:
};

#endif