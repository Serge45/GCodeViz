#include "glaxis.h"

namespace {
    GLDrawable::GLfloat3 xyzAxisPointTable[3][2] = {{{0, 0, 0}, {0.5, 0, 0}},
                                                    {{0, 0, 0}, {0, 0.5, 0}},
                                                    {{0, 0, 0}, {0, 0, 0.5}}
                                                   };
}

GLAxis::GLAxis(Axis axis)
    : m_axis(axis), m_axisLength(0.5) {
    m_points = std::vector<GLfloat3>(xyzAxisPointTable[m_axis], xyzAxisPointTable[m_axis] + 2);
}

void GLAxis::setAxisLength(GLfloat len) {
    m_axisLength = len;

    if (m_axis == x) {
        m_points[1].x = len;
    } else if (m_axis == y) {
        m_points[1].y = len;
    } else {
        m_points[1].z = len;
    }
}
