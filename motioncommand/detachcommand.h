#ifndef DETACHCOMMAND_H
#define DETACHCOMMAND_H
#include "kmcommand.h"

class DetachCommand : public KMCommand
{
public:
    enum : uint32_t {
        Type = KMCommand::DetachCommand
    };
public:
    DetachCommand(const std::string &targetName);

public:
    std::string toStdString() const;
    uint32_t type() const {return Type;}

private:
    std::string m_targetName;
};

#endif // DETACHCOMMAND_H
