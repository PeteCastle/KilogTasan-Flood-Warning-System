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

        // Serial.println(distance);
        // lcd.print(distance);
    //This is to return zero when the the distane is actually close (<0.10m).  Reads 900.  Max should be 400 or 4m
    // TO DO: The ultrasonic sensor is not yet tested when distance is greater than 4m
    // return (distance >= 900) ?  0 : distance;
    return distance;
}

double UltrasonicSensor::getRiverLevel(int currentDistance){
    currentDistance==-1 ? currentDistance = getDistance() : false;
    double waterLevel = _RIVER_DEPTH - currentDistance;

    if (waterLevel == _RIVER_DEPTH){
        return -1;
    }
    // TO DO: Return water level when measured distance is beyond the scope of the ultrasonic sensor.
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
        // Serial.println("red OF THE ABOVE");
        return 3;
    }
    else if (currentDistance <= _ORANGE_LEVEL_THRESHOLD && currentDistance > _RED_LEVEL_THRESHOLD){
        // Serial.println("orange OF THE ABOVE");
        return 2;
    }
    else if (currentDistance <= _YELLOW_LEVEL_THRESHOLD && currentDistance > _ORANGE_LEVEL_THRESHOLD){
        // Serial.println("yellow OF THE ABOVE");
        return 1;
    }
    else if (currentDistance >_YELLOW_LEVEL_THRESHOLD){
        // Serial.println("NONE OF THE ABOVE");
        return 0;
    }
    else{
        Serial.println(F("currentDistance outside the range."));
    }
}