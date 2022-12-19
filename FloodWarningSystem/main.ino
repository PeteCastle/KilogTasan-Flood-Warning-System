//#include "ConfigManager.h"
#include "src/notifier/LCDManager.h"
#include "src/notifier/SIMManager.h"
#include "src/sensors/UltrasonicSensor.h"
#include "src/sensors/RainSensor.h"
#include "src/storage/SDManager.h"

// References
// https://github.com/arduino/ArduinoCore-avr/blob/master/cores/arduino/Arduino.h
// https://github.com/gamegine/HCSR04-ultrasonic-sensor-lib/blob/master/src/HCSR04.h
// https://github.com/naoto64/Arduino-RainSense/blob/master/src/RainSense.h
// https://github.com/PaulStoffregen/SoftwareSerial/blob/master/SoftwareSerial.cpp
// https://github.com/cristiansteib/Sim800l/blob/master/Sim800l.cpp
// https://github.com/arduino-libraries/SD

// TODO :
// Integration of sim numbers from API to the memory card.
// In Ultrasonic Sensor.cpp
    // Return water level when measured distance is beyond the scope of the ultrasonic sensor.
    // The ultrasonic sensor is not yet tested when distance is greater than 4m
// Multiple warning alerts tier

// REMINDERS:
    // SoftwareSerial instantiation is done before begin itself.  May not workin since its not placed in begin()
    // Same goes with LiquidCrystal I2C
    // LiquidCrystal_I2C "init" changed to "begin" contradictory to previously tested method.
    // SDManager::readFile still does not save to the vector.  Temporarily replaced with default Serial.write

//CONFIGURATION FILES
//Digital Pins
const int SIM_RESET_PIN= 2;
const int SDC_CS_PIN= 3;
const int ULTRASONIC_TRIG_PIN= 4;
const int ULTRASONIC_ECHO_PIN= 5;
const int SIM_RX_PIN= 6;
const int SIM_TX_PIN= 7;
const int SD_SDO_PIN= 11;
const int SD_SDI_PIN= 12;
const int SD_CLK_PIN= 13;

//Analog pins
const uint8_t LCD_ADDRESS= 0x27;
const uint8_t RAIN_SENSOR_PIN= A0;

//Hardware Configs
const int LCD_CHAR_COUNT= 16;
const int LCD_ROW_COUNT= 2;
byte rain_samples= 4;
const byte RAIN_SENSITIVITY= 16;

//Environmental Configs
const int RIVER_DEPTH= 100; // in centimeters
const byte YELLOW_RAIN_THRESHOLD= 40; // Will be changed later.
const byte ORANGE_RAIN_THRESHOLD= 40;
const byte RED_RAIN_THRESHOLD= 40;
const int YELLOW_LEVEL_THRESHOLD= 4; //Threshold for water level is measured by the distance from the ultrasonic sensor to the current water level
const int ORANGE_LEVEL_THRESHOLD= 2.5;
const int RED_LEVEL_THRESHOLD= 1;

LCDManager lcd(LCD_ADDRESS, LCD_CHAR_COUNT, LCD_ROW_COUNT);
SIMManager sim(SIM_RX_PIN, SIM_TX_PIN, SIM_RESET_PIN);
RainSensor rainSensor(RAIN_SENSOR_PIN, rain_samples, RAIN_SENSITIVITY, YELLOW_RAIN_THRESHOLD, ORANGE_RAIN_THRESHOLD, RED_RAIN_THRESHOLD);
UltrasonicSensor ultrasonicSensor(ULTRASONIC_TRIG_PIN, ULTRASONIC_ECHO_PIN, RIVER_DEPTH, YELLOW_LEVEL_THRESHOLD, ORANGE_LEVEL_THRESHOLD, RED_LEVEL_THRESHOLD);
SDManager sd(SDC_CS_PIN);

void setup(){
    Serial.begin(9600);
    lcd.begin();
    sim.begin();
    rainSensor.begin();
    ultrasonicSensor.begin();
    sd.begin();
}

void loop(){
    Serial.println("Hello World");
    delay(1000);
}