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
#include <QSettings>
#include "interpreter/gcodeinterpreter.h"
#include "interpreter/gcodevisualizer.h"

namespace {
    const QString APP_NAME = "GCodeViz";
    const QString ORGANIZATION_NAME = "SP Studio";
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_memoryUsageProgressBar(new QProgressBar(this)),
    m_memoryUsageLabel(new QLabel(this)),
    m_openHistoryManager(new OpenHistoryManager(this))
{
    qRegisterMetaType<size_t>("size_t");
    ui->setupUi(this);
    ui->splitter->setSizes(QList<int>()
                           << (width() * 5) / 6
                           << width() / 6);

    ui->menuTraceView->addActions(ui->traceView->actions());

    initStatusBar();

    connect(this, SIGNAL(addPointToTraceView(QVector3D,int)),
            ui->traceView, SLOT(addPointToTrace(QVector3D,int)));

    m_memoryMonitorFuture = QtConcurrent::run(&m_memPollingObject,
                                              &MemoryMonitorObject::startMonitor);

    initOpenHistoryMenu();
    loadAppSettings();
}

MainWindow::~MainWindow()
{
    saveAppSettings();
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

void MainWindow::initOpenHistoryMenu()
{
    connect(m_openHistoryManager->filePathActions(), SIGNAL(triggered(QAction*)),
            this, SLOT(onOpenHistoryActionGroupTriggered(QAction*)));

    connect(ui->actionOpenHistoryClear, SIGNAL(triggered()),
            m_openHistoryManager, SLOT(clear()));
}

void MainWindow::on_actionOpen_triggered()
{
    auto path = QFileDialog::getOpenFileName(this,
                                             tr("Open G-Code File"),
                                             "",
                                             tr("Text files(*.txt *.nc *.ncc *.tap *.gcode)"));

    if (openGCodeFile(path)) {
        setWindowTitle(QString("%1(%2)").arg(APP_NAME).arg(path));
        updateOpenHistoryActions(path);
    }
}

void MainWindow::on_actionSaveMotionCommands_triggered()
{
    auto path = QFileDialog::getSaveFileName(this,
                                             tr("Save As PRG File"),
                                             "",
                                             tr("*.PRG"));

    if (path.size() > 2) {
        QList<KMCommand *> motionCommands = GCodeVisualizer().toKMMotionCommands(m_lastUsedCommandList, "xyzSpace");
        QFile f(path);

        if (f.open(QFile::WriteOnly | QFile::Truncate)) {
            QTextStream stream(&f);

            foreach (const KMCommand *command, motionCommands) {
                stream << command->toQString();
            }

            f.close();
        }

        foreach (const KMCommand *command, motionCommands) {
            delete command;
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

void MainWindow::onOpenHistoryActionGroupTriggered(QAction *action)
{
    QString path = action->text();

    if (openGCodeFile(path)) {
        setWindowTitle(QString("%1(%2)").arg(APP_NAME).arg(path));
        updateOpenHistoryActions(path);
    }
}

void MainWindow::on_actionOpenHistoryClear_triggered()
{
    for (QAction *a : ui->menuRecentOpened->actions()) {
        if (a != ui->actionOpenHistoryClear) {
            ui->menuRecentOpened->removeAction(a);
        }
    }
}

void MainWindow::updateGCodeView(const QString &gcodes)
{
    ui->gcodeView->clear();
    ui->gcodeView->insertPlainText(gcodes);
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

bool MainWindow::openGCodeFile(const QString &path)
{
    bool openOk = false;

    if (path.size() > 2) {
        QFile f(path);

        if (f.open(QFile::ReadOnly | QFile::Truncate)) {
            QTextStream stream(&f);
            QString gcodes = stream.readAll();
            GCodeInterpreter interpreter(gcodes);
            QElapsedTimer timer;
            timer.start();
            m_lastUsedCommandList = interpreter.interpret();
            qDebug() << "Interpreter time cost: " << timer.elapsed() << " ms";
            updateGCodeView(gcodes);
            updateTraceView(m_lastUsedCommandList);
            f.close();
            openOk = true;
        }
    }

    return openOk;
}

void MainWindow::updateOpenHistoryActions(const QString &newOpenPath)
{
    m_openHistoryManager->addNewFilePath(newOpenPath);

    for (QAction *a : ui->menuRecentOpened->actions()) {
        if (a != ui->actionOpenHistoryClear) {
            ui->menuRecentOpened->removeAction(a);
        }
    }

    auto currentActions = m_openHistoryManager->filePathActions()->actions();

    for (auto i = currentActions.rbegin(); i != currentActions.rend(); ++i) {
        ui->menuRecentOpened->insertAction(ui->actionOpenHistoryClear,
                                           *i);
    }
}

void MainWindow::saveAppSettings()
{
    QSettings settings(ORGANIZATION_NAME, APP_NAME, this);
    settings.beginGroup("mainWindow");
    settings.setValue("pos", pos());
    settings.setValue("size", size());
    settings.endGroup();

    settings.beginGroup("traceView");
    settings.setValue("traceColor", ui->traceView->traceColor().rgba());
    settings.endGroup();

    settings.beginWriteArray("openHistory");

    auto actions = m_openHistoryManager->filePathActions()->actions();

    for (int i = 0; i < actions.size(); ++i) {
        settings.setArrayIndex(i);
        settings.setValue("filePath", actions[i]->text());
    }

    settings.endArray();
}

void MainWindow::loadAppSettings()
{
    QSettings settings(ORGANIZATION_NAME, APP_NAME, this);
    settings.beginGroup("mainWindow");
    auto pos = settings.value("pos", QPoint(100, 100)).toPoint();
    auto size = settings.value("size", QSize(800, 600)).toSize();
    move(pos);
    resize(size);
    settings.endGroup();

    settings.beginGroup("traceView");
    auto traceColor = settings.value("traceColor", QRgb(Qt::green)).toUInt();
    ui->traceView->setTraceColor(QColor::fromRgba(traceColor));
    settings.endGroup();

    int numOpenFilePaths = settings.beginReadArray("openHistory");

    for (int i = 0; i < numOpenFilePaths; ++i) {
        settings.setArrayIndex(i);
        auto filePath = settings.value("filePath", QString()).toString();
        m_openHistoryManager->addNewFilePath(filePath);
    }

    auto currentActions = m_openHistoryManager->filePathActions()->actions();

    for (auto i = currentActions.rbegin(); i != currentActions.rend(); ++i) {
        ui->menuRecentOpened->insertAction(ui->actionOpenHistoryClear,
                                           *i);
    }

    settings.endArray();
}
