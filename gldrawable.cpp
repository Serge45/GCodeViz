#include "gldrawable.h"

GLDrawable::GLDrawable() {

}

GLDrawable::~GLDrawable() {

}

const std::vector<GLDrawable::GLfloat3> &GLDrawable::points() const {
    return m_points;
}

std::vector<GLuint> &GLDrawable::indexBuffer() {
    return m_indices;
}

bool GLDrawable::hasIndexBuffer() const {
    return false;
}

GLenum GLDrawable::polygonType() const {
    return GL_LINE_STRIP;
}
