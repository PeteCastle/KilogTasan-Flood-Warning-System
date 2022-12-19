#ifndef UltrasonicSensor_h
#define UltrasonicSensor_h

#include <Arduino.h>
class UltrasonicSensor{
    public:
        UltrasonicSensor( int TRIG_PIN,  int ECHO_PIN,  double RIVER_DEPTH,  int YELLOW_LEVEL_THRESHOLD,  int ORANGE_LEVEL_THRESHOLD,  int RED_LEVEL_THRESHOLD);
        double getDistance();
        double getRiverLevel(int currentDistance = -1);
        void begin();
        int getWarningLevel(int currentDistance = -1);
    private:
        const int _TRIG_PIN;
        const int _ECHO_PIN;
        const double _RIVER_DEPTH;
        const int _YELLOW_LEVEL_THRESHOLD;
        const int _ORANGE_LEVEL_THRESHOLD;
        const int _RED_LEVEL_THRESHOLD;
};

#endif