#include "glaxisarrow.h"

namespace {
    GLuint axisArrowIndices[] = {0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 1};
    const GLfloat defaultAxisLength = 0.5;
    const GLfloat defaultArrowSize = defaultAxisLength / 7;
    const GLfloat defaultHalfArrowSize = defaultArrowSize / 2;

    GLDrawable::GLfloat3 xyzArrowPointTable[3][5] =
    {
        {
             {defaultAxisLength, 0, 0},
             {defaultAxisLength - defaultArrowSize, defaultHalfArrowSize, defaultHalfArrowSize},
             {defaultAxisLength - defaultArrowSize, -defaultHalfArrowSize, defaultHalfArrowSize},
             {defaultAxisLength - defaultArrowSize, -defaultHalfArrowSize, -defaultHalfArrowSize},
             {defaultAxisLength - defaultArrowSize, defaultHalfArrowSize, -defaultHalfArrowSize}
        },
        {
             {0, defaultAxisLength, 0},
             {-defaultHalfArrowSize, defaultAxisLength - defaultArrowSize, -defaultHalfArrowSize},
             {defaultHalfArrowSize, defaultAxisLength - defaultArrowSize, -defaultHalfArrowSize},
             {defaultHalfArrowSize, defaultAxisLength - defaultArrowSize, defaultHalfArrowSize},
             {-defaultHalfArrowSize, defaultAxisLength - defaultArrowSize, defaultHalfArrowSize}
        },
        {
             {0, 0, defaultAxisLength},
             {-defaultHalfArrowSize, defaultHalfArrowSize, defaultAxisLength - defaultArrowSize},
             {-defaultHalfArrowSize, -defaultHalfArrowSize, defaultAxisLength - defaultArrowSize},
             {defaultHalfArrowSize, -defaultHalfArrowSize, defaultAxisLength - defaultArrowSize},
             {defaultHalfArrowSize, defaultHalfArrowSize, defaultAxisLength - defaultArrowSize}
        }
    };
}

GLAxisArrow::GLAxisArrow(GLAxis::Axis axis)
    : m_axis(axis) {
    auto idxCount = sizeof(axisArrowIndices) / sizeof(axisArrowIndices[0]);
    m_indices = std::vector<GLuint>(axisArrowIndices, axisArrowIndices + idxCount);
    m_points = std::vector<GLfloat3>(xyzArrowPointTable[m_axis],
                                     xyzArrowPointTable[m_axis] + 5);
}

bool GLAxisArrow::hasIndexBuffer() const {
    return true;
}

GLenum GLAxisArrow::polygonType() const {
    return GL_TRIANGLES;
}

