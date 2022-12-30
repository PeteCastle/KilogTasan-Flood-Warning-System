#include "UltrasonicSensor.h"
#include <Wire.h>


UltrasonicSensor::UltrasonicSensor(const byte TRIG_PIN, const byte ECHO_PIN, const double RIVER_DEPTH) : 
    _TRIG_PIN(TRIG_PIN), 
    _ECHO_PIN(ECHO_PIN),
    _RIVER_DEPTH(RIVER_DEPTH),
    _YELLOW_LEVEL_THRESHOLD(300),
    _ORANGE_LEVEL_THRESHOLD(200),
    _RED_LEVEL_THRESHOLD(100){
}
void UltrasonicSensor::begin(){
    pinMode(_TRIG_PIN, OUTPUT);
    pinMode(_ECHO_PIN, INPUT);
}
int UltrasonicSensor::getDistance(){
    // Note that measurements are always in centimeters (CM)
        digitalWrite(4, LOW);
        delayMicroseconds(2); 
        
        digitalWrite(4, HIGH);
        delayMicroseconds(10);
        digitalWrite(4, LOW);

        long duration = pulseIn(5, HIGH);
        int distance = duration * 0.034 / 2;

    return distance;
}

double UltrasonicSensor::getRiverLevel(int currentDistance){
    currentDistance==-1 ? currentDistance = getDistance() : false;
    double waterLevel = _RIVER_DEPTH - currentDistance;

    if (waterLevel == _RIVER_DEPTH){
        return -1;
    }
    else if( false ){
        return 0;
    }
    else {
        return waterLevel;
    }
}

byte UltrasonicSensor::getWarningLevel(int currentDistance){
    currentDistance==-1 ? currentDistance = getDistance() : false;
    if (currentDistance <= _RED_LEVEL_THRESHOLD ){
        return 3;
    }
    else if (currentDistance <= _ORANGE_LEVEL_THRESHOLD && currentDistance > _RED_LEVEL_THRESHOLD){
        return 2;
    }
    else if (currentDistance <= _YELLOW_LEVEL_THRESHOLD && currentDistance > _ORANGE_LEVEL_THRESHOLD){
        return 1;
    }
    else if (currentDistance >_YELLOW_LEVEL_THRESHOLD){
        return 0;
    }
    else{
        Serial.println(F("currentDistance outside the range."));
    }
}