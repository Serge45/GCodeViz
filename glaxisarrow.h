#ifndef GLAXISARROW_H
#define GLAXISARROW_H

#include "gldrawable.h"
#include "glaxis.h"

class GLAxisArrow : public GLDrawable
{
public:
    explicit GLAxisArrow(GLAxis::Axis axis = GLAxis::x);

public:
    bool hasIndexBuffer() const;
    GLenum polygonType() const;

private:
    GLAxis::Axis m_axis;
};

#endif // GLAXISARROW_H
