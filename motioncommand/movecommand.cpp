#include "movecommand.h"
#include <sstream>
#include "vectorutils.h"

MoveCommand::MoveCommand(const std::string &targetName,
            size_t axisCount,
            const std::vector<double> &finalPos,
            double finalVelocity,
            const MoveMotionParameter *overrideParam)
    : m_targetName(targetName),
    m_axisCount(axisCount),
    m_finalPos(finalPos),
    m_finalVelocity(finalVelocity),
    m_overrideParam(nullptr) {

    if (overrideParam) {
        m_overrideParam = new MoveMotionParameter(*overrideParam);
    }
}

MoveCommand::~MoveCommand() {
    if (m_overrideParam) {
        delete m_overrideParam;
    }
}

std::string MoveCommand::toStdString() const {
    std::stringstream cmd;
    cmd << "Move "
        << m_targetName
        << " "
        << stdVectorToString(m_finalPos)
        << " VFinal="
        << m_finalVelocity;

    if (m_overrideParam) {
        cmd << m_overrideParam->toString();
    }
    cmd << std::endl;
    return cmd.str();
}
