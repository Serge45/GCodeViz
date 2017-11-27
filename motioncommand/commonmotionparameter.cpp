#include "commonmotionparameter.h"
#include <sstream>

std::string CommonMotionParameter::toStdString() const {
    std::stringstream cmd;
    cmd << " Acc="
        << acceleration
        << " Dec="
        << deceleration
        << " StartType="
        << startType
        << " SmoothFactor="
        << smoothFactor
        << " Jerk="
        << jerk
        << " VRate="
        << velocityRate
        << " ARate="
        << accelerationRate
        << " DRate="
        << decelerationRate
        << " JRate="
        << jerkRate;
    return cmd.str();
}
