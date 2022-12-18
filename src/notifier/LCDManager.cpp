#include "LCDManager.h"

LCDManager::LCDManager(uint8_t  address,  uint8_t colCount = 16, uint8_t rowCount=2) : LiquidCrystal_I2C_I2C(address, colCount, rowCount) {
    LiquidCrystal_I2C::init();
    LiquidCrystal_I2C::backlight();
    LiquidCrystal_I2C::setCursor(0,0);
    LiquidCrystal_I2C::print("Arduino Flood");
    LiquidCrystal_I2C::setCursor(0,1);
    LiquidCrystal_I2C::print("Warning System");
    Serial.println("LCDManager initialization complete.");
}
LCDManager::printText(String attributeName, int value, int row){
    LiquidCrystal_I2C::setCursor(0,row);
    LiquidCrystal_I2C::print(attributeName + ":");
    LiquidCrystal_I2C::setCursor(attributeName.length()+1, row);
    LiquidCrystal_I2C::print(value);
    Serial.println(attributeName + ": ");
    Serial.print(value);
}
LCDManager::printText(String attributeName, String value, int row){
    LiquidCrystal_I2C::setCursor(0,row);
    LiquidCrystal_I2C::print(attributeName + ":");
    LiquidCrystal_I2C::setCursor(attributeName.length()+1, row);
    LiquidCrystal_I2C::print(value);
    Serial.println(attributeName + ": " + value);
    Serial.print(value);
}
LCDManager::printText(String value){
    if(value.length() > 16)[
        LiquidCrystal_I2C::setCursor(0,0);
        LiquidCrystal_I2C::print(value.substring(0,16));
        LiquidCrystal_I2C::setCursor(0,1);
        LiquidCrystal_I2C::print(value.substring(16));
    ]
    else{
        LiquidCrystal_I2C::setCursor(0,0);
        LiquidCrystal_I2C::print(value);
    }
    Serial.println(value);
}