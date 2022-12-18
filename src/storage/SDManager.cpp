 #include "libraries/SDManager.h"


 
 SDManager::SDManager(const int SD_CS_PIN):
    _SC_CS_PIN(SD_CS_PIN){
        Serial.print("Initializing SD card...");
        if (!SD.begin(chipSelect)) {
            Serial.println("Initialization of sd card failed. SDManager.cpp:9");
            // Serial.println("1. is a card inserted?");
            // Serial.println("2. is your wiring correct?");
            // Serial.println("3. did you change the chipSelect pin to match your shield or module?");
            // Serial.println("Note: press reset button on the board and reopen this Serial Monitor after fixing your issue!");
            while (true);
        }
        Serial.println("initialization done.");

    }

void SDManager::writeFile(const String filePath, String value){
    File dataFile = SD.open(filePath, FILE_WRITE);

    if(dataFile){
        dataFile.println(value);
        dataFile.close();
        Serial.println("The following contents were added to the file " + filePath + ": \n" + value);
    }
    else{
        Serial.println("An error has occured while opening file: " + filePath);
    }
}
void SDManager::writeFile(const String filePath, Vector<String> values){
    File dataFile = SD.open(filePath, FILE_WRITE);

    if(dataFile){
        for( string value : values) dataFile.println(value + "\n");
        dataFile.close();
        Serial.println("The following contents were added to the file " + filePath + ": \n" + value);
    }
    else{
        Serial.println("An error has occured while opening file: " + filePath);
    }
}

Vector<String> SDManager::readFile(const String filePath){
    File dataFile = SD.open(filePath, FILE_WRITE);

    String storage_array(1);
    Vector<String> vector(storage_array);

    if(dataFile){
        while(dataFile.available()){
            vector.push_back(dataFile.read());
        }
        dataFile.close();
        Serial.println("Read contents of file " + filePath + " complete.");
        return vector;
    }
    else{
        Serial.println("An error has occured while opening file: " + filePath);
    }

}

void SDManager::removeFile(const String filePath){
    SD.remove(filePath);
    Serial.println("Removed file: " + filePath);
}