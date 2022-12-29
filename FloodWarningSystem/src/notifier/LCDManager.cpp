#include "LCDManager.h"

#define LCD_ADDRESS (uint8_t) 0x27
#define LCD_CHAR_COUNT (uint8_t) 16
#define LCD_ROW_COUNT (int) 2

LCDManager::LCDManager():
    LiquidCrystal_I2C(LCD_ADDRESS, LCD_CHAR_COUNT, LCD_ROW_COUNT){
    
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
    LiquidCrystal_I2C::print(attributeName + String(F(":")));
    LiquidCrystal_I2C::setCursor(attributeName.length()+1, row);
    LiquidCrystal_I2C::print(value);
    // Serial.println(attributeName + ": ");
    // Serial.print(value);
}
void LCDManager::printText(String attributeName, String value, int row){
    LiquidCrystal_I2C::setCursor(0,row);
    LiquidCrystal_I2C::print(attributeName + String(F(":")));
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