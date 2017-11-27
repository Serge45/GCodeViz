#include "attachcommand.h"
#include <sstream>

AttachCommand::AttachCommand(const std::string &targetName)
    : m_targetName(targetName) {

}

std::string AttachCommand::toStdString() const {
    std::stringstream cmd;
    cmd << "Attach "
        << m_targetName
        << std::endl;
    return cmd.str();
}
