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
// Try to deprecate VECTOR and use methods when sending to multiple recipientss
// Use API to get time.

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
#define SIM_RESET_PIN (int) 2 //const int SIM_RESET_PIN= 2;
#define SD_CS_PIN (int) 3 //const int SDC_CS_PIN= 3;
#define ULTRASONIC_TRIG_PIN (int) 4 //const int ULTRASONIC_TRIG_PIN= 4;
#define ULTRASONIC_ECHO_PIN (int) 5 //const int ULTRASONIC_ECHO_PIN= 5;
#define SIM_RX_PIN (int) 6 //const int SIM_RX_PIN= 6;
#define SIM_TX_PIN (int) 7  //const int SIM_TX_PIN= 7;
#define SD_SDO_PIN (int) 11 //const int SD_SDO_PIN= 11;
#define SD_SDI_PIN (int) 12  //const int SD_SDI_PIN= 12;
#define SD_CLK_PIN (int) 13 //const int SD_CLK_PIN= 13;

//Analog pins
#define LCD_ADDRESS (uint8_t) 0x27 //const uint8_t LCD_ADDRESS= 0x27;
#define RAIN_SENSOR_PIN (uint8_t) A0 //const uint8_t RAIN_SENSOR_PIN= A0;
#define LCD_SDA_PIN (uint8_t) A4
#define LCD_SCL_PIN (uint8_t) A5


//Hardware Configs
#define LCD_CHAR_COUNT (int) 16 //const int LCD_CHAR_COUNT= 16;
#define LCD_ROW_COUNT (int) 2 //const int LCD_ROW_COUNT= 2;
#define rain_samples (byte) 4 //const byte rain_samples= 4;
#define RAIN_SENSITIVITY (int) 16 //const byte RAIN_SENSITIVITY= 16;

//Environmental Configs
#define RIVER_DEPTH (int) 100 //const int RIVER_DEPTH= 100; // in centimeters
// #define RIVER_NAME (String) "Kiko River" 
const String RIVER_NAME = "Kiko River"; 
#define YELLOW_RAIN_THRESHOLD (byte) 40 //const byte YELLOW_RAIN_THRESHOLD= 40; // Will be changed later.
#define ORANGE_RAIN_THRESHOLD (byte) 40 //const byte ORANGE_RAIN_THRESHOLD= 40;
#define RED_RAIN_THRESHOLD (byte) 40 //const byte RED_RAIN_THRESHOLD= 40;
#define YELLOW_LEVEL_THRESHOLD (int) 4 //const int YELLOW_LEVEL_THRESHOLD= 4; //Threshold for water level is measured by the distance from the ultrasonic sensor to the current water level
#define ORANGE_LEVEL_THRESHOLD (int) 2.5 //const int ORANGE_LEVEL_THRESHOLD= 2.5;
#define RED_LEVEL_THRESHOLD (int) 1 //const int RED_LEVEL_THRESHOLD= 1;
typedef struct {
    uint8_t lang;
    String name;
} WarningDictionary;
const WarningDictionary levelDict[] = {
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
SDManager sd(SD_CS_PIN);

void setup(){
    Serial.begin(9600);
    lcd.begin();
    sim.begin();
    rainSensor.begin();
    ultrasonicSensor.begin();
    // sd.begin();
}


// CORE WARNING SYSTEM
// SAVE TO DATABSE

void loop(){
    int currentRainLevel = rainSensor.getSampledValue();
    int currentRiverLevel = ultrasonicSensor.getRiverLevel();
    
    int currentRainWarning = rainSensor.getWarningLevel(currentRainLevel);
    int currentLevelWarning = ultrasonicSensor.getWarningLevel(currentRiverLevel);

    lcd.printText(String(F("Rain Level")), String(currentRainLevel), 0);
    lcd.printText(String(F("River Level")), String(currentRiverLevel), 1);

    //Serial.println("TEST");

    // if (currentRainWarning > 0 | currentLevelWarning > 0){
    //     String message1 = String(F("[RIVER WARNING SYSTEM] "));
    //     String message2 = String(F("DATETIMEHERE"));
    //     String message3 = String(F(" rainfall warning ang inaasahan malapit sa "));
    //     String message4 = String(F(". Asahan ang pag-ulan sa loob ng ilang oras."));
    //     String message5 = String(F(" ang kalagayan sa " ));
    //     String message6 = String(F(". Mag-ingat sa posibilidad ng pagbaha sa loob ng ilang oras."));
    //     String message7 = String(F("Mag-ingat sa posibilidad ng pagbaha at pagulan sa loob ng ilang oras."));
    //     String message8 = String(F("Inaanyayahan ang lahat na lumikas sa pinakamalapit na evacuation center."));
    //     String message9 = String(F("Inuutusan ang lahat na lumikas sa pinakamalapit na evacuation center."));
    //     String message10 = String(F("."));

    //     if(currentRainWarning > 0 && currentLevelWarning == 0){
    //         String message = message1 + message2 + levelDict[currentRainWarning].name + message3, RIVER_NAME, message4;
    //         sim.sendSmsMultipleRecipients(message, sd.readFile(RECIPIENTS_FILE));
    //     }
    //     else if (currentLevelWarning > 0 && currentRainWarning == 0){
    //         String message = message1 + message2 + levelDict[currentLevelWarning].name + 
    //             message5 + RIVER_NAME + message6;
    //         sim.sendSmsMultipleRecipients(message, sd.readFile(RECIPIENTS_FILE));
    //     }
    //     else{ // If both have warning levels
    //         // Selects whichever is higher
    //         int highestLevel = currentLevelWarning > currentRainWarning ? highestLevel = currentLevelWarning : highestLevel = currentRainWarning;

    //         String message = message1 + message2 + levelDict[highestLevel].name + message5 + RIVER_NAME + message10;
                
    //         if (highestLevel == 1 ) message += message7;
    //         else if (highestLevel == 2 ) message += message8;
    //         else if (highestLevel == 3 ) message += message9;

    //         sim.sendSmsMultipleRecipients(message, sd.readFile(RECIPIENTS_FILE));
    //     }
    // }

    delay(1000);
}