#include "jogcommand.h"
#include <sstream>

JogCommand::JogCommand(const std::string &axisName, double velocity, double timeJog,
                       const CommonMotionParameter *overrideParam)
    : m_axisName(axisName),
      m_velocity(velocity),
      m_timeJog(timeJog),
      m_overrideCommonParam(overrideParam) {
}

std::string JogCommand::toStdString() const {
    std::stringstream cmd;

    cmd << "Jog "
        << m_axisName
        << " "
        << m_velocity
        << " "
        << "TimeJog="
        << m_timeJog;

    if (m_overrideCommonParam) {
        cmd << m_overrideCommonParam->toStdString();
    }
    cmd << std::endl;

    return cmd.str();
}
