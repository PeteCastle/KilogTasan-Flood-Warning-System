 #include "SDManager.h"

 SDManager::SDManager(byte SD_CS_PIN):
    _SC_CS_PIN(SD_CS_PIN){
    }

bool SDManager::begin(){
    if (!SD.begin(_SC_CS_PIN)) {
       // Serial.println(String(F("Initialization of sd card failed. SDManager.cpp:9")));
        // Serial.println("1. is a card inserted?");
        // Serial.println("2. is your wiring correct?");
        // Serial.println("3. did you change the chipSelect pin to match your shield or module?");
        // Serial.println("Note: press reset button on the board and reopen this Serial Monitor after fixing your issue!");
        return false;
    }
    Serial.println(String(F("SD initialization done.")));
    return true;
}

void SDManager::writeFile(const String filePath, String value){
    File dataFile = SD.open(filePath, FILE_WRITE);
    if(dataFile){
        dataFile.println(value);
        dataFile.close();
        Serial.print(F("A text was added to the file:  "));
        Serial.println(filePath);
    }
    else{
        Serial.print(F("An error has occured while opening file: "));
        Serial.println(filePath);
    }
}

void SDManager::writeFileReplace(const String filePath, String value){
    removeFile(filePath);
    File dataFile = SD.open(filePath, FILE_WRITE);
    if(dataFile){
        dataFile.println(value);
        dataFile.close();
        Serial.print(F("A text was added to the file:  "));
        Serial.println(filePath);
    }
    else{
        Serial.print(F("An error has occured while opening file: "));
        Serial.println(filePath);
    }
}

String SDManager::readFile(String filePath){
    File dataFile = SD.open(filePath);

    String tempString;
    if(dataFile){
        while(dataFile.available()){
            //strcat(tempString,(char) dataFile.read() );
            tempString += (char) dataFile.read();
        }
        dataFile.close();

        Serial.println(String(F("Read contents of file ")) + filePath + String(F(" complete...")));
        return tempString;
    }
    else{
        Serial.print(F("An error has occured while opening file: "));
        Serial.println(filePath);
    }
    return String(F("Null"));
}

void SDManager::removeFile(const String filePath){
    SD.remove(filePath);
    Serial.print(F("Removed file: "));
    Serial.println(filePath);
}



