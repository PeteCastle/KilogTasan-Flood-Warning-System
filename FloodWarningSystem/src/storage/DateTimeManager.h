#ifndef DateTimeManager_h
#define DateTimeManager_h

// #include "../libraries/RTC/ThreeWire.h"
// #include "../libraries/RTC/RtcDS1302.h"

#include <stdint.h>
#include <Arduino.h>

#include <ThreeWire.h>
#include <RtcDS1302.h>

#define MESSAGE_INTERVAL (int) 5  // In minutes

class DateTimeManager{
    public:
        DateTimeManager(byte DATA_PIN, byte CLOCK_PIN, byte RESET_PIN);
        void begin();
        void updateDateTime();
        String getCurrentDate();
        String getCurrentTime();
        String getCurrentDateTimeString();
        RtcDateTime getCurrentDateTime();
        bool withinInterval(RtcDateTime *previous, int timeInterval = MESSAGE_INTERVAL);
    private:
        ThreeWire datetimeWire;
        RtcDS1302<ThreeWire> Rtc;
};

#endif