#ifndef ENDCOMMAND_H
#define ENDCOMMAND_H

#include "kmcommand.h"

class EndCommand : public KMCommand
{
public:
    enum : uint32_t {
        Type = KMCommand::EndCommand
    };

public:
    EndCommand();

public:
    std::string toStdString() const;
    uint32_t type() const {return Type;}
};

#endif // ENDCOMMAND_H
