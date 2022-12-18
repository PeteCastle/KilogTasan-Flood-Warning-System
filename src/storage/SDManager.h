#ifndef SDManager_h
#define SDManager_h

#include <SPI.h>
#include "SD.h"
 #include "../libraries/Vector.h"

class SDManager{
    public:
        SDManager(const int SD_CS_PIN);
        void removeFile(const String filePath);
        Vector<String> readFile(const String filePath);
        void writeFile(const String filePath, String value);
    
    private:
        const int _SC_CS_PIN;
};

#endif