#include <SoftwareSerial.h>

class SIMManager{
    public:
        SIMManager(const int SIM_RX_PIN, const int SIM_TX_PIN, const int SIM_RESET_PIN);
        void reset();
        bool sendSms(char number, char text);

    private:
        const int _SIM_RX_PIN;
        const int _SIM_TX_PIN;
        const int _SIM_RESET_PIN;
        SoftwareSerial SIM;
        int _timeout;
        String _buffer;
        String __readSerial();

};