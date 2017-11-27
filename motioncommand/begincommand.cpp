#include "begincommand.h"
#include <sstream>

BeginCommand::BeginCommand() {

}

std::string BeginCommand::toStdString() const {
    std::stringstream cmd;
    cmd << "Program"
        << std::endl;
    return cmd.str();
}

