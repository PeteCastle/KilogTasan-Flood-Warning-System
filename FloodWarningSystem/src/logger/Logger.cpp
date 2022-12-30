#include "Logger.h"

Logger::Logger(LCDManager *lcd, SDManager *sd){
    this->lcd = lcd;
    this->sd = sd;
}

void Logger::standardLog(String log){
    Serial.println(log);
    sd->writeFile(String(F("log.log")),log);
    lcd->printText(log);
}

void Logger::measureLog(String time, int rainLevel, int riverLevel){  // The string should already be in CSV
    sd->writeFile(String(F("measures.csv")),time + rainLevel + String(F(",")) + rainLevel + String(F(",")) + riverLevel);
    lcd->clear();
    lcd->printText(time,0);
    lcd->printText(String(F("RAI:")) + String(rainLevel) + String(F(" RIV: ")) + String(riverLevel), 1);
}