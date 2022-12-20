#include "SIMManager.h"

SIMManager::SIMManager(const int SIM_RX_PIN, const int SIM_TX_PIN, const int SIM_RESET_PIN) :
    _SIM_RX_PIN(SIM_RX_PIN),
    _SIM_TX_PIN(SIM_TX_PIN),
    _SIM_RESET_PIN(SIM_RESET_PIN),
    SIM(_SIM_RX_PIN, SIM_TX_PIN){
    
    }

void SIMManager::begin(){
    _buffer.reserve(255);
}

String SIMManager::_readSerial(){
    _timeout=0;
    while(!SIM.available() && _timeout<12000) {
        delay(13);
        _timeout++;
    }
    if (SIM.available()) {
        return SIM.readString();
    }
}

void SIMManager::reset(){
    digitalWrite(_SIM_RESET_PIN,1);
    delay(1000);
    digitalWrite(_SIM_RESET_PIN,0);
    delay(1000);

    SIM.print(F("AT\r\n"));
    while (_readSerial().indexOf("OK")==-1 ){
        SIM.print(F("AT\r\n"));
    }
    
    while (_readSerial().indexOf("SMS")==-1 ){}
}

bool SIMManager::sendSms(String number, String text){
    SIM.print (F("AT+CMGF=1\r")); //set sms to text mode  
    _buffer=_readSerial();
    SIM.print (F("AT+CMGS=\""));  // command to send sms
    SIM.print (number);           
    SIM.print(F("\"\r"));       
    _buffer=_readSerial(); 
    SIM.print (text);
    SIM.print ("\r"); //change delay 100 to readserial	
    _buffer=_readSerial();
    SIM.print((char)26);
    _buffer=_readSerial();
    //expect CMGS:xxx   , where xxx is a number,for the sending sms.
    return _buffer.indexOf("CMGS")  != -1 ?  true: false;
}

void SIMManager::sendSmsMultipleRecipients(String message, Vector<String> listOfRecipients){
    for(String recipient : listOfRecipients){
        sendSms(recipient, message);
    }
}

//https://m2msupport.net/m2msupport/globe-telecomphilippines-set-the-apn-to-internet-globe-com-ph/
//https://stackoverflow.com/questions/63187583/arduino-sim900-atsapbr-1-1-operation-not-allowed

// void sendHttpRequest(){
//     SIM.print(F("AT+SAPBR=3,1,\"Contype\",\"GPRS\"\r"));
//     SIM.flush();
//     SIM.print("AT+SAPBR=3,1,\"APN\",\"internet.globe.com.ph\"\r");
//     SIM.flush(); 
//     SIM.print("AT+SAPBR=1,1\r");
//     delay(15000);  
//     SIM.flush(); 

//     SIM.print("AT+SAPBR=2,1\r");
//     Serial.println(_readSerial());

//     SIM.print("AT+HTTPINIT\r");
//     SIM.flush();
//     SIM.print("AT+HTTPPARA=\"CID\",1\r");
//     SIM.flush();
//     SIM.flush();
//     SIM.print("AT+HTTPPARA=\"URL\",\"my-api.com/foo\"\r");
//     SIM.flush();
//     SIM.print("AT+HTTPPARA=\"CONTENT\",\"application/json\"\r");
//     SIM.flush();
//     SIM.print("AT+HTTPACTION=0\r");
//     SIM.flush();
//     SIM.print("AT+HTTPREAD\r");
//     SIM.flush();
// }