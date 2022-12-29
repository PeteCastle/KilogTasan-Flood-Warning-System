#ifndef UltrasonicSensor_h
#define UltrasonicSensor_h

#include <Arduino.h>
class UltrasonicSensor{
    public:
        UltrasonicSensor( byte TRIG_PIN,  byte ECHO_PIN,  double RIVER_DEPTH);
        int getDistance();
        double getRiverLevel(int currentDistance = -1);
        void begin();
        byte getWarningLevel(int currentDistance = -1);
    private:
        const byte _TRIG_PIN;
        const byte _ECHO_PIN;
        const double _RIVER_DEPTH;
        const int _YELLOW_LEVEL_THRESHOLD;
        const int _ORANGE_LEVEL_THRESHOLD;
        const int _RED_LEVEL_THRESHOLD;
};

#endif