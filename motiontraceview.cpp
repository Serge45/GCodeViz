#ifdef __APPLE__
#include "OpenGL/gl.h"
#include "OpenGL/glu.h"
#else
#include "GL/gl.h"
#include "GL/glu.h"
#endif
#include <cassert>
#include <QMouseEvent>
#include <QAction>
#include <QActionGroup>
#include <QMenu>
#include <QMutexLocker>
#include <QDebug>
#include <cmath>
#include "gldrawable.h"
#include "motiontraceview.h"

namespace {
GLfloat sqrt2 = static_cast<GLfloat>(sqrt(2.0));
}

MotionTraceView::MotionTraceView(QWidget *parent)
    : QOpenGLWidget(parent),
    m_axisShaderProgram(nullptr),
    m_axisVertexShader(nullptr),
    m_axisFragmentShader(nullptr),
    m_mouseLeftButtonPressed(false),
    m_viewScale(1),
    m_displayRangeMax(1),
    m_actionPlaneXY(new QAction(tr("X-Y Plane"), this)),
    m_actionPlaneZY(new QAction(tr("Z-Y Plane"), this)),
    m_actionPlaneXZ(new QAction(tr("X-Z Plane"), this)),
    m_actionOrigin(new QAction(tr("Origin"), this)),
    m_actionDrawMesh(new QAction(tr("Draw Mesh"), this)),
    m_planeActionGroup(new QActionGroup(this)),
    m_rescaleMutex(QMutex::Recursive),
    m_planeMeshXY(1, GLPlaneMesh::PlaneXY),
    m_drawMesh(true) {

    updateProjection();
    updateModelViewProjection();
    initAxes();
    initTraces();
    initActions();

    m_meshColor = QColor(255, 255, 255, 128);

    connect(this, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(onCustomContextMenuRequested(QPoint)));

    m_updateTimer.setInterval(30);
    m_updateTimer.setSingleShot(false);
    connect(&m_updateTimer, SIGNAL(timeout()), this, SLOT(update()));
    m_updateTimer.start();
}

MotionTraceView::~MotionTraceView() {
    m_updateTimer.stop();
}

void MotionTraceView::initializeGL() {
    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.f, 0.f, 0.f, 1.f);
    initShaders();
}

void MotionTraceView::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
    m_aspectRatio = static_cast<qreal>(w) / h;

    updateProjection();
    updateModelViewProjection();
}

void MotionTraceView::paintGL() {
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glClearColor(0.f, 0.f, 0.f, 1.f);
    drawAxes();
    drawTrace(0);
    drawHighlightSegment();

    if (m_drawMesh) {
        glEnable(GL_BLEND);
        drawMesh();
        glDisable(GL_BLEND);
    }
}

void MotionTraceView::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        m_curMousePressedPos = event->localPos();
        m_mouseLeftButtonPressed = true;
    }
    return QOpenGLWidget::mousePressEvent(event);
}

void MotionTraceView::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        m_mouseLeftButtonPressed = false;
        auto diffDeg = calculateRotateDegrees(event->localPos());
        m_worldRotation = QMatrix4x4();

        if (event->modifiers() == Qt::CTRL) {
            m_projectionShift += calculateShift(event->localPos(), m_curMousePressedPos);
        } else {
            m_rotationDegrees[0] += diffDeg.y();
            m_rotationDegrees[1] += diffDeg.x();
        }
        m_worldRotation.translate(m_projectionShift.x(), -m_projectionShift.y());
        m_worldRotation.rotate(m_rotationDegrees[0], 1, 0, 0);
        m_worldRotation.rotate(m_rotationDegrees[1], 0, 1, 0);
        m_curMousePressedPos = event->localPos();
        updateModelViewProjection();
    }
}

void MotionTraceView::mouseMoveEvent(QMouseEvent *event) {
    if (m_mouseLeftButtonPressed) {
        QMutexLocker locker(&m_rescaleMutex);
        QMatrix4x4 m;
        auto shift = m_projectionShift;

        if (event->modifiers() == Qt::CTRL) {
            shift += calculateShift(event->localPos(), m_curMousePressedPos);
            m.translate(shift.x(), -shift.y());
            m.rotate(m_rotationDegrees[0], 1, 0, 0);
            m.rotate(m_rotationDegrees[1], 0, 1, 0);
            m.rotate(m_rotationDegrees[2], 0, 0, 1);
        } else {
            m.translate(shift.x(), -shift.y());
            auto diff = calculateRotateDegrees(event->localPos());
            m.rotate(diff.y() + m_rotationDegrees[0], 1, 0, 0);
            m.rotate(diff.x() + m_rotationDegrees[1], 0, 1, 0);
            m.rotate(m_rotationDegrees[2], 0, 0, 1);
        }
        m_modelViewProjection = m_projection * m * m_modelView;
    }
}

void MotionTraceView::wheelEvent(QWheelEvent *event) {
    int numDeg = event->delta() / 8;
    int numStep = abs(numDeg) / 15;
    qreal s = numStep * 0.05;

    if (numDeg > 0) {
        m_viewScale -= s;
    } else {
        m_viewScale += s;
    }

    if (m_viewScale < 0.01) {
        m_viewScale = 0.01;
    }
    updateProjection();
    updateModelViewProjection();
    event->accept();
}

void MotionTraceView::onCustomContextMenuRequested(const QPoint &pt) {
    QMenu menu(this);
    menu.addActions(m_planeActionGroup->actions());
    menu.addAction(m_actionDrawMesh);
    menu.exec(mapToGlobal(pt));
}

void MotionTraceView::onPlaneActionGroupTriggered(QAction *action) {
    QMatrix4x4 rot;

    if (action != m_actionOrigin) {
        for (auto &i : m_rotationDegrees) {
            i = 0;
        }

        if (action == m_actionPlaneZY) {
            m_rotationDegrees[1] = 90;
        } else if (action == m_actionPlaneXZ) {
            m_rotationDegrees[0] = -90;
        }
    } else {
        m_projectionShift = QPointF();
    }
    rot.translate(m_projectionShift.x(), -m_projectionShift.y());
    rot.rotate(m_rotationDegrees[0], 1, 0, 0);
    rot.rotate(m_rotationDegrees[1], 0, 1, 0);

    m_worldRotation = rot;
    updateModelViewProjection();
}

void MotionTraceView::onActionDrawMeshTriggered(bool onOff) {
    m_drawMesh = onOff;
}

void MotionTraceView::addPointToTrace(const QVector3D &pt, int traceIdx) {
    GLDrawable::GLfloat3 glPt = {static_cast<GLfloat>(pt.x()),
                                 static_cast<GLfloat>(pt.y()),
                                 static_cast<GLfloat>(pt.z())};
    m_traces[traceIdx].addPoint(glPt);
    rescale();
}

void MotionTraceView::clearTrace(int traceIdx) {
    m_traces[traceIdx].clear();
    rescale();
}

void MotionTraceView::clearAllTrace() {
    m_highlightSegment.first = 0;
    m_highlightSegment.second = 0;

    for (int i = 0; i < m_traces.size(); ++i) {
        m_traces[i].clear();
    }
    rescale();
}

void MotionTraceView::setHighlightSegment(const QPair<int, int> &segment) {
    m_highlightSegment = segment;
}

void MotionTraceView::initShaders() {
    m_axisShaderProgram = new QOpenGLShaderProgram(this);
    m_axisVertexShader = new QOpenGLShader(QOpenGLShader::Vertex, this);
    m_axisFragmentShader = new QOpenGLShader(QOpenGLShader::Fragment, this);
    auto err = m_axisVertexShader->compileSourceFile(":/glsl/coordinateaxisshader.vert");
    err = m_axisFragmentShader->compileSourceFile(":/glsl/coordinateaxisfragmentshader.frag");
    err = m_axisShaderProgram->addShader(m_axisVertexShader);
    err = m_axisShaderProgram->addShader(m_axisFragmentShader);
    err = m_axisShaderProgram->link();
    assert(err);
}

void MotionTraceView::initAxes() {
    m_axes[0] = GLAxis(GLAxis::x);
    m_axes[1] = GLAxis(GLAxis::y);
    m_axes[2] = GLAxis(GLAxis::z);
    m_axisArrows[0] = GLAxisArrow(GLAxis::x);
    m_axisArrows[1] = GLAxisArrow(GLAxis::y);
    m_axisArrows[2] = GLAxisArrow(GLAxis::z);
    m_xAxisColor = Qt::red;
    m_yAxisColor = Qt::yellow;
    m_zAxisColor = Qt::blue;
}

void MotionTraceView::initTraces() {
    m_traces.append(GLMotionTrace());
    m_traceColor = Qt::green;
}

void MotionTraceView::initActions() {
    m_planeActionGroup->addAction(m_actionPlaneXY);
    m_planeActionGroup->addAction(m_actionPlaneZY);
    m_planeActionGroup->addAction(m_actionPlaneXZ);
    m_planeActionGroup->addAction(m_actionOrigin);

    connect(m_planeActionGroup, SIGNAL(triggered(QAction *)),
            this, SLOT(onPlaneActionGroupTriggered(QAction *)));

    connect(m_actionDrawMesh, SIGNAL(triggered(bool)),
            this, SLOT(onActionDrawMeshTriggered(bool)));

    m_actionDrawMesh->setCheckable(true);
    m_actionDrawMesh->setChecked(true);
}

void MotionTraceView::drawAxes() {
    drawObject(&m_axes[0], m_axisShaderProgram, m_xAxisColor);
    drawObject(&m_axes[1], m_axisShaderProgram, m_yAxisColor);
    drawObject(&m_axes[2], m_axisShaderProgram, m_zAxisColor);
    drawObject(&m_axisArrows[0], m_axisShaderProgram, m_xAxisColor);
    drawObject(&m_axisArrows[1], m_axisShaderProgram, m_yAxisColor);
    drawObject(&m_axisArrows[2], m_axisShaderProgram, m_zAxisColor);
}

void MotionTraceView::drawTrace(int idx) {
    assert(idx < m_traces.size() && "Index out of bound");
    drawObject(&m_traces[idx], m_axisShaderProgram, m_traceColor);
}

void MotionTraceView::drawMesh() {
    drawObject(&m_planeMeshXY, m_axisShaderProgram, m_meshColor);
}

void MotionTraceView::drawHighlightSegment() {

    if (m_traces[0].points().size() > 1) {
        glLineWidth(3.f);
        m_axisShaderProgram->bind();

        m_axisShaderProgram->enableAttributeArray("qt_Vertex");
        m_axisShaderProgram->setUniformValue("qt_ModelViewProjectionMatrix",
                                             m_modelViewProjection);

        m_axisShaderProgram->setUniformValue("qt_InColor", QColor(255, 0, 0));
        m_axisShaderProgram->setAttributeArray("qt_Vertex",
                                  &(m_traces[0].points()[m_highlightSegment.first].x),
                                  3);

        glDrawArrays(GL_LINE_STRIP,
                     0,
                     m_highlightSegment.second - m_highlightSegment.first + 1);

        m_axisShaderProgram->disableAttributeArray("qt_Vertex");

        m_axisShaderProgram->release();
        glLineWidth(1.f);
    }
}

QPointF MotionTraceView::calculateRotateDegrees(const QPointF &mousePos) {
    auto result = mousePos - m_curMousePressedPos;
    result.setX(360. * result.x() / width());
    result.setY(360. * result.y() / height());
    return result;
}

void MotionTraceView::updateProjection() {
    m_projection = QMatrix4x4();

    m_projection.ortho(m_viewScale * -m_aspectRatio * m_displayRangeMax,
                       m_viewScale * m_aspectRatio * m_displayRangeMax,
                       -m_viewScale * m_displayRangeMax,
                       m_viewScale  * m_displayRangeMax,
                       -m_displayRangeMax, m_displayRangeMax);
}

void MotionTraceView::updateModelViewProjection() {
    m_modelViewProjection = m_projection * m_worldRotation * m_modelView;
}

void MotionTraceView::drawObject(GLDrawable *obj, QOpenGLShaderProgram *drawer, const QColor &color) {
    drawer->bind();

    drawer->enableAttributeArray("qt_Vertex");
    drawer->setUniformValue("qt_ModelViewProjectionMatrix",
                                         m_modelViewProjection);

    drawer->setUniformValue("qt_InColor", color);
    drawer->setAttributeArray("qt_Vertex", &(obj->points().data()->x), 3);

    if (obj->hasIndexBuffer()) {
        glDrawElements(obj->polygonType(), obj->indexBuffer().size(),
                       GL_UNSIGNED_INT, obj->indexBuffer().data());
    } else {
        glDrawArrays(obj->polygonType(), 0, obj->points().size());
    }

    drawer->disableAttributeArray("qt_Vertex");
    drawer->release();
}

void MotionTraceView::rescale() {
    QMutexLocker locker(&m_rescaleMutex);
    GLfloat rangeMax = 1;

    if (m_traces.size()) {
        rangeMax = sqrt2 * m_traces[0].maxRange() + 1;
    }

    if (fabs(sqrt2 * rangeMax - m_displayRangeMax) > 1) {
        m_displayRangeMax = sqrt2 * rangeMax + 0.5;
        m_planeMeshXY.setMaxRange(m_displayRangeMax);
        updateProjection();
        updateModelViewProjection();
    }
}

QPointF MotionTraceView::calculateShift(const QPointF &curPos,
                                        const QPointF &pressPos) const {
    auto tmpShift = curPos - pressPos;
    tmpShift.setX((tmpShift.x() * 2 * m_traces[0].maxRange() * m_viewScale * m_aspectRatio) / width());
    tmpShift.setY((tmpShift.y() * 2 * m_traces[0].maxRange() * m_viewScale) / height());
    return tmpShift;
}
