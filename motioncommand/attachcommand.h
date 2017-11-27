#ifndef ATTACHCOMMAND_H
#define ATTACHCOMMAND_H
#include "kmcommand.h"

class AttachCommand : public KMCommand
{
public:
    enum : uint32_t {
        Type = KMCommand::AttachCommand
    };
public:
    AttachCommand(const std::string &targetName);

public:
    std::string toStdString() const;
    uint32_t type() const {return Type;}

private:
    std::string m_targetName;

};

#endif // ATTACHCOMMAND_H
