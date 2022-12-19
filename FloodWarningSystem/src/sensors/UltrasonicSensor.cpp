#include "UltrasonicSensor.h"

UltrasonicSensor::UltrasonicSensor(const int TRIG_PIN, const int ECHO_PIN, const double RIVER_DEPTH, const int YELLOW_LEVEL_THRESHOLD, const int ORANGE_LEVEL_THRESHOLD, const int RED_LEVEL_THRESHOLD) : 
    _TRIG_PIN(TRIG_PIN), 
    _ECHO_PIN(ECHO_PIN),
    _RIVER_DEPTH(RIVER_DEPTH),
    _YELLOW_LEVEL_THRESHOLD(YELLOW_LEVEL_THRESHOLD),
    _ORANGE_LEVEL_THRESHOLD(ORANGE_LEVEL_THRESHOLD),
    _RED_LEVEL_THRESHOLD(RED_LEVEL_THRESHOLD){
        
}
void UltrasonicSensor::begin(){
    pinMode(_TRIG_PIN, OUTPUT);
    pinMode(_ECHO_PIN, INPUT);
}
double UltrasonicSensor::getDistance(){
    // Note that measurements are always in centimeters (CM)
    digitalWrite(_TRIG_PIN, LOW);
    delayMicroseconds(2); 

    digitalWrite(_TRIG_PIN, HIGH);
  
    delayMicroseconds(10);
    digitalWrite(_TRIG_PIN, LOW);

    long duration = pulseIn(_ECHO_PIN, HIGH);
    double distance = duration * 0.034 / 2;

    //This is to return zero when the the distane is actually close (<0.10m).  Reads 900.  Max should be 400 or 4m
    // TO DO: The ultrasonic sensor is not yet tested when distance is greater than 4m
    return (distance >= 900) ?  0 : distance;
}

double UltrasonicSensor::getRiverLevel(){
    int currentDistance = getDistance();
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