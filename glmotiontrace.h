#ifndef GLMOTIONTRACE_H
#define GLMOTIONTRACE_H

#include "gldrawable.h"

class GLMotionTrace : public GLDrawable
{
public:
    GLMotionTrace();

public:
    void clear();
    void addPoint(const GLfloat3 &pt);
    GLfloat3 maxRanges() const;
    GLfloat maxRange() const;

private:
    GLfloat m_addThreshold;
    GLfloat3 m_absMax;
};

#endif // GLMOTIONTRACE_H
