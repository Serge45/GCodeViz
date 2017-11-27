#include "detachcommand.h"
#include <sstream>

DetachCommand::DetachCommand(const std::string &targetName)
    : m_targetName(targetName) {

}

std::string DetachCommand::toStdString() const {
    std::stringstream cmd;
    cmd << "Detach "
        << m_targetName
        << std::endl;
    return cmd.str();
}
