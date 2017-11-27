#ifndef KMCOMMAND_H
#define KMCOMMAND_H

#include <string>
#include <cstdint>

#ifdef QT_CORE_LIB
#include <QString>
#endif

#include "commonmotionparameter.h"

class KMCommand
{
public:
    enum : uint32_t {
        BaseType = 65536,
        BeginCommand = BaseType + 1,
        EndCommand = BaseType + 2,
        AttachCommand = BaseType + 3,
        DetachCommand = BaseType + 4,
        JogCommand = BaseType + 5,
        MoveCommand = BaseType + 6,
        CircleCommand = BaseType + 7,
        UserType
    };

public:
    KMCommand();
    virtual ~KMCommand();

public:
    virtual std::string toStdString() const = 0;
    virtual uint32_t type() const = 0;

#ifdef QT_CORE_LIB
    QString toQString() const {return QString::fromStdString(toStdString());}
#endif
};

#endif // KMCOMMAND_H
