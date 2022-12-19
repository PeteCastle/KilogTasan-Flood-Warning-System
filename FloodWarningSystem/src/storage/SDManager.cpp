 #include "SDManager.h"

 SDManager::SDManager(int SD_CS_PIN):
    _SC_CS_PIN(SD_CS_PIN){
    }

void SDManager::begin(){
    Serial.print(String(F("Initializing SD card...")));
    if (!SD.begin(_SC_CS_PIN)) {
        Serial.println(String(F("Initialization of sd card failed. SDManager.cpp:9")));
        // Serial.println("1. is a card inserted?");
        // Serial.println("2. is your wiring correct?");
        // Serial.println("3. did you change the chipSelect pin to match your shield or module?");
        // Serial.println("Note: press reset button on the board and reopen this Serial Monitor after fixing your issue!");
        while (true);
    }
    Serial.println(String(F("initialization done.")));
}

void SDManager::writeFile(const String filePath, String value){
    File dataFile = SD.open(filePath, FILE_WRITE);

    if(dataFile){
        dataFile.println(value);
        dataFile.close();
        Serial.println(String(F("The following contents were added to the file ")) + filePath + ": \n" + value);
    }
    else{
        Serial.println(String(F("An error has occured while opening file: ")) + filePath);
    }
}
void SDManager::writeFile(String filePath, Vector<String> values){
    File dataFile = SD.open(filePath, FILE_WRITE);

    if(dataFile){
        for( String value : values) dataFile.println(value + "\n");
        
        Serial.println(String(F("The following contents were added to the file ")) + filePath);
        dataFile.close();
    }
    else{
        Serial.println(String(F("An error has occured while opening file: ")) + filePath);
    }
}

Vector<String> SDManager::readFile(String filePath){
    File dataFile = SD.open(filePath);

    String storage_array[1];
    Vector<String> vector(storage_array);

    char *tempString;
    const char* delimiter = "\n";

    if(dataFile){
        while(dataFile.available()){
            tempString += (char) dataFile.read();
            //Serial.write(dataFile.read());
            // vector.push_back(dataFile.read());
        }
        dataFile.close();
        
        char *split = strtok(tempString, delimiter);
        
        while(split != nullptr){
            vector.push_back(String(split));
            split = strtok(nullptr, delimiter);
        }

        Serial.println(String(F("Read contents of file ")) + filePath + String(F(" complete.")));
        return vector;
    }

    else{
        Serial.println(String(F("An error has occured while opening file: ")) + filePath);
    }



    return vector;

}

void SDManager::removeFile(const String filePath){
    SD.remove(filePath);
    Serial.println("Removed file: " + filePath);
}