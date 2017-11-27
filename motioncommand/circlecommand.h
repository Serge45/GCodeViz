#ifndef CIRCLECOMMAND_H
#define CIRCLECOMMAND_H

#include "kmcommand.h"
#include <string>
#include <vector>

struct MoveMotionParameter;

class CircleCommand : public KMCommand
{
public:
    enum : uint32_t {
        Type = KMCommand::CircleCommand
    };

    enum MoveType : uint32_t {
        CenterAndAngle,
        ThreePoints
    };

    enum CirclePlane : uint32_t {
        XY = 0,
        XZ,
        YZ
    };

public:
    CircleCommand(const std::string &target,
                  size_t axisCount,
                  double angle,
                  const std::vector<double> &center,
                  CirclePlane plane,
                  const MoveMotionParameter *overrideParam = nullptr
                  );

    CircleCommand(const std::string &target,
                  size_t axisCount,
                  const std::vector<double> &middlePoint,
                  const std::vector<double> &targetPoint,
                  const MoveMotionParameter *overrideParam = nullptr
                  );

    ~CircleCommand();

public:
    uint32_t type() const {return Type;}
    std::string toStdString() const;

private:
    std::string m_target;
    size_t m_axisCount;
    double m_angle;
    MoveType m_moveType;
    CirclePlane m_circlePlane;
    std::vector<double> m_center;
    std::vector<double> m_middlePoint;
    std::vector<double> m_targetPoint;
    const MoveMotionParameter *m_overrideParam;
};

#endif // CIRCLECOMMAND_H
