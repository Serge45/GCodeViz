#ifndef MOTIONTRACEVIEW_H
#define MOTIONTRACEVIEW_H
#include <array>
#include <vector>
#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>
#include <QOpenGLShader>
#include <QMatrix4x4>
#include <QPoint>
#include <QColor>
#include <QList>
#include <QMutex>
#include <QPair>
#include <QTimer>
#include "glaxis.h"
#include "glaxisarrow.h"
#include "glmotiontrace.h"
#include "glplanemesh.h"

class QGLShaderProgram;
class QGLShader;
class QAction;
class QActionGroup;
class GLDrawable;

class MotionTraceView : public QOpenGLWidget
{
    Q_OBJECT
public:
    explicit MotionTraceView(QWidget *parent);
    ~MotionTraceView() override;

public slots:
    void addPointToTrace(const QVector3D &pt, int traceIdx);
    void clearTrace(int traceIdx);
    void clearAllTrace();
    void setHighlightSegment(const QPair<int, int> &segment);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

private slots:
    void onCustomContextMenuRequested(const QPoint &pt);
    void onPlaneActionGroupTriggered(QAction *action);
    void onActionDrawMeshTriggered(bool onOff);
    void onActionRenderWithOptionsTriggered();

private:
    void initShaders();
    void initAxes();
    void initTraces();
    void initActions();
    void drawAxes();
    void drawTrace(int idx);
    void drawMesh();
    void drawHighlightSegment();
    QPointF calculateRotateDegrees(const QPointF &mousePos);
    void updateProjection();
    void updateModelViewProjection();
    void drawObject(GLDrawable *obj,
                    QOpenGLShaderProgram *drawer,
                    const QColor &color,
                    qreal beginPercentage = 0.,
                    qreal endPercentage = 1.);
    void rescale();
    void resetObjectDrawingPercentages();
    QPointF calculateShift(const QPointF &curPos, const QPointF &pressPos) const;

private:
    QMatrix4x4 m_modelViewProjection;
    QMatrix4x4 m_projection;
    QMatrix4x4 m_worldRotation;
    QMatrix4x4 m_modelView;
    QOpenGLShaderProgram *m_axisShaderProgram;
    QOpenGLShader *m_axisVertexShader;
    QOpenGLShader *m_axisFragmentShader;
    GLAxis m_axes[3];
    GLAxisArrow m_axisArrows[3];
    bool m_mouseLeftButtonPressed;
    std::array<qreal, 3> m_rotationDegrees;
    QColor m_xAxisColor;
    QColor m_yAxisColor;
    QColor m_zAxisColor;
    QColor m_meshColor;
    QPointF m_curMousePressedPos;
    QList<GLMotionTrace> m_traces;
    QColor m_traceColor;
    qreal m_viewScale;
    qreal m_aspectRatio;
    GLfloat m_displayRangeMax;
    QAction *m_actionPlaneXY;
    QAction *m_actionPlaneZY;
    QAction *m_actionPlaneXZ;
    QAction *m_actionOrigin;
    QAction *m_actionDrawMesh;
    QAction *m_actionRenderWithOptions;
    QActionGroup *m_planeActionGroup;
    QPointF m_projectionShift;
    QMutex m_rescaleMutex;
    GLPlaneMesh m_planeMeshXY;
    bool m_drawMesh;
    QPair<int, int> m_highlightSegment;
    QTimer m_updateTimer;
    QPair<qreal, qreal> m_objectDrawingPercentages;
};

#endif // MOTIONTRACEVIEW_H
