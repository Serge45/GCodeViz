#include "mainwindow.h"
#include "ui_mainwindow.h"
#ifdef __WIN32
#include <windows.h>
#define sleepMs(X) Sleep(X)
#endif
#include <QList>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QVector2D>
#include <QElapsedTimer>
#include <QProgressDialog>
#include <QProgressBar>
#include <QLabel>
#include <QDebug>
#include <QtConcurrentRun>
#include "interpreter/gcodeinterpreter.h"
#include "interpreter/gcodevisualizer.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_memoryUsageProgressBar(new QProgressBar(this)),
    m_memoryUsageLabel(new QLabel(this))
{
    qRegisterMetaType<size_t>("size_t");
    ui->setupUi(this);
    ui->splitter->setSizes(QList<int>()
                           << (width() * 5) / 6
                           << width() / 6);

    initStatusBar();

    connect(this, SIGNAL(addPointToTraceView(QVector3D,int)),
            ui->traceView, SLOT(addPointToTrace(QVector3D,int)));

    m_memoryMonitorFuture = QtConcurrent::run(&m_memPollingObject,
                                              &MemoryMonitorObject::startMonitor);
}

MainWindow::~MainWindow()
{
    m_memPollingObject.stopMonitor();
    m_memoryMonitorFuture.waitForFinished();
    delete ui;
}

void MainWindow::initStatusBar()
{
    ui->statusBar->insertPermanentWidget(0, m_memoryUsageLabel);
    ui->statusBar->insertPermanentWidget(1, m_memoryUsageProgressBar);
    m_memoryUsageProgressBar->setFormat("");
    m_memoryUsageProgressBar->setMaximumWidth(128);

    connect(&m_memPollingObject, SIGNAL(memoryUsageUpdated(size_t,size_t)),
            this, SLOT(onMemoryMonitorUpdated(size_t,size_t)));

    connect(&m_memPollingObject, SIGNAL(memoryUsageUpdated(QString)),
            m_memoryUsageLabel, SLOT(setText(QString)));
}

void MainWindow::on_actionOpen_triggered()
{
    auto path = QFileDialog::getOpenFileName(this,
                                             tr("Open G-Code File"),
                                             "",
                                             tr("Text files(*.txt *.nc *.ncc *.tap *gcode)"));

    if (path.size() > 2) {
        QFile f(path);

        if (f.open(QFile::ReadOnly | QFile::Truncate)) {
            QTextStream stream(&f);
            GCodeInterpreter interpreter(stream.readAll());
            QElapsedTimer timer;
            timer.start();
            m_lastUsedCommandList = interpreter.interpret();
            qDebug() << "Interpreter time cost: " << timer.elapsed() << " ms";
            updateGCodeListView(m_lastUsedCommandList);
            updateCommandListView(m_lastUsedCommandList);
            updateTraceView(m_lastUsedCommandList);
            f.close();
        }
    }
}

void MainWindow::on_actionSaveMotionCommands_triggered()
{
    auto path = QFileDialog::getSaveFileName(this,
                                             tr("Save As PRG File"),
                                             "",
                                             tr("*.PRG"));

    if (path.size() > 2) {
        QFile f(path);

        if (f.open(QFile::WriteOnly | QFile::Truncate)) {
            QTextStream stream(&f);

            for (int i = 0; i < ui->commandListWidget->count(); ++i) {
                stream << ui->commandListWidget->item(i)->text();
            }

            f.close();
        }
    }

}

void MainWindow::onMemoryMonitorUpdated(size_t curUsage, size_t maxUsage)
{
    if (maxUsage != static_cast<size_t>(m_memoryUsageProgressBar->maximum())) {
        m_memoryUsageProgressBar->setMaximum(maxUsage);
    }
    m_memoryUsageProgressBar->setValue(curUsage);
}

void MainWindow::on_gcodeListWidget_itemClicked(QListWidgetItem *item)
{
    int row = ui->gcodeListWidget->row(item);
    ui->traceView->setHighlightSegment(m_segmentList[row]);
    ui->traceView->update();
}

void MainWindow::updateGCodeListView(const QList<GCodeCommand> &list)
{
    ui->gcodeListWidget->clear();

    ui->gcodeListWidget->setUpdatesEnabled(false);

    foreach (auto &s, list) {
        ui->gcodeListWidget->addItem(s.toString());
    }
    ui->gcodeListWidget->setUpdatesEnabled(true);
}

void MainWindow::updateTraceView(const QList<GCodeCommand> &list)
{
    ui->traceView->clearAllTrace();

    auto viz = GCodeVisualizer().visualize(list, m_segmentList);

    QProgressDialog progressDialog(tr("Rendering..."),
                                   tr("Cancel"),
                                   0,
                                   viz.size(),
                                   this);

    progressDialog.setWindowModality(Qt::WindowModal);
    progressDialog.setAutoClose(true);
    progressDialog.show();

    for (int i = 0; i < viz.size(); ++i) {
        ui->traceView->addPointToTrace(viz[i], 0);
        progressDialog.setValue(i + 1);

        if (progressDialog.wasCanceled()) {
            break;
        }
    }

    progressDialog.setValue(viz.size() + 1);
}

void MainWindow::updateCommandListView(const QList<GCodeCommand> &gcodeCmdList)
{
    ui->commandListWidget->clear();
    auto cmdList = GCodeVisualizer().toKMMotionCommands(gcodeCmdList, "xyzSpace");

    ui->commandListWidget->setUpdatesEnabled(false);

    foreach (auto &c, cmdList) {
        ui->commandListWidget->addItem(c->toQString());
    }

    ui->commandListWidget->setUpdatesEnabled(true);

    for (int i = 0; i < cmdList.size(); ++i) {
        delete cmdList[i];
    }
}
