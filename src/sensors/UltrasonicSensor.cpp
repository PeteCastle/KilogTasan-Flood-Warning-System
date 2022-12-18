#include "UltrasonicSensor.h"

UltrasonicSensor::UltrasonicSensor(const int TRIG_PIN, const int ECHO_PIN, const double RIVER_DEPTH) : 
    _TRIG_PIN(TRIG_PIN), 
    _ECHO_PIN(ECHO_PIN),
    _RIVER_DEPTH(RIVER_DEPTH){
        pinMode(_TRIG_PIN, OUTPUT);
        pinMode(_ECHO_PIN, INPUT)
}

double UltrasonicSensor::getDistance(){
    // Note that measurements are always in centimeters (CM)
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2); 

    digitalWrite(trigPin, HIGH);
  
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    long duration = pulseIn(echoPin, HIGH);
    double distance = duration * 0.034 / 2;

    //This is to return zero when the the distane is actually close (<0.10m).  Reads 900.  Max should be 400 or 4m
    // TO DO: The ultrasonic sensor is not yet tested when distance is greater than 4m
    distance >= 900 ? return 0 : return distance;
}

double UltrasonicSensor::getRiverLevel(){
    int currentDistance = getDistance();
    double waterLevel = _RIVER_DEPTH - currentDistance;

    if (waterLevel == _RIVER_DEPTH){
        return -1;
    }
    // TO DO: Return water level when measured distance is beyond the scope of the ultrasonic sensor.
    else if( False ){
        return 0;
    }
    else {
        return waterLevel;
    }
}