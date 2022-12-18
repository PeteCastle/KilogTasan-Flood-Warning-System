#include "ConfigManager.h"
#include "notifier/LCDManager.h"
#include "notifier/SIMManager.h"
#include "sensors/UltrasonicSensor.h"
#include "sensors/RainSensor.h"
#include "storage/SDManager.h"

// References
// https://github.com/arduino/ArduinoCore-avr/blob/master/cores/arduino/Arduino.h
// https://github.com/gamegine/HCSR04-ultrasonic-sensor-lib/blob/master/src/HCSR04.h
// https://github.com/naoto64/Arduino-RainSense/blob/master/src/RainSense.h
// https://github.com/PaulStoffregen/SoftwareSerial/blob/master/SoftwareSerial.cpp
// https://github.com/cristiansteib/Sim800l/blob/master/Sim800l.cpp


// TODO :
// Integration of sim numbers from API to the memory card.
// In Ultrasonic Sensor.cpp
    // Return water level when measured distance is beyond the scope of the ultrasonic sensor.
    // The ultrasonic sensor is not yet tested when distance is greater than 4m

String test = ConfigManager.getConfigVar("SAMPLE");

void setup(){
    Serial.begin(9600);
}

void loop(){
    Serial.println("Hello World");
    delay(1000);
}