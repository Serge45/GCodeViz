#ifndef BEGINCOMMAND_H
#define BEGINCOMMAND_H

#include "kmcommand.h"

class BeginCommand : public KMCommand
{
public:
    enum : uint32_t {
        Type = KMCommand::BeginCommand
    };

public:
    BeginCommand();
    uint32_t type() const {return Type;}

public:
    std::string toStdString() const;
};

#endif // BEGINCOMMAND_H
