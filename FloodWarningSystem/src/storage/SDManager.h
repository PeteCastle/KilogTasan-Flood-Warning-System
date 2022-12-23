#ifndef SDManager_h
#define SDManager_h

#include <SPI.h>
#include "SD.h"
 #include "../libraries/Vector.h"
#include <Arduino.h>

class SDManager{
    public:
        SDManager( int SD_CS_PIN);
        void removeFile(String filePath);
        Vector<String> readFile( String filePath);
        void writeFile(String filePath, String value);
        void writeFile(String filePath, Vector<String> values);
        bool begin();
    
    private:
        const int _SC_CS_PIN;
};

#endif