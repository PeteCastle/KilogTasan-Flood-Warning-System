#include "RainSensor.h"

RainSensor::RainSensor(const uint8_t RAIN_SENSOR_PIN, const byte SAMPLES, const byte SENSITIVITY, const byte THRESHOLD) :
    _RAIN_SENSOR_PIN(RAIN_SENSOR_PIN),
    _SAMPLES(SAMPLES),
    _SENSITIVITY(SENSITIVITY),
    _THRESHOLD(THRESHOLD){
        pinMode(_RAIN_SENSOR_PIN, INPUT_PULLUP);
    }

int RainSensor::getSensorValue(){
    int Y = 1023 * _SAMPLES;
    for (size_t i=0; i< _SAMPLES; i++){
        Y -= analogRead(_RAIN_SENSOR_PIN);
    }
    return Y / _SAMPLES;
}

byte RainSensor::getSampledValue(){
    byte val = 0;
    int sumXY = 0;
    int sumX = 0;
    int sumY = 0;
    int sumX2 = 0;
    if(samples < 1){
        samples = 1;
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

bool RainSensor::isRaining(){
    getSampledValue(_SAMPLES,_SENSITIVITY) >= _THRESHOLD ? return True: return False;
}