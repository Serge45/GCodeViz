#include "gcodevisualizer.h"
#include <cmath>
#include <QVector2D>
#include "motioncommand/motioncommand.h"

namespace {
    QVector2D getCircleCenter(const QVector2D &p1,
                              const QVector2D &p2,
                              qreal r,
                              bool cw = true) {
        QVector3D middle = (p1 + p2) / 2;
        auto diff = p1 - p2;
        auto q = diff.length();

        QList<QVector2D> result;
        QVector2D center;
        auto p = sqrt(r * r - (q / 2) * (q / 2));
        center.setX(middle.x() + p * diff.y() / q);
        center.setY(middle.y() + p * (-diff.x()) / q);
        result.push_back(center);

        center.setX(middle.x() - p * diff.y() / q);
        center.setY(middle.y() - p * (-diff.x()) / q);
        result.push_back(center);

        if (r < 0) {
            cw = !cw;
        }

        if (cw) {
            return result[1];
        }

        return result[0];
    }
}

GCodeVisualizer::GCodeVisualizer()
{

}

QList<QVector3D> GCodeVisualizer::visualize(const QList<GCodeCommand> &list,
                                            QList<QPair<int, int> > &segmentInfo) const
{

    QList<QVector3D> result;
    QVector3D prevPt;
    CoordinateSystem coordSystem = Absolute;
    segmentInfo.clear();
    QPair<int, int> segInfoElement;

    foreach (auto &s, list) {
        segInfoElement.first = qMax<int>(result.size() - 1, 0);
        switch (s.code()) {
        case GCodeCommand::G00:
        case GCodeCommand::G01:
        case GCodeCommand::G02:
        case GCodeCommand::G03:
            prevPt = visualizeAsMotionGCode(s, prevPt, result, coordSystem);
            break;
        case GCodeCommand::G90:
            coordSystem = Absolute;
            break;
        case GCodeCommand::G91:
            coordSystem = Increment;
            break;
        default:
            break;
        }
        segInfoElement.second = qMax<int>(result.size() - 1, segInfoElement.first);
        segmentInfo.push_back(segInfoElement);
    }
    return result;
}

QList<KMCommand *> GCodeVisualizer::toKMMotionCommands(const QList<GCodeCommand> &list,
                                                     const QString &groupName) const
{
    QList<KMCommand *> result;
    QVector3D prevPt;
    QVector3D curPt;
    std::string groupNameStr = groupName.toStdString();
    result.push_back(new BeginCommand());
    result.push_back(new AttachCommand(groupNameStr));
    MoveMotionParameter param;
    param.coordinateSystem = MoveMotionParameter::Absolute;
    param.velocityCruise = 100;

    foreach (auto &s, list) {
        if (s.code() == GCodeCommand::G90) {
            param.coordinateSystem = MoveMotionParameter::Absolute;
        } else if (s.code() == GCodeCommand::G91) {
            param.coordinateSystem = MoveMotionParameter::Increment;
        } else if (s.code() <= GCodeCommand::G03) {
            curPt = s.vector();

            if (!s.xChanged()) {
                curPt.setX(prevPt.x());
            }

            if (!s.yChanged()) {
                curPt.setY(prevPt.y());
            }

            if (!s.zChanged()) {
                curPt.setZ(prevPt.z());
            }

            if (s.overrideFeedRate()) {
                param.velocityCruise = s.fillRate();
            }

            if (s.code() <= GCodeCommand::G01) {
                result.push_back(new MoveCommand(groupNameStr,
                                                 3,
                                                 {curPt.x(), curPt.y(), curPt.z()},
                                                 0,
                                                 &param));
            } else if (s.code() == GCodeCommand::G02 ||
                       s.code() == GCodeCommand::G03) {

                QVector2D center;
                auto p1 = prevPt.toVector2D();
                auto p2 = curPt.toVector2D();
                qreal radius = 0;

                if (s.radius()) {
                    radius = s.radius();
                    center = getCircleCenter(p1,
                                             p2,
                                             s.radius(),
                                             s.code() == GCodeCommand::G02);
                } else {
                    center = (prevPt + s.center()).toVector2D();
                    radius = s.center().length();
                }

                QVector2D direction = p1 - p2;

                if (s.code() == GCodeCommand::G03) {
                    direction *= -1;
                }

                direction = QVector2D(direction.y(), -direction.x()).normalized();
                QVector2D middle = center + radius * direction;

                result.push_back(new CircleCommand(groupNameStr,
                                                   3,
                                                   {middle.x(), middle.y(), 0},
                                                   {s.vector().x(), s.vector().y(), s.vector().z()},
                                                   &param));
            }

            prevPt = curPt;
        }
    }
    result.push_back(new DetachCommand(groupNameStr));
    result.push_back(new EndCommand());
    return result;
}

QVector3D GCodeVisualizer::visualizeAsMotionGCode(const GCodeCommand &s,
                                                  const QVector3D &prevPt,
                                                  QList<QVector3D> &result,
                                                  CoordinateSystem coordSystem) const
{
    auto curPt = s.vector();

    if (coordSystem == Absolute) {

        if (!s.xChanged()) {
            curPt.setX(prevPt.x());
        }

        if (!s.yChanged()) {
            curPt.setY(prevPt.y());
        }

        if (!s.zChanged()) {
            curPt.setZ(prevPt.z());
        }

    } else if (coordSystem == Increment) {
        curPt += prevPt;
    }

    if (s.code() <= GCodeCommand::G01) {
        result.push_back(curPt);
    } else if (s.code() == GCodeCommand::G02 ||
               s.code() == GCodeCommand::G03) {

        QVector2D center;
        auto p1 = prevPt.toVector2D();
        auto p2 = curPt.toVector2D();

        if (s.radius()) {
            center = getCircleCenter(p1,
                                     p2,
                                     s.radius(),
                                     s.code() == GCodeCommand::G02);
        } else {
            center = (prevPt + s.center()).toVector2D();
            auto d1 = (prevPt - center).length();
            auto d2 = (curPt - center).length();
            Q_ASSERT(fabs(d1 - d2) < 1e-3);
        }

        auto p = p1;

        do {
            auto tangent = p - center;

            if (s.code() == GCodeCommand::G03) {
                tangent *= -1;
            }

            tangent = 0.01 * QVector2D(tangent.y(), -tangent.x()).normalized();
            p += tangent;
            result.push_back(p);
        } while ((p - p2).length() > 0.05);

        result.push_back(curPt);
    }
    return curPt;
}
