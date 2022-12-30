#include "RainSensor.h"

RainSensor::RainSensor(const uint8_t RAIN_SENSOR_PIN, byte samples) :
    _RAIN_SENSOR_PIN(RAIN_SENSOR_PIN),
    _samples(samples),
    _YELLOW_RAIN_THRESHOLD(80),
    _ORANGE_RAIN_THRESHOLD(90),
    _RED_RAIN_THRESHOLD(100){
    }

void RainSensor::begin(){
    pinMode(_RAIN_SENSOR_PIN, INPUT_PULLUP);
}

int RainSensor::getSensorValue(){
    int Y = 1023 * _samples;
    for (size_t i=0; i< _samples; i++){
        Y -= analogRead(_RAIN_SENSOR_PIN);
    }
    return Y / _samples;
}

byte RainSensor::getSampledValue(){
    int Y = getSensorValue();
    return map(Y, 0, 1023, 0, 100);;
}

byte RainSensor::getWarningLevel(int currentRainLevel = -1){
    currentRainLevel==-1 ? currentRainLevel = getSampledValue() : false;

    if (currentRainLevel >= _RED_RAIN_THRESHOLD ){
        return 3;
    }
    else if (currentRainLevel >= _ORANGE_RAIN_THRESHOLD && currentRainLevel < _RED_RAIN_THRESHOLD){
        return 2;
    }
    else if (currentRainLevel >= _YELLOW_RAIN_THRESHOLD && currentRainLevel  <_ORANGE_RAIN_THRESHOLD){
        return 1;
    }
    else if (currentRainLevel < _YELLOW_RAIN_THRESHOLD){
        return 0;
    }
    else{
        Serial.println(F("rainLevel outside the range."));
    }

}