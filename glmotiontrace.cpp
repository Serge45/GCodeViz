#include "glmotiontrace.h"
#include <cmath>

namespace {
    GLfloat distance2(const GLDrawable::GLfloat3 &p,
                      const GLDrawable::GLfloat3 &q) {
        auto diffX = p.x - q.x;
        auto diffY = p.y - q.y;
        auto diffZ = p.z - q.z;

        return (diffX * diffX + diffY * diffY + diffZ * diffZ);
    }
}

GLMotionTrace::GLMotionTrace()
    : m_addThreshold(1e-5), m_absMax({1, 1, 1}) {

}

void GLMotionTrace::clear() {
    m_points.clear();
    m_absMax.x = 1;
    m_absMax.y = 1;
    m_absMax.z = 1;
}

void GLMotionTrace::addPoint(const GLDrawable::GLfloat3 &pt) {
    if (!m_points.size() || distance2(pt, m_points.back()) > m_addThreshold) {
        m_points.push_back(pt);
        m_absMax.x = std::max<GLfloat>(m_absMax.x, fabs(pt.x));
        m_absMax.y = std::max<GLfloat>(m_absMax.y, fabs(pt.y));
        m_absMax.z = std::max<GLfloat>(m_absMax.z, fabs(pt.z));
    }
}

GLDrawable::GLfloat3 GLMotionTrace::maxRanges() const {
    return m_absMax;
}

GLfloat GLMotionTrace::maxRange() const {
    return std::max<GLfloat>(std::max<GLfloat>(m_absMax.x, m_absMax.y),
                             m_absMax.z);
}
