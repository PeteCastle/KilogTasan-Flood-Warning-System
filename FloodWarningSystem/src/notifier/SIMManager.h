#ifndef SIMManager_h    
#define SIMManager_h

#include <Arduino.h>
#include <SoftwareSerial.h>


class SIMManager{
    public:
        SIMManager( int SIM_RX_PIN,  int SIM_TX_PIN,  int SIM_RESET_PIN);
        void reset();
        bool sendSms(char number, char text);
        void begin();

    private:
        const int _SIM_RX_PIN;
        const int _SIM_TX_PIN;
        const int _SIM_RESET_PIN;
        SoftwareSerial SIM;
        int _timeout;
        String _buffer;
        String _readSerial();

};

#endif