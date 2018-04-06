#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QPair>
#include <QVector3D>
#include <QFuture>
#include "interpreter/gcodecommand.h"
#include "memory/memorymonitorobject.h"

namespace Ui {
class MainWindow;
}

class QProgressBar;
class QLabel;
class QListWidgetItem;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void initStatusBar();

signals:
    void addPointToTraceView(const QVector3D &pt, int traceIdx);

private slots:
    void on_actionOpen_triggered();
    void on_actionSaveMotionCommands_triggered();
    void onMemoryMonitorUpdated(size_t curUsage, size_t maxUsage);

private:
    void updateGCodeView(const QString &gcodes);
    void updateTraceView(const QList<GCodeCommand> &list);

private:
    Ui::MainWindow *ui;
    QList<GCodeCommand> m_lastUsedCommandList;
    MemoryMonitorObject m_memPollingObject;
    QFuture<void> m_memoryMonitorFuture;
    QProgressBar *m_memoryUsageProgressBar;
    QLabel *m_memoryUsageLabel;
    QList<QPair<int, int> > m_segmentList;
};

#endif // MAINWINDOW_H
