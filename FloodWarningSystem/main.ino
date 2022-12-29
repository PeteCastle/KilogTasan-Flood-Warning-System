#include "src/notifier/LCDManager.h"
#include "src/notifier/SIMManager.h"
#include "src/sensors/UltrasonicSensor.h"
#include "src/sensors/RainSensor.h"
#include "src/storage/SDManager.h"
#include "src/logger/Logger.h"
#include "src/storage/DateTimeManager.h"

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

#define SIM_TX_PIN (byte) 2
#define SIM_RX_PIN (byte) 3
#define ULTRASONIC_TRIG_PIN (byte) 4 
#define ULTRASONIC_ECHO_PIN (byte) 5 
#define SD_CS_PIN (byte) 10
#define SD_MOSI_PIN (byte) 11
#define SD_MISO_PIN (byte) 12
#define SD_SCK_PIN (byte) 13

//(Currently) Unimplemented Digital pins
#define SIM_RESET_PIN (byte) 6 // Not connected as there's no use yet
#define RTC_RESET_PIN (byte) 7
#define RTC_DATA_PIN (byte) 8
#define RTC_CLOCK_PIN (byte) 9 

//Analog pins

#define RAIN_SENSOR_PIN (uint8_t) A0 
#define LCD_SDA_PIN (uint8_t) A4
#define LCD_SCL_PIN (uint8_t) A5

//Hardware Configs
#define rain_samples (byte) 4 

//Environmental Configs
#define RIVER_DEPTH (int) 100

// Default Configs
char const DEFAULT_DATE[] PROGMEM  = "Jan 10 2010";
char const DEFAULT_TIME[] PROGMEM = "10:00:00";

LCDManager lcd;
SIMManager sim(SIM_RX_PIN, SIM_TX_PIN, SIM_RESET_PIN);
RainSensor rainSensor(RAIN_SENSOR_PIN, rain_samples);
UltrasonicSensor ultrasonicSensor(ULTRASONIC_TRIG_PIN, ULTRASONIC_ECHO_PIN, RIVER_DEPTH);
SDManager sd(SD_CS_PIN);
Logger logger(&lcd, &sd);
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


    String lastMsgDate = sd.readFile(String(F("lastmsgd.txt")));
    String lastMsgTime = sd.readFile(String(F("lastmsgt.txt")));
    timeSinceLastWarning = RtcDateTime((char*) lastMsgDate.c_str(),(char*) lastMsgTime.c_str());

    while(!sim.getSIMConnectivityStatus()){
        lcd.printText(String(F("SIM: Establishing connection...")));
    }


    // String test = String(F("test"));
    // sim.sendSms("+639297728087", test);
    // sim.sendSms("+639369322603", test);
    // sim.sendSms("+639990368778", test);
    // sim.sendSms("+639959509368", test);
    // sim.sendSms("+639065067465", test);


    //sim.sendHttpRequest(CURRENT_DATE_TIME);
}

String getAlertLevelName(byte currentWarning){
    switch(currentWarning){
        case 0: return String(F(" No Alert")); break;
        case 1: return String(F(" Yellow Alert")); break;
        case 2: return String(F(" Orange Alert")); break;
        case 3: return String(F(" Red Alert")); break;
        default: return String(F("None"));
    }
}


void loop(){
    String currentTime = datetime.getCurrentDateTimeString();
    byte currentRainLevel = rainSensor.getSampledValue();
    int currentRiverLevel = ultrasonicSensor.getDistance();
    
    byte currentRainWarning = rainSensor.getWarningLevel(currentRainLevel);
    byte currentLevelWarning = ultrasonicSensor.getWarningLevel(currentRiverLevel);
 
    logger.measureLog(currentTime, currentRainLevel, currentRiverLevel);

    if (currentRainWarning > 0 | currentLevelWarning > 0){
        String warningMessage;
        warningMessage[0] = '\0';
        String RIVER_NAME  = String(F("Itlog ni Cortez"));
        String standardMessage1 = String(F("[RIVER WARNING] "));
        String standardMessage2 = String(F("[RAIN WARNING] "));

        //If rain only
        if(currentRainWarning > 0 && currentLevelWarning == 0){ 
            warningMessage+=standardMessage2;
            warningMessage+=currentTime;
            warningMessage+=getAlertLevelName(currentRainWarning);
            String alertLevelRainOnly = String(F(" ang pag-ulan malapit sa "));
            warningMessage+=alertLevelRainOnly;
            String rainWarning  = String(F(".Asahan ang pag-ulan sa loob ng ilang oras."));
            warningMessage+=RIVER_NAME;
            warningMessage+=rainWarning;
        }
        //If river level only
        else if(currentLevelWarning > 0 && currentRainWarning == 0){
            warningMessage+=standardMessage1;
            warningMessage+=currentTime;
            warningMessage+=getAlertLevelName(currentLevelWarning);
            warningMessage+=String(F(" sa "));
            warningMessage+=RIVER_NAME;

            switch(currentLevelWarning){
                case 1: warningMessage+=String(F(". Mag-ingat sa posibilidad ng pagbaha sa loob ng ilang oras.")); break;
                case 2: warningMessage+=String(F(". Inaanyayahan ang lahat na lumikas sa evacuation center.")); break;
                case 3: warningMessage+=String(F(". Inuutusan ang lahat na lumikas sa evacuation center.")); break;
                default: break;
            }
        }
        else{
            byte higherLevel = currentRainWarning>=currentLevelWarning ? currentRainWarning : currentLevelWarning;
            warningMessage+=standardMessage1;
            warningMessage+=currentTime;
            warningMessage+=getAlertLevelName(higherLevel);
            warningMessage+=String(F(" ngayon ang pag-ulan at ang kalagayan sa "));
            warningMessage+=RIVER_NAME;
            switch(currentLevelWarning){
                case 1: warningMessage+=String(F(". Mag-ingat sa posibilidad ng pagbaha at pagulan sa loob ng ilang oras.")); break;
                case 2: warningMessage+=String(F(". Inaanyayahan ang lahat na lumikas sa pinakamalapit na evacuation center.")); break;
                case 3: warningMessage+=String(F(". Inuutusan ang lahat na lumikas sa pinakamalapit na evacuation center.")); break;
                default: break;
            }
        }
        sim.sendMessageToAllRecipients(&warningMessage, &sd);
        Serial.println(warningMessage);

        sd.writeFileReplace(String(F("lastmsgd.txt")),datetime.getCurrentDate());
        sd.writeFileReplace(String(F("lastmsgt.txt")),datetime.getCurrentTime());

        timeSinceLastWarning = datetime.getCurrentDateTime();
    }
    delay(20000);
}