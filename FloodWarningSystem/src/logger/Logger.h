#ifndef Logger_h
#define Logger_h
#include "../notifier/LCDManager.h"
#include "../storage/SDManager.h"

class Logger{
    public: 
        Logger(LCDManager *lcd, SDManager *sd, String RECIPIENTS_FILE, String OEPRATIONS_FILE, String MEASUREMENTS_FILE);
        void standardLog(String log);
        void measureLog(String time, int rainLevel, int riverLevel);
    private:
        LCDManager *lcd;
        SDManager *sd;
        const String _RECIPIENTS_FILE;
        const String _OPERATIONS_FILE;
        const String _MEASUREMENTS_FILE;
};

#endif