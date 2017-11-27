#ifndef GCODEVISUALIZER_H
#define GCODEVISUALIZER_H

#include <QVector3D>
#include <QList>
#include <QPair>
#include "gcodecommand.h"
#include "motioncommand/kmcommand.h"

class GCodeVisualizer
{
public:
    GCodeVisualizer();

public:
    QList<QVector3D> visualize(const QList<GCodeCommand> &list,
                               QList<QPair<int, int> > &segmentInfo) const;
    QList<KMCommand *> toKMMotionCommands(const QList<GCodeCommand> &list,
                                        const QString &groupName) const;

private:
    enum CoordinateSystem {
        Increment = 0,
        Absolute,
    };

    QVector3D visualizeAsMotionGCode(const GCodeCommand &s,
                                     const QVector3D &prevPt,
                                     QList<QVector3D> &result,
                                     CoordinateSystem coordSystem = Absolute) const;
};

#endif // GCODEVISUALIZER_H
