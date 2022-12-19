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
    // SIM.print() in in sending messages: Data type changed to String instead of char.

// FOR FUTURE UPDATES:
    // Live website status
    // Status via SMS manually by ppl.

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
const String RIVER_NAME = "Kiko River"; 
const byte YELLOW_RAIN_THRESHOLD= 40; // Will be changed later.
const byte ORANGE_RAIN_THRESHOLD= 40;
const byte RED_RAIN_THRESHOLD= 40;
const int YELLOW_LEVEL_THRESHOLD= 4; //Threshold for water level is measured by the distance from the ultrasonic sensor to the current water level
const int ORANGE_LEVEL_THRESHOLD= 2.5;
const int RED_LEVEL_THRESHOLD= 1;
typedef struct {
    uint8_t lang;
    String name;
} WarningDictionary;
const WarningDictionary levelDict[] {
    {0, String("None")},
    {1, String("Yellow")},
    {2, String("Orange")},
    {3, String("Red")}
};

//File Configs
const String RECIPIENTS_FILE = "recipients.txt";

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


// CORE WARNING SYSTEM
// SAVE TO DATABSE

void loop(){
    int currentRainLevel = rainSensor.getSampledValue();
    int currentRiverLevel = ultrasonicSensor.getRiverLevel();
    
    int currentRainWarning = rainSensor.getWarningLevel(currentRainLevel);
    int currentLevelWarning = ultrasonicSensor.getWarningLevel(currentRiverLevel);

    if (currentRainWarning > 0 | currentLevelWarning > 0){
        String message1 = String(F("[RIVER WARNING SYSTEM] "));
        String message2 = String(F("DATETIMEHERE"));
        String message3 = String(F(" rainfall warning ang inaasahan malapit sa "));
        String message4 = String(F(". Asahan ang pag-ulan sa loob ng ilang oras."));
        String message5 = String(F(" ang kalagayan sa " ));
        String message6 = String(F(". Mag-ingat sa posibilidad ng pagbaha sa loob ng ilang oras."));
        String message7 = String(F("Mag-ingat sa posibilidad ng pagbaha at pagulan sa loob ng ilang oras."));
        String message8 = String(F("Inaanyayahan ang lahat na lumikas sa pinakamalapit na evacuation center."));
        String message9 = String(F("Inuutusan ang lahat na lumikas sa pinakamalapit na evacuation center."));
        String message10 = String(F("."));

        if(currentRainWarning > 0 && currentLevelWarning == 0){
            String message = message1 + message2 + levelDict[currentRainWarning].name + message3, RIVER_NAME, message4;
            sim.sendSmsMultipleRecipients(message, sd.readFile(RECIPIENTS_FILE));
        }
        else if (currentLevelWarning > 0 && currentRainWarning == 0){
            String message = message1 + message2 + levelDict[currentLevelWarning].name + 
                message5 + RIVER_NAME + message6;
            sim.sendSmsMultipleRecipients(message, sd.readFile(RECIPIENTS_FILE));
        }
        else{ // If both have warning levels
            // Selects whichever is higher
            int highestLevel = currentLevelWarning > currentRainWarning ? highestLevel = currentLevelWarning : highestLevel = currentRainWarning;

            String message = message1 + message2 + levelDict[highestLevel].name + message5 + RIVER_NAME + message10;
                
            if (highestLevel == 1 ) message += message7;
            else if (highestLevel == 2 ) message += message8;
            else if (highestLevel == 3 ) message += message9;

            sim.sendSmsMultipleRecipients(message, sd.readFile(RECIPIENTS_FILE));
        }
    }

    // Serial.println("Hello World");
    delay(10000);
}