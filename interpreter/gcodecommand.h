#ifndef GCODECOMMAND_H
#define GCODECOMMAND_H

#include <cstdint>
#include <QString>
#include <QVector3D>

class GCodeCommand
{
public:
    enum CodeType : uint8_t {
        G00 = 0,
        G01 = 1,
        G02 = 2,
        G03 = 3,
        G17 = 17,/*XY*/
        G18 = 18,/*ZX*/
        G19 = 19,/*YZ*/
        G90 = 90,
        G91 = 91
    };
public:
    explicit GCodeCommand(const QString &rawStr);

public:
    CodeType code() const {return m_code;}
    qreal radius() const {return m_raduis;}
    qreal fillRate() const {return m_fillRate;}
    const QVector3D &center() const {return m_center;}
    const QVector3D &vector() const {return m_vector;}
    const QString &toString() const {return m_rawString;}
    bool xChanged() const {return m_usedCoord[0];}
    bool yChanged() const {return m_usedCoord[1];}
    bool zChanged() const {return m_usedCoord[2];}
    bool overrideFeedRate() const {return m_overrideFeedRate;}

protected:
    void parse();

private:
    QVector3D mapToPlane(const QVector3D &pt,
                         CodeType dstType) const;

private:
    CodeType m_code;
    QString m_rawString;
    QVector3D m_vector;
    QVector3D m_center;
    qreal m_fillRate;
    qreal m_raduis;
    bool m_usedCoord[3];
    bool m_overrideFeedRate;
};

#endif // GCODECOMMAND_H
