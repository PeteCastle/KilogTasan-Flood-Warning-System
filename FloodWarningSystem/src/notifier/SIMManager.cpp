#include "SIMManager.h"

SIMManager::SIMManager(const int SIM_RX_PIN, const int SIM_TX_PIN, const int SIM_RESET_PIN) :
    _SIM_RX_PIN(SIM_RX_PIN),
    _SIM_TX_PIN(SIM_TX_PIN),
    _SIM_RESET_PIN(SIM_RESET_PIN),
    SIM(_SIM_TX_PIN,_SIM_RX_PIN){
    
    }

void SIMManager::begin(){
    _buffer.reserve(128);
    SIM.begin(9600);
}

String SIMManager::_readSerial(){
    _timeout=0;
    
    while(!SIM.available() && _timeout<12000) {
        //Serial.println("not available");
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

void SIMManager::sendHttpRequest(String URI){
    // SIM.print(F("AT+SAPBR=3,1,\"Contype\",\"GPRS\"\r"));
    // _buffer = _readSerial();
    
    SIM.print(F("AT+SAPBR=3,1,\"APN\",\"internet.globe.com.ph\"\r"));
    _buffer = _readSerial();

    // SIM.print(F("AT+CGDCONT= 1,\"IP\",\"http.globe.com.ph\",\"0.0.0.0\",0,0"));
    // _buffer = _readSerial();

    // SIM.print(F("AT+CSTT=\"http.globe.com.ph\",\"\",\"\"\r"));
    // _buffer = _readSerial();
    // SIM.print(_buffer);

    // SIM.print(F("AT+SAPBR=1,1\r"));
    // delay(5000);  
    // _buffer = _readSerial();
    // Serial.print("THS ONE");
    // Serial.println(_buffer.substring(9,12));
    
//     AT+SAPBR=2,1

// +SAPBR: 1,1,"100.94.95.53"

// OK
    do{
        Serial.println(String(F("Establishing GPRS connection")));
        SIM.print(F("AT+SAPBR=1,1\r"));
    } while(_readSerial().substring(9,12) != F("1,1"));
    Serial.println(String(F("GPRS connection established")));

    SIM.print(F("AT+CMEE=2\r"));
    _buffer = _readSerial();
    SIM.print(_buffer);

    SIM.print(F("AT+SAPBR=2,1\r"));
    _buffer = _readSerial();
    SIM.print(_buffer);

    SIM.print(F("AT+HTTPINIT\r"));
    _buffer = _readSerial();
    SIM.print(_buffer);

    SIM.print(F("AT+HTTPPARA=\"CID\",1\r"));
    _buffer = _readSerial();
    SIM.print(_buffer);

    SIM.print(F("AT+HTTPPARA=\"URL\",\"http://google.com\"\r"));
    // SIM.print(F("AT+HTTPPARA=\"URL\",\""));
    // Serial.println(URI);
    // SIM.print(URI);
    // SIM.print(F("\"\r"));
    _buffer = _readSerial();
    SIM.print(_buffer);

    SIM.print(F("AT+HTTPPARA=\"CONTENT\",\"application/json\"\r"));
    _buffer = _readSerial();

    // SIM.print(F("AT+SAPBR=3,1"));
    // _buffer = _readSerial();
    // Serial.println(_buffer);


    SIM.print(F("AT+HTTPACTION=0\r"));
    delay(10000);
    _buffer = _readSerial();
    SIM.print(F("AT+HTTPREAD\r"));
    Serial.println("HTTP READ VALUE:");
    Serial.println(_readSerial());

    SIM.print(F("AT+HTTPTERM"));
    _buffer = _readSerial();

    SIM.print(F("AT+SAPBR=0,1\r"));
    _buffer = _readSerial();

    Serial.println("end of method:");
}