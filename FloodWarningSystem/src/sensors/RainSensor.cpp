#include "RainSensor.h"

RainSensor::RainSensor(const uint8_t RAIN_SENSOR_PIN, byte samples, const byte SENSITIVITY, const byte YELLOW_RAIN_THRESHOLD, const byte ORANGE_RAIN_THRESHOLD, const byte RED_RAIN_THRESHOLD) :
    _RAIN_SENSOR_PIN(RAIN_SENSOR_PIN),
    _samples(samples),
    _SENSITIVITY(SENSITIVITY),
    _YELLOW_RAIN_THRESHOLD(YELLOW_RAIN_THRESHOLD),
    _ORANGE_RAIN_THRESHOLD(ORANGE_RAIN_THRESHOLD),
    _RED_RAIN_THRESHOLD(ORANGE_RAIN_THRESHOLD){
        
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
    byte val = 0;
    int sumXY = 0;
    int sumX = 0;
    int sumY = 0;
    int sumX2 = 0;
    if(_samples < 1){
        _samples = 1;
    }
    for(int i = 0; i < _SENSITIVITY; i++) {
        int Y = getSensorValue();
        sumXY += i * Y;
        sumX += i;
        sumY += Y;
        sumX2 += i * i;
    }
    val = (_SENSITIVITY * sumXY - sumX * sumY) / (_SENSITIVITY * sumX2 - sumX * sumX);
    if(val < 0) val *= -1;
    if(val > 100) val = 100;
    return val;
}

int RainSensor::getWarningLevel(int currentRainLevel = -1){
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
        Serial.println("rainLevel outside the range.");
    }

}