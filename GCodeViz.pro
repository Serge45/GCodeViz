#-------------------------------------------------
#
# Project created by QtCreator 2016-01-30T15:37:03
#
#-------------------------------------------------

QT       += core gui widgets concurrent
CONFIG += c++11

TARGET = GCodeViz
TEMPLATE = app

OBJECTS_DIR = ./obj
MOC_DIR = ./moc
RCC_DIR = ./rcc

INCLUDEPATH += include

win32 {
    LIBS += Opengl32.lib
    !contains(QMAKE_TARGET.arch, x86_64) {
        message("x86 build")
    } else {
        message("x86_64 build")
    }
}

SOURCES += main.cpp\
        mainwindow.cpp \
    interpreter/gcodecommand.cpp \
    interpreter/gcodeinterpreter.cpp \
    glaxis.cpp \
    glaxisarrow.cpp \
    gldrawable.cpp \
    glmotiontrace.cpp \
    motiontraceview.cpp \
    interpreter/gcodevisualizer.cpp \
    motioncommand/attachcommand.cpp \
    motioncommand/begincommand.cpp \
    motioncommand/circlecommand.cpp \
    motioncommand/commonmotionparameter.cpp \
    motioncommand/detachcommand.cpp \
    motioncommand/endcommand.cpp \
    motioncommand/jogcommand.cpp \
    motioncommand/kmcommand.cpp \
    motioncommand/movecommand.cpp \
    motioncommand/movemotionparameter.cpp \
    glplanemesh.cpp \
    memory/memorymonitorobject.cpp \
    tracedrawingrangedialog.cpp \
    openhistorymanager.cpp

HEADERS  += mainwindow.h \
    interpreter/gcodecommand.h \
    interpreter/gcodeinterpreter.h \
    glaxis.h \
    glaxisarrow.h \
    gldrawable.h \
    glmotiontrace.h \
    motiontraceview.h \
    interpreter/gcodevisualizer.h \
    motioncommand/attachcommand.h \
    motioncommand/begincommand.h \
    motioncommand/circlecommand.h \
    motioncommand/commonmotionparameter.h \
    motioncommand/detachcommand.h \
    motioncommand/endcommand.h \
    motioncommand/jogcommand.h \
    motioncommand/kmcommand.h \
    motioncommand/motioncommand.h \
    motioncommand/movecommand.h \
    motioncommand/movemotionparameter.h \
    motioncommand/vectorutils.h \
    glplanemesh.h \
    memory/memorymonitorobject.h \
    tracedrawingrangedialog.h \
    openhistorymanager.h

FORMS    += mainwindow.ui \
    tracedrawingrangedialog.ui

RESOURCES += \
    resources.qrc

DISTFILES +=
