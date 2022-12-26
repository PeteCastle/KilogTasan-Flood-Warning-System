#include "LCDManager.h"

LCDManager::LCDManager(uint8_t  address,  uint8_t colCount, uint8_t rowCount):
    _address(address),
    _colCount(colCount),
    _rowCount(rowCount),
    LiquidCrystal_I2C(address, colCount, rowCount){
    
}
void LCDManager::begin(){
    LiquidCrystal_I2C::begin();
    LiquidCrystal_I2C::backlight();
    LiquidCrystal_I2C::setCursor(0,0);
    LiquidCrystal_I2C::print(String(F("Arduino Flood")));
    LiquidCrystal_I2C::setCursor(0,1);
    LiquidCrystal_I2C::print(String(F("Warning System")));
    Serial.println(String(F("LCDManager initialization complete.")));
}
void LCDManager::printText(String attributeName, int value, int row){
    LiquidCrystal_I2C::setCursor(0,row);
    LiquidCrystal_I2C::print(attributeName + ":");
    LiquidCrystal_I2C::setCursor(attributeName.length()+1, row);
    LiquidCrystal_I2C::print(value);
    // Serial.println(attributeName + ": ");
    // Serial.print(value);
}
void LCDManager::printText(String attributeName, String value, int row){
    LiquidCrystal_I2C::setCursor(0,row);
    LiquidCrystal_I2C::print(attributeName + ":");
    LiquidCrystal_I2C::setCursor(attributeName.length()+1, row);
    LiquidCrystal_I2C::print(value);
    // Serial.println(attributeName + ": " + value);
    // Serial.print(value);
}

void LCDManager::printText(String value, int row){
    LiquidCrystal_I2C::setCursor(0,row);
    LiquidCrystal_I2C::print(value);
}

void LCDManager::printText(String value){
    if(value.length() > 16){
        LiquidCrystal_I2C::setCursor(0,0);
        LiquidCrystal_I2C::print(value.substring(0,16));
        LiquidCrystal_I2C::setCursor(0,1);
        LiquidCrystal_I2C::print(value.substring(16));
    }
    else{
        LiquidCrystal_I2C::setCursor(0,0);
        LiquidCrystal_I2C::print(value);
    }
    // Serial.println(value);
}

void LCDManager::clear(){
    LiquidCrystal_I2C::clear();
}