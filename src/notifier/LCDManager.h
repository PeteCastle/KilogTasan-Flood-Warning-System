#ifndef LCDManager_h
#define LCDManager_h
#include "../libraries/LiquidCrystal_I2C.h"

class LCDManager : public LiquidCrystal_I2C{
    LCDManager(uint8_t  address,  int colCount = 16, int rowCount=2);
    void printText(String attributeName, int value, int row);
    void printText(String attributeName, String value, int row);
    void printText(String value);
};

#endif