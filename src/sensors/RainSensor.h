#ifndef RainSensor_h
#define RainSensor_h

class RainSensor{
    private:
        const uint8_t _RAIN_SENSOR_PIN;
        const byte _SAMPLES;
        const byte _SENSITIVITY;
        const byte _THRESHOLD;
        int getSensorValue();
    public:
        RainSensor(const uint8_t RAIN_SENSOR_PIN, const byte SAMPLES, const byte SENSITIVITY, const byte THRESHOLD);
        byte getSampledValue();
        bool isRaining();

  
};

#endif