#ifndef MOVEMOTIONPARAMETER
#define MOVEMOTIONPARAMETER
#include "commonmotionparameter.h"

struct MoveMotionParameter {
    enum CoordinateSystem : uint32_t {
        Increment = 0,
        Absolute = 1
    };

    CommonMotionParameter commonParam;
    CoordinateSystem coordinateSystem = Increment;
    double velocityCruise = 500;

    std::string toString() const;
};

#endif // MOVEMOTIONPARAMETER
