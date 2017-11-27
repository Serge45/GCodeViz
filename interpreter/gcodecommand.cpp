#include "gcodecommand.h"
#include <QRegExp>

GCodeCommand::GCodeCommand(const QString &rawStr)
    : m_code(G00),
      m_rawString(rawStr), m_fillRate(100), m_raduis(0),
      m_overrideFeedRate(false)
{
    for (auto &i : m_usedCoord) {
        i = false;
    }

    parse();
}

void GCodeCommand::parse()
{
    QRegExp regExp("[GXYZFRIJK][-+]?[0-9]*\\.?[0-9]*", Qt::CaseInsensitive);

    int pos = 0;

    while ((pos = regExp.indexIn(m_rawString, pos)) != -1) {
        auto str = regExp.cap();
        switch (str.at(0).toUpper().toLatin1()) {

        case 'G':
            m_code = static_cast<CodeType>(QString(str.data() + 1).toUInt());
            break;
        case 'I':
            m_center.setX(QString(str.data() + 1).toDouble());
            m_usedCoord[0] = true;
            break;
        case 'X':
            m_vector.setX(QString(str.data() + 1).toDouble());
            m_usedCoord[0] = true;
            break;
        case 'J':
            m_center.setY(QString(str.data() + 1).toDouble());
            m_usedCoord[1] = true;
            break;
        case 'Y':
            m_vector.setY(QString(str.data() + 1).toDouble());
            m_usedCoord[1] = true;
            break;
        case 'K':
            m_center.setZ(QString(str.data() + 1).toDouble());
            m_usedCoord[2] = true;
            break;
        case 'Z':
            m_vector.setZ(QString(str.data() + 1).toDouble());
            m_usedCoord[2] = true;
            break;
        case 'F':
            m_fillRate = QString(str.data() + 1).toDouble();
            m_overrideFeedRate = true;
            break;
        case 'R':
            m_raduis = QString(str.data() + 1).toDouble();
            break;
        default:
            break;
        }
        pos += regExp.matchedLength();
    }
}

QVector3D GCodeCommand::mapToPlane(const QVector3D &pt,
                                   GCodeCommand::CodeType dstType) const
{
    if (dstType == G17) {
        return pt;
    }

    QVector3D dstPt;

    if (dstType == G18) {
        dstPt.setZ(pt.x());
        dstPt.setX(pt.y());
    } else if (dstType == G19) {
        dstPt.setY(pt.x());
        dstPt.setZ(pt.y());
    } else {
        Q_ASSERT(false);
    }

    return dstPt;
}
