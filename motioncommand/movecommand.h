#ifndef MOVECOMMAND
#define MOVECOMMAND
#include "kmcommand.h"
#include <vector>
#include "movemotionparameter.h"

class MoveCommand : public KMCommand {

public:
    enum : uint32_t {
        Type = KMCommand::MoveCommand
    };

public:
    MoveCommand(const std::string &targetName,
                size_t axisCount,
                const std::vector<double> &finalPos,
                double finalVelocity = 0,
                const MoveMotionParameter *overrideParam = nullptr);
    ~MoveCommand();

    uint32_t type() const {return Type;}
    std::string toStdString() const;

private:
    std::string m_targetName;
    size_t m_axisCount;
    std::vector<double> m_finalPos;
    double m_finalVelocity;
    const MoveMotionParameter *m_overrideParam;
};

#endif // MOVECOMMAND
