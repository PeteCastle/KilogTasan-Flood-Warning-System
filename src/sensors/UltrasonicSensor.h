#ifndef UltrasonicSensor_h
#define UltrasonicSensor_h

class UltrasonicSensor{
    public:
        UltrasonicSensor(const int TRIG_PIN, const int ECHO_PIN, const double RIVER_DEPTH);
        double getDistance();
        double getRiverLevel();
    private:
        const int _TRIG_PIN;
        const int _ECHO_PIN;
        const double _RIVER_DEPTH;
};

#endif