#include "movemotionparameter.h"
#include <sstream>

std::string MoveMotionParameter::toString() const {
    std::stringstream cmd;
    cmd << commonParam.toStdString()
        << " VCruise="
        << velocityCruise
        << " Abs="
        << coordinateSystem;
    return cmd.str();
}
