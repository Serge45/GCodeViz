#include "endcommand.h"
#include <sstream>

EndCommand::EndCommand() {

}

std::string EndCommand::toStdString() const {
    std::stringstream cmd;
    cmd << "End Program"
        << std::endl;
    return cmd.str();
}

