#ifndef GLPLANEMESH_H
#define GLPLANEMESH_H

#include "gldrawable.h"

class GLPlaneMesh : public GLDrawable
{
public:
    enum Plane {
        PlaneXY = 0,
        PlaneYZ,
        PlaneXZ
    };
public:
    GLPlaneMesh(GLfloat maxRange, Plane plane);

public:
    void setMaxRange(GLfloat maxRange);
    GLenum polygonType() const;

private:
    void setupMesh();

private:
    GLfloat m_xInterval;
    GLfloat m_yInterval;
    GLfloat m_zInterval;
    GLfloat m_maxRange;
    GLfloat m_plane;
};

#endif // GLPLANEMESH_H
