#include "ConfigManager.h"

String test = ConfigManager.getConfigVar("SAMPLE");

void setup(){
    Serial.begin(9600);
}

void loop(){
    Serial.println("Hello World");
    delay(1000);
}