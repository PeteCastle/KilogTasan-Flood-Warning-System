#include "Logger.h"

Logger::Logger(LCDManager *lcd, SDManager *sd, String RECIPIENTS_FILE, String OPERATIONS_FILE, String MEASUREMENTS_FILE):
    _OPERATIONS_FILE(OPERATIONS_FILE),
    _RECIPIENTS_FILE(RECIPIENTS_FILE),
    _MEASUREMENTS_FILE(MEASUREMENTS_FILE){
    this->lcd = lcd;
    this->sd = sd;
}

void Logger::standardLog(String log){
    Serial.println(log);
    sd->writeFile(_OPERATIONS_FILE,log);
    lcd->printText(log);
}

void Logger::measureLog(String time, int rainLevel, int riverLevel){  // The string should already be in CSV
    Serial.println(time + "\t" + String(rainLevel) + "\t" + String(riverLevel));

    sd->writeFile(_MEASUREMENTS_FILE,time + rainLevel + String(F(",")) + riverLevel );

    // sd->writeFile(F("teest.txt"),F("HELLO WORLD PLEASE GUMANA KA NA"));

    lcd->clear();

    lcd->printText(time,0);
    lcd->printText(String(F("RAI:")) + String(rainLevel) + String(F(" RIV: ")) + String(riverLevel), 1);

    // lcd.printText(String(F("Rain Level")), String(currentRainLevel), 0);
    // lcd.printText(String(F("River Level")), String(currentRiverLevel), 1);

}