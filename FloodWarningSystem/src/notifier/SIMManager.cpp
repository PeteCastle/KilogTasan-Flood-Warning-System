#include "SIMManager.h"

SIMManager::SIMManager(const int SIM_RX_PIN, const int SIM_TX_PIN, const int SIM_RESET_PIN) :
    _SIM_RX_PIN(SIM_RX_PIN),
    _SIM_TX_PIN(SIM_TX_PIN),
    _SIM_RESET_PIN(SIM_RESET_PIN),
    SIM(_SIM_TX_PIN,_SIM_RX_PIN){
    
    }

void SIMManager::begin(){
    //_buffer.reserve(512);
    SIM.begin(9600);
}

String SIMManager::_readSerial(){
    _timeout=0;
    
    while(!SIM.available() && _timeout<12000) {
        Serial.println("not available");
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
    Serial.println(String(F("Sending a message...")));
    SIM.print(F("AT+CMGF=1\r")); //set sms to text mode 
    delay(200);
    _buffer=_readSerial();

    SIM.print (F("AT+CMGS=\""));  // command to send sms
    SIM.print (number);         
    SIM.print(F("\"\r"));     
    delay(200);    
    _buffer=_readSerial(); 

    SIM.print (text);
    SIM.print ("\r"); //change delay 100 to readserial	
    _buffer=_readSerial();
    SIM.print((char)26);
    _buffer=_readSerial();
    //expect CMGS:xxx   , where x is a number,for the sending sms.
    _buffer.indexOf("CMGS")  != -1 ? Serial.println(String(F("Successfully sent a message to ")) + number) : Serial.println(String(F("Failed to send a message to ")) + number);
    return _buffer.indexOf("CMGS")  != -1 ?  true: false;
}

void SIMManager::sendSmsMultipleRecipients(String message, Vector<String> listOfRecipients){
    for(String recipient : listOfRecipients){
        sendSms(recipient, message);
    }
}

bool SIMManager::getSIMConnectivityStatus(){
    SIM.print(F("AT+CREG?\r"));
    _buffer = _readSerial();
    //Serial.println("START  OF BUFFER");
    //Serial.println(_buffer);
    return _buffer.substring(18,21) == F("0,1");

    // return true;
}
//https://m2msupport.net/m2msupport/globe-telecomphilippines-set-the-apn-to-internet-globe-com-ph/
//https://stackoverflow.com/questions/63187583/arduino-sim900-atsapbr-1-1-operation-not-allowed

void SIMManager::sendHttpRequest(){
    SIM.print(F("AT+SAPBR=3,1,\"Contype\",\"GPRS\"\r"));
    _buffer = _readSerial();
    SIM.print(F("AT+SAPBR=3,1,\"APN\",\"internet.globe.com.ph\"\r"));
    _buffer = _readSerial();
    SIM.print(F("AT+SAPBR=1,1\r"));
    delay(15000);  
    _buffer = _readSerial();

    SIM.print(F("AT+SAPBR=2,1\r"));
    Serial.println(_readSerial());

    SIM.print(F("AT+HTTPINIT\r"));
    _buffer = _readSerial();
    SIM.print(F("AT+HTTPPARA=\"CID\",1\r"));
    _buffer = _readSerial();
    SIM.print(F("AT+HTTPPARA=\"URL\",\"http://worldtimeapi.org/api/timezone/Asia/Manila\"\r"));
    _buffer = _readSerial();
    SIM.print(F("AT+HTTPPARA=\"CONTENT\",\"application/json\"\r"));
    _buffer = _readSerial();
    SIM.print(F("AT+HTTPACTION=0\r"));
    delay(6000);
    _buffer = _readSerial();
    SIM.print(F("AT+HTTPREAD\r"));
    Serial.println("HTTP READ VALUE:");
    Serial.println(_readSerial());

    SIM.print(F("AT+HTTPTERM"));
    _buffer = _readSerial();

    Serial.println("end of method:");
}