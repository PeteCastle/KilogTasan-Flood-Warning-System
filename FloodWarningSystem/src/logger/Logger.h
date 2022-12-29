#ifndef Logger_h
#define Logger_h
#include "../notifier/LCDManager.h"
#include "../storage/SDManager.h"

class Logger{
    public: 
        Logger(LCDManager *lcd, SDManager *sd);
        void standardLog(String log);
        void measureLog(String time, int rainLevel, int riverLevel);
    private:
        LCDManager *lcd;
        SDManager *sd;
        //  String _RECIPIENTS_FILE;
        //  String _OPERATIONS_FILE;
        //  String _MEASUREMENTS_FILE;
        //File Configs

};

#endif