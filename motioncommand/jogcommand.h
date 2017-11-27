#ifndef JOGCOMMAND_H
#define JOGCOMMAND_H

#include <string>
#include "kmcommand.h"

class JogCommand : public KMCommand
{
public:
    enum {
        Type = KMCommand::JogCommand
    };
public:
    JogCommand(const std::string &axisName,
               double velocity,
               double timeJog = -1,
               const CommonMotionParameter *overrideParam = nullptr);

public:
    std::string toStdString() const;
    uint32_t type() const {return Type;}

private:
    std::string m_axisName;
    double m_velocity;
    double m_timeJog;
    const CommonMotionParameter *m_overrideCommonParam;
};

#endif // JOGCOMMAND_H
