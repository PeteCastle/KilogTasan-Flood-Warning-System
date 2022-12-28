#include "src/notifier/LCDManager.h"
#include "src/notifier/SIMManager.h"
#include "src/sensors/UltrasonicSensor.h"
#include "src/sensors/RainSensor.h"
#include "src/storage/SDManager.h"
#include "src/logger/Logger.h"
#include "src/storage/DateTimeManager.h"
#include <avr/pgmspace.h>

// References

// https://github.com/arduino/ArduinoCore-avr/blob/master/cores/arduino/Arduino.h
// https://github.com/gamegine/HCSR04-ultrasonic-sensor-lib/blob/master/src/HCSR04.h
// https://github.com/naoto64/Arduino-RainSense/blob/master/src/RainSense.h
// https://github.com/PaulStoffregen/SoftwareSerial/blob/master/SoftwareSerial.cpp
// https://github.com/cristiansteib/Sim800l/blob/master/Sim800l.cpp
// https://github.com/arduino-libraries/SD
// https://github.com/Makuna/Rtc/blob/master/extras/RtcTemperatureTests/RtcTemperatureTests.ino

// TODO :
// Integration of sim numbers from API to the memory card.
// In Ultrasonic Sensor.cpp
    // Return water level when measured distance is beyond the scope of the ultrasonic sensor.
    // The ultrasonic sensor is not yet tested when distance is greater than 4m
// Try to deprecate VECTOR and use methods when sending to multiple recipientss
// Use API to get time.

// FOR FUTURE UPDATES:
    // Live website status
    // Status via SMS manually by ppl.

//CONFIGURATION FILES
//Digital Pins

#define SIM_TX_PIN (int) 2
#define SIM_RX_PIN (int) 3
#define ULTRASONIC_TRIG_PIN (int) 4 
#define ULTRASONIC_ECHO_PIN (int) 5 
#define SD_CS_PIN (int) 10
#define SD_MOSI_PIN (int) 11
#define SD_MISO_PIN (int) 12
#define SD_SCK_PIN (int) 13

//(Currently) Unimplemented Digital pins
#define SIM_RESET_PIN (int) 6 // Not connected as there's no use yet
#define RTC_RESET_PIN (int) 7
#define RTC_DATA_PIN (int) 8
#define RTC_CLOCK_PIN (int) 9 

//Analog pins
#define LCD_ADDRESS (uint8_t) 0x27
#define RAIN_SENSOR_PIN (uint8_t) A0 
#define LCD_SDA_PIN (uint8_t) A4
#define LCD_SCL_PIN (uint8_t) A5

//Hardware Configs
#define LCD_CHAR_COUNT (int) 16
#define LCD_ROW_COUNT (int) 2
#define rain_samples (byte) 4 
#define RAIN_SENSITIVITY (int) 16

//Environmental Configs
#define RIVER_DEPTH (int) 100
//#define RIVER_NAME (String) "Kiko River"
char const RIVER_NAME[] PROGMEM = "Kiko River";
#define YELLOW_RAIN_THRESHOLD (byte) 80 
#define ORANGE_RAIN_THRESHOLD (byte) 90 
#define RED_RAIN_THRESHOLD (byte) 100 
#define YELLOW_LEVEL_THRESHOLD (int) 300 //Threshold for water level is measured by the distance from the ultrasonic sensor to the current water level
#define ORANGE_LEVEL_THRESHOLD (int) 200
#define RED_LEVEL_THRESHOLD (int) 100


//File Configs
#define RECIPIENTS_FILE (String) "sendlist.txt"
#define OPERATIONS_FILE (String) "log.log"
#define MEASUREMENTS_FILE (String) "measures.csv"
#define LAST_MESSAGE_FILE_DATE (String) "lastmsgd.txt"
#define LAST_MESSAGE_FILE_TIME (String) "lastmsgt.txt"

// Default Configs
char const DEFAULT_DATE[] PROGMEM  = "Jan 10 2010";
char const DEFAULT_TIME[] PROGMEM = "10:00:00";

char const ALERT_LEVEL_0[] PROGMEM = "No Alert Level";
char const ALERT_LEVEL_1[] PROGMEM = "Yellow Alert Level";
char const ALERT_LEVEL_2[] PROGMEM = "Orange Alert Level";
char const ALERT_LEVEL_3[] PROGMEM = "Red Alert Level";

char const standardMessage1[] PROGMEM = "[RIVER WARNING SYSTEM] ";
char const standardMessage2[] PROGMEM = "[RAIN WARNING SYSTEM] ";

char const alertLevelRainOnly[] PROGMEM = " ngayon ang pag-ulan malapit sa .";
char const alertLevelFloodOnly[] PROGMEM = " ngayon ang kalagayan sa .";
char const alertLevelBoth[] PROGMEM = " ngayon ang pag-ulan at ang kalagayan sa "; //Must choose whoever is higher in the two

char const rainWarning[] PROGMEM = "Asahan ang pag-ulan sa loob ng ilang oras.";
char const floodWarning[] PROGMEM = "Mag-ingat sa posibilidad ng pagbaha sa loob ng ilang oras.";
char const bothWarning[] PROGMEM = "Mag-ingat sa posibilidad ng pagbaha at pagulan sa loob ng ilang oras.";

char const evacuationOptional[] PROGMEM = "Inaanyayahan ang lahat na lumikas sa pinakamalapit na evacuation center.";
char const evacuationMandatory[] PROGMEM = "Inuutusan ang lahat na lumikas sa pinakamalapit na evacuation center.";



LCDManager lcd(LCD_ADDRESS, LCD_CHAR_COUNT, LCD_ROW_COUNT);
SIMManager sim(SIM_RX_PIN, SIM_TX_PIN, SIM_RESET_PIN);
RainSensor rainSensor(RAIN_SENSOR_PIN, rain_samples, RAIN_SENSITIVITY, YELLOW_RAIN_THRESHOLD, ORANGE_RAIN_THRESHOLD, RED_RAIN_THRESHOLD);
UltrasonicSensor ultrasonicSensor(ULTRASONIC_TRIG_PIN, ULTRASONIC_ECHO_PIN, RIVER_DEPTH, YELLOW_LEVEL_THRESHOLD, ORANGE_LEVEL_THRESHOLD, RED_LEVEL_THRESHOLD);
SDManager sd(SD_CS_PIN);
Logger logger(&lcd, &sd, RECIPIENTS_FILE, OPERATIONS_FILE, MEASUREMENTS_FILE);
DateTimeManager datetime(RTC_DATA_PIN, RTC_CLOCK_PIN, RTC_RESET_PIN);
RtcDateTime timeSinceLastWarning(DEFAULT_DATE,DEFAULT_TIME); //Initialize temporary value

void setup(){
    logger.standardLog(String(F("Starting Flood Warning System")));
    
    Serial.begin(9600);
    lcd.begin();

    lcd.printText(String(F("Setting up SD card")));
    while(!sd.begin()) lcd.printText(String(F("SD card init failed. Try again")));


    lcd.printText(String(F("Setting up rainsensor...")));
    rainSensor.begin();

    logger.standardLog(String(F("Setting up ultrasonic sensor")));
    ultrasonicSensor.begin();
    
    logger.standardLog(String(F("Setting up SIM card...")));
    sim.begin();

    logger.standardLog(String(F("Setting up date and time modules..")));
    datetime.begin();


    String lastMsgDate = sd.readFile(LAST_MESSAGE_FILE_DATE);
    String lastMsgTime = sd.readFile(LAST_MESSAGE_FILE_TIME);
    timeSinceLastWarning = RtcDateTime((char*) lastMsgDate.c_str(),(char*) lastMsgTime.c_str());

    // while(!sim.getSIMConnectivityStatus()){
    //     lcd.printText(String(F("SIM: Establishing connection...")));
    // }



    //sim.sendSms("+639369322603", "HELLO WORLD");
    //sim.sendSms("+639990368778", "HELLO WORLD");


    //sim.sendHttpRequest(CURRENT_DATE_TIME);
}

//FOR DEBUGGING ONLY
extern unsigned int __heap_start;
extern void *__brkval;
struct __freelist {
  size_t sz;
  struct __freelist *nx;
};
extern struct __freelist *__flp;
int freeListSize() {
  struct __freelist* current;
  int total = 0;
  for (current = __flp; current; current = current->nx) {
    total += 2; /* Add two bytes for the memory block's header  */
    total += (int) current->sz;
  }
  return total;
}

int freeMemory() {
  int free_memory;
  if ((int)__brkval == 0) {
    free_memory = ((int)&free_memory) - ((int)&__heap_start);
  } else {
    free_memory = ((int)&free_memory) - ((int)__brkval);
    free_memory += freeListSize();
  }
  return free_memory;
}




void loop(){
    Serial.println(F("START OF LOOP"));
    String currentTime = datetime.getCurrentTime();
 

    int currentRainLevel = rainSensor.getSampledValue();
    int currentRiverLevel = ultrasonicSensor.getDistance();
    
    int currentRainWarning = rainSensor.getWarningLevel(currentRainLevel);
    int currentLevelWarning = ultrasonicSensor.getWarningLevel(currentRiverLevel);

    Serial.print(F("FREE MEMORY: "));
    Serial.println(freeMemory());
    logger.measureLog(currentTime, currentRainLevel, currentRiverLevel);

    if (currentRainWarning > 0 | currentLevelWarning > 0){
        Serial.print(F("FREE MEMORY: "));
        Serial.println(freeMemory());
        char *warningMessage;
        //String warningMessage;
        //char *buffer;
        warningMessage[0] = '\0';

        //If rain only
        if(currentRainWarning > 0 && currentLevelWarning == 0){ 
            Serial.println(F("1ST MESSAGE:"));
            // warningMessage+=standardMessage2;
            // warningMessage+=currentTime;
            // warningMessage+=getAlertLevelName(currentRainWarning);
            // warningMessage+=alertLevelRainOnly;
            // warningMessage+=RIVER_NAME;
            // warningMessage+=rainWarning;
        }
        //If river level only
        else if(currentLevelWarning > 0 && currentRainWarning == 0){
            Serial.println(F("2ND MESSAGE:"));
            warningMessage = strcat_P(warningMessage, standardMessage1);
            warningMessage = strcat(warningMessage, currentTime.c_str());


            // switch(currentLevelWarning){
            //     case 0: warningMessage = strcat_P(warningMessage,ALERT_LEVEL_0); break;
            //     case 1: warningMessage = strcat_P(warningMessage,ALERT_LEVEL_1); break;
            //     case 2: warningMessage = strcat_P(warningMessage,ALERT_LEVEL_2); break;
            //     case 3: warningMessage = strcat_P(warningMessage,ALERT_LEVEL_3); break;
            //     default: break;
            // }
            warningMessage = strcat_P(warningMessage, alertLevelFloodOnly);
            warningMessage = strcat_P(warningMessage, RIVER_NAME);
            switch(currentLevelWarning){
                case 1: warningMessage = strcat_P(warningMessage, floodWarning); break;
                case 2: warningMessage = strcat_P(warningMessage, evacuationOptional); break;
                case 3: warningMessage = strcat_P(warningMessage, evacuationMandatory); break;
                default: break;
            }
        }
        // If both
        else{
            // Serial.println(F("3RD MESSAGE:"));
            // int higherLevel = currentRainWarning>=currentLevelWarning ? currentRainWarning : currentLevelWarning;
            // warningMessage+=standardMessage1;
            // warningMessage+=currentTime;
            // warningMessage+=getAlertLevelName(higherLevel);
            // warningMessage+=alertLevelBoth;
            // warningMessage+=RIVER_NAME;
            // switch(currentLevelWarning){
            //     case 1: warningMessage+=bothWarning; break;
            //     case 2: warningMessage+=evacuationOptional; break;
            //     case 3: warningMessage+=evacuationMandatory; break;
            //     default: break;
            // }
        }
        Serial.println(F("WARNING MESSAGE:"));
        Serial.println(warningMessage);
        Serial.println(F("WARNING MESSAGE END"));
    }
    delay(1000);
}



 // }
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