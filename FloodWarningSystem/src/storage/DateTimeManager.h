#ifndef DateTimeManager_h
#define DateTimeManager_h

// #include "../libraries/RTC/ThreeWire.h"
// #include "../libraries/RTC/RtcDS1302.h"

#include <stdint.h>
#include <Arduino.h>

#include <ThreeWire.h>
#include <RtcDS1302.h>

class DateTimeManager{
    public:
        DateTimeManager(int DATA_PIN, int CLOCK_PIN, int RESET_PIN);
        void begin();
        void updateDateTime();
        String getCurrentTime();
        bool withinInterval(RtcDateTime previous, int threshold);

    private:
        ThreeWire datetimeWire;
        RtcDS1302<ThreeWire> Rtc;
};

#endif