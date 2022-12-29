#ifndef SDManager_h
#define SDManager_h

#include <SPI.h>
#include "SD.h"
#include <Arduino.h>

class SDManager{
    private:

    public:
        SDManager( byte SD_CS_PIN);
        void removeFile(String filePath);

        String readFile( String filePath);
        // Vector<String> readFile( String filePath);
        void writeFile(String filePath, String value);
        void writeFileReplace(String filePath, String value);
        // void writeFile(String filePath, Vector<String> values);
        bool begin();
        const byte _SC_CS_PIN;
    
    
        
};

#endif