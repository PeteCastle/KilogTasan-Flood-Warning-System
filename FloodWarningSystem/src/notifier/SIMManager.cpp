#include "SIMManager.h"

SIMManager::SIMManager(const byte SIM_RX_PIN, const byte SIM_TX_PIN, const byte SIM_RESET_PIN) :
    _SIM_RX_PIN(SIM_RX_PIN),
    _SIM_TX_PIN(SIM_TX_PIN),
    _SIM_RESET_PIN(SIM_RESET_PIN),
    SIM(_SIM_TX_PIN,_SIM_RX_PIN){
    
    }

void SIMManager::begin(){
    //_buffer.reserve(128);
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
    while (_readSerial().indexOf(F("OK"))==-1 ){
        SIM.print(F("AT\r\n"));
    }
    while (_readSerial().indexOf(F("SMS"))==-1 ){}
}

bool SIMManager::sendSms(String number, String text){
    Serial.println(String(F("Sending a message...")));
    SIM.print(F("AT+CMGF=1\r")); //set sms to text mode 
    delay(400);

    SIM.print (F("AT+CMGS=\""));  // command to send sms
    SIM.print (number);         
    SIM.print(F("\"\r"));     
    delay(400);    

    SIM.print (text);
    SIM.print ("\r"); //change delay 100 to readserial	

    SIM.print((char)26);
    delay(400);
    _buffer=_readSerial();
    //expect CMGS:xxx   , where x is a number,for the sending sms.
    _buffer.indexOf(F("CMGS"))  != -1 ? Serial.println(String(F("Successfully sent a message to ")) + String(number)) : Serial.println(String(F("Failed to send a message to ")) + number);
    return _buffer.indexOf(F("CMGS"))  != -1 ?  true: false;
}

bool SIMManager::sendSms(String number, String *text){
    Serial.println(F("Sending message with pointer"));
    
    SIM.print(F("AT+CMGF=1\r")); //set sms to text mode 
    delay(400);
    _buffer=_readSerial();

    SIM.print (F("AT+CMGS=\""));  // command to send sms
    SIM.print (number);         
    SIM.print(F("\"\r"));     
    delay(400);    
    _buffer=_readSerial(); 
    SIM.print (*text);
    SIM.print ("\r"); //change delay 100 to readserial	
    _buffer=_readSerial();
    SIM.print((char)26);
    _buffer=_readSerial();
    //expect CMGS:xxx   , where x is a number,for the sending sms.
    if(_buffer.indexOf(F("CMGS"))  != -1 ){
        Serial.print(F("Successfully sent a message to "));
        Serial.println(number);
    }
    else{
        Serial.print(F("Failed to send a message to "));
        Serial.println(number);
        Serial.println(_buffer);
    }
    return _buffer.indexOf(F("CMGS"))  != -1 ?  true: false;
}

bool SIMManager::getSIMConnectivityStatus(){
    SIM.print(F("AT+CREG?\r"));
    _buffer = _readSerial();
    return _buffer.substring(18,21) == F("0,1");
}

//NOTE: HTTP REQUESTS CURRENTLY NOT USED
// void SIMManager::sendHttpRequest(String URI){
//     // SIM.print(F("AT+SAPBR=3,1,\"Contype\",\"GPRS\"\r"));
//     // _buffer = _readSerial();
    
//     SIM.print(F("AT+SAPBR=3,1,\"APN\",\"internet.globe.com.ph\"\r"));
//     _buffer = _readSerial();

//     do{
//         Serial.println(String(F("Establishing GPRS connection")));
//         SIM.print(F("AT+SAPBR=1,1\r"));
//     } while(_readSerial().substring(9,12) != F("1,1"));
//     Serial.println(String(F("GPRS connection established")));

//     SIM.print(F("AT+CMEE=2\r"));
//     _buffer = _readSerial();
//     SIM.print(_buffer);

//     SIM.print(F("AT+SAPBR=2,1\r"));
//     _buffer = _readSerial();
//     SIM.print(_buffer);

//     SIM.print(F("AT+HTTPINIT\r"));
//     _buffer = _readSerial();
//     SIM.print(_buffer);

//     SIM.print(F("AT+HTTPPARA=\"CID\",1\r"));
//     _buffer = _readSerial();
//     SIM.print(_buffer);

//     SIM.print(F("AT+HTTPPARA=\"URL\",\"http://google.com\"\r"));
//     SIM.print(_buffer);

//     SIM.print(F("AT+HTTPPARA=\"CONTENT\",\"application/json\"\r"));
//     _buffer = _readSerial();

//     SIM.print(F("AT+HTTPACTION=0\r"));
//     delay(10000);
//     _buffer = _readSerial();
//     SIM.print(F("AT+HTTPREAD\r"));
//     Serial.println(_readSerial());

//     SIM.print(F("AT+HTTPTERM"));
//     _buffer = _readSerial();

//     SIM.print(F("AT+SAPBR=0,1\r"));
//     _buffer = _readSerial();
// }

void SIMManager::sendMessageToAllRecipients(String *content, SDManager *sd){
    String RECIPIENTS_FILE = String(F("sendlist.txt"));
    
    File dataFile = SD.open(RECIPIENTS_FILE);

    if(dataFile){
        String tempString;
        while(dataFile.available()){
            int ascii = dataFile.read();
            if(ascii==10){}
            else if(ascii!=13){ // If not a newline character
                tempString+= (char) ascii;
            }
            else{
                sendSms(tempString,content);
                tempString="";
            }
        }
        tempString != ""  ? sendSms(tempString,content) : bool();
        dataFile.close();
        Serial.println(String(F("Read contents of file ")) + RECIPIENTS_FILE + String(F(" complete...")));
    }
    else{
        Serial.print(F("An error has occured while opening file: "));
        Serial.println(RECIPIENTS_FILE);
    }
}
