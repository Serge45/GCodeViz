#ifndef GLDRAWABLE_H
#define GLDRAWABLE_H

#ifdef __APPLE__
#include "OpenGL/gl.h"
#else
#include "GL/gl.h"
#endif

#include <vector>

class GLDrawable
{
public:
    struct GLfloat3 {
        GLfloat x;
        GLfloat y;
        GLfloat z;
    };
public:
    GLDrawable();
    virtual ~GLDrawable();

public:
    const std::vector<GLfloat3> &points() const;
    std::vector<GLuint> &indexBuffer();
    virtual bool hasIndexBuffer() const;
    virtual GLenum polygonType() const;

protected:
    std::vector<GLfloat3> m_points;
    std::vector<GLuint> m_indices;
};

#endif // GLDRAWABLE_H
