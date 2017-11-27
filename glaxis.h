#ifndef GLAXIS_H
#define GLAXIS_H
#include "gldrawable.h"


class GLAxis : public GLDrawable
{
public:
    enum Axis {
        x = 0,
        y,
        z
    };

public:
    GLAxis(Axis axis = x);
    GLfloat axisLength() const {return m_axisLength;}
    void setAxisLength(GLfloat len);

private:
    Axis m_axis;
    GLfloat m_axisLength;
};

#endif // GLAXIS_H
