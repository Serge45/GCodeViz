#ifndef COMMONMOTIONPARAMETER_H
#define COMMONMOTIONPARAMETER_H
#include <string>

struct CommonMotionParameter {
    enum StartType : uint32_t {
        ImmediateStart = 1,
        InPosition,
        GeneratorCompleted,
        Sync,
        SuperImmediate
    };

    double acceleration = 1000;
    double deceleration = 1000;
    StartType startType = GeneratorCompleted;
    double smoothFactor = 1;
    double jerk = 50;
    double velocityRate = 100;
    double accelerationRate = 100;
    double decelerationRate = 100;
    double jerkRate = 100;

    std::string toStdString() const;

};


#endif // COMMONMOTIONPARAMETER_H

