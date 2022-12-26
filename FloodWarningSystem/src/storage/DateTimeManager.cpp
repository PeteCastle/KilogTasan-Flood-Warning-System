#include "DateTimeManager.h"

// ThreeWire myWire(RTC_RESET_PIN,RTC_DATA_PIN,RTC_CLOCK_PIN); // IO, SCLK, CE
// RtcDS1302<ThreeWire> Rtc(myWire);

DateTimeManager::DateTimeManager(const int DATA_PIN, const int CLOCK_PIN, const int RESET_PIN):
    datetimeWire(DATA_PIN, CLOCK_PIN, RESET_PIN),
    Rtc(datetimeWire){

}

void DateTimeManager::begin(){
    Rtc.Begin();
    !Rtc.IsDateTimeValid() ? updateDateTime() : void() ;
}

void DateTimeManager::updateDateTime(){
    Serial.println(F("Updating datetime!"));
    Rtc.GetIsWriteProtected() ? Rtc.SetIsWriteProtected(false) : void();
    !Rtc.GetIsRunning() ? Rtc.SetIsRunning(true) : void();

    RtcDateTime compiler_date_time = RtcDateTime(__DATE__, __TIME__);
    RtcDateTime module_date_time = Rtc.GetDateTime();
    module_date_time < compiler_date_time ? Rtc.SetDateTime(compiler_date_time) : void();
}