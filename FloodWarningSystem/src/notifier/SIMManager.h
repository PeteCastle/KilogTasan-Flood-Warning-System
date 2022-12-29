#ifndef SIMManager_h    
#define SIMManager_h

#include <Arduino.h>
#include <SoftwareSerial.h>
#include "../storage/SDManager.h"
// #include "../libraries/Vector.h"



class SIMManager{
    public:
        SIMManager( byte SIM_RX_PIN,  byte SIM_TX_PIN,  byte SIM_RESET_PIN);
        void reset();
        bool sendSms(String number, String text);
        bool sendSms(String number, String *text);
        void begin();
       // void sendSmsMultipleRecipients(String message, Vector<String> listOfRecipients);
        void sendHttpRequest(String URI);
        bool getSIMConnectivityStatus();
        void sendMessageToAllRecipients(String *content, SDManager *sd);

    private:
        const byte _SIM_RX_PIN;
        const byte _SIM_TX_PIN;
        const byte _SIM_RESET_PIN;
        SoftwareSerial SIM;
        int _timeout;
        String _buffer;
        String _readSerial();

};

#endif