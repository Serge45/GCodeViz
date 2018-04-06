#include "gcodeinterpreter.h"
#include <QRegExp>
#include <QStringList>
#include <QDebug>

GCodeInterpreter::GCodeInterpreter(const QString &content)
    : m_gcodeContent(content) {

    QString strippedResult;
    strippedResult.reserve(m_gcodeContent.size());

    QStringList contentLines = content.split(QRegExp("[\r\n]"), QString::SkipEmptyParts);

    for (QString &line : contentLines) {
        int commentStartIndex = -1;

        if ((commentStartIndex = line.indexOf(';')) >= 0) {
            line = line.mid(0, commentStartIndex);
        }
    }

    for (const QString &line : contentLines) {
        if (line.length()) {
            strippedResult.append(line);
            strippedResult.append("\r\n");
        }
    }

    qSwap(strippedResult, m_gcodeContent);
}

QList<GCodeCommand> GCodeInterpreter::interpret() const
{
    QRegExp regExp("[GXYZFRIJK][-+]?[0-9]*\\.?[0-9]*", Qt::CaseInsensitive);
    QStringList splittedResult;
    int pos = 0;

    while ((pos = regExp.indexIn(m_gcodeContent, pos)) != -1) {
        splittedResult << regExp.cap();
        pos += regExp.matchedLength();
    }

    QList<GCodeCommand> result;
    result.reserve(splittedResult.size() / 2);
    QString commandStr;
    bool xyzUsed[3] = {false, false, false};
    QString lastCode = "G00";

    foreach (const QString &s, splittedResult) {
        auto firstChar = s[0].toUpper();

        if (firstChar == QChar('G')) {
            lastCode = s;

            if (commandStr.size()) {
                result << GCodeCommand(commandStr);
                commandStr.clear();
                xyzUsed[0] = xyzUsed[1] = xyzUsed[2] = false;
            }

        } else if (firstChar == QChar('X')) {

            if (xyzUsed[0]) {
                result << GCodeCommand(commandStr);
                commandStr.clear();
                commandStr.append(lastCode);
                xyzUsed[0] = xyzUsed[1] = xyzUsed[2] = false;
            }
            xyzUsed[0] = true;

        } else if (firstChar == QChar('Y')) {

            if (xyzUsed[1]) {
                result << GCodeCommand(commandStr);
                commandStr.clear();
                commandStr.append(lastCode);
                xyzUsed[0] = xyzUsed[1] = xyzUsed[2] = false;
            }
            xyzUsed[1] = true;

        } else if (firstChar == QChar('Z')) {

            if (xyzUsed[2]) {
                result << GCodeCommand(commandStr);
                commandStr.clear();
                commandStr.append(lastCode);
                xyzUsed[0] = xyzUsed[1] = xyzUsed[2] = false;
            }
            xyzUsed[2] = true;

        }

        commandStr.append(s);

    }
    result << GCodeCommand(commandStr);

    return result;
}
