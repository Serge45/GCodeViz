#include "circlecommand.h"
#include <sstream>
#include "movemotionparameter.h"
#include "vectorutils.h"

CircleCommand::CircleCommand(const std::string &target,
                             size_t axisCount,
                             double angle,
                             const std::vector<double> &center,
                             CircleCommand::CirclePlane plane,
                             const MoveMotionParameter *overrideParam)
    : m_target(target),
    m_axisCount(axisCount),
    m_angle(angle),
    m_moveType(CenterAndAngle),
    m_circlePlane(plane),
    m_center(center),
    m_overrideParam(nullptr) {

    if (overrideParam) {
        m_overrideParam = new MoveMotionParameter(*overrideParam);
    }
}

CircleCommand::CircleCommand(const std::string &target,
                             size_t axisCount,
                             const std::vector<double> &middlePoint,
                             const std::vector<double> &targetPoint,
                             const MoveMotionParameter *overrideParam)
    : m_target(target),
    m_axisCount(axisCount),
    m_angle(0),
    m_moveType(ThreePoints),
    m_circlePlane(XY),
    m_middlePoint(middlePoint),
    m_targetPoint(targetPoint),
    m_overrideParam(overrideParam) {

}

CircleCommand::~CircleCommand() {
}

std::string CircleCommand::toStdString() const {
    std::stringstream cmd;

    cmd << "Circle "
        << m_target;

    if (m_moveType == CenterAndAngle) {
        cmd << " Angle="
            << m_angle
            << " CircleCenter="
            << stdVectorToString(m_center)
            << " CirclePlane="
            << m_circlePlane;
    } else {
        cmd << " CirclePoint="
            << stdVectorToString(m_middlePoint)
            << " TargetPoint="
            << stdVectorToString(m_targetPoint);
    }

    if (m_overrideParam) {
        cmd << m_overrideParam->toString();
    }

    cmd << std::endl;
    return cmd.str();
}
