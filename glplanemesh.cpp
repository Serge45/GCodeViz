#include "glplanemesh.h"

GLPlaneMesh::GLPlaneMesh(GLfloat maxRange, GLPlaneMesh::Plane plane)
    : GLDrawable(), m_xInterval(1), m_yInterval(1), m_zInterval(1),
    m_maxRange(maxRange), m_plane(plane) {

    setMaxRange(m_maxRange);
    setupMesh();
}

void GLPlaneMesh::setMaxRange(GLfloat maxRange)
{
    m_maxRange = static_cast<int>(maxRange + 0.5f);
    setupMesh();
}

GLenum GLPlaneMesh::polygonType() const
{
    return GL_LINES;
}

void GLPlaneMesh::setupMesh()
{
    m_points.clear();

    if (m_plane == PlaneXY) {

        for (GLfloat x = -m_maxRange; x <= m_maxRange; x += m_xInterval) {
            m_points.push_back(GLfloat3({x, -m_maxRange, 0}));
            m_points.push_back(GLfloat3({x, m_maxRange, 0}));
        }

        for (GLfloat y = -m_maxRange; y <= m_maxRange; y += m_yInterval) {
            m_points.push_back(GLfloat3({-m_maxRange, y, 0}));
            m_points.push_back(GLfloat3({m_maxRange, y, 0}));
        }
    }
}

