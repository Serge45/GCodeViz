#ifndef GCODEINTERPRETER_H
#define GCODEINTERPRETER_H

#include <QList>
#include <QString>
#include "gcodecommand.h"

class GCodeInterpreter
{
public:
    explicit GCodeInterpreter(const QString &content);

public:
    QList<GCodeCommand> interpret() const;

private:
    QString m_gcodeContent;
};

#endif // GCODEINTERPRETER_H
