#include "DateTimeManager.h"

// ThreeWire myWire(RTC_RESET_PIN,RTC_DATA_PIN,RTC_CLOCK_PIN); // IO, SCLK, CE
// RtcDS1302<ThreeWire> Rtc(myWire);

DateTimeManager::DateTimeManager(const byte DATA_PIN, const byte CLOCK_PIN, const byte RESET_PIN):
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

RtcDateTime DateTimeManager::getCurrentDateTime(){
    return Rtc.GetDateTime();
}
String DateTimeManager::getCurrentDateTimeString(){
    RtcDateTime now = Rtc.GetDateTime();
    return String(now.Month()) + String(F("/")) + String(now.Day()) + String(F("/")) + String(now.Year()-2000)  +String(F(" "))
        + String(now.Hour()) + String(F(":")) + String(now.Minute()) + String(F(":")) + String(now.Second());  
} 

String DateTimeManager::getCurrentDate(){
    String date;
    RtcDateTime now = Rtc.GetDateTime();
    switch(now.Month()){
        case 1: date+=String(F("Jan ")); break;
        case 2: date+=String(F("Feb ")); break;
        case 3: date+=String(F("Mar ")); break;
        case 4: date+=String(F("Apr ")); break;
        case 5: date+=String(F("Mau ")); break;
        case 6: date+=String(F("Jun ")); break;
        case 7: date+=String(F("Jul ")); break;
        case 8: date+=String(F("Aug ")); break;
        case 9: date+=String(F("Sep ")); break;
        case 10: date+=String(F("Oct ")); break;
        case 11: date+=String(F("Nov ")); break;
        case 12: date+=String(F("Dec ")); break;
        default: date+=String(F("Unk ")); break;
    }
    now.Day() <10 ? date+= String(F(" 0")) + String(now.Day()): date+=String(now.Day());
    return date + String(F(" ")) + now.Year();
}

String DateTimeManager::getCurrentTime(){
    RtcDateTime now = Rtc.GetDateTime();
    return String(now.Hour()) + String(F(":")) + String(now.Minute()) + String(F(":")) + String(now.Second());  
}
bool DateTimeManager::withinInterval(RtcDateTime *previous, int timeInterval){ // tmeInterval in minutes
    RtcDateTime now = Rtc.GetDateTime();
    return (now.TotalSeconds() - previous->TotalSeconds()) >= timeInterval * 60;
}


