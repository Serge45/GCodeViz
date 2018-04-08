#include "memorymonitorobject.h"
#include <QApplication>
#include <QDebug>

#if defined(_WIN32)
#include <Windows.h>
#define sleepMs(X) Sleep(X)
#else
#include <unistd.h>
#define sleepMs(X) usleep(1000 * (X))
#endif

MemoryMonitorObject::MemoryMonitorObject(QObject *parent)
    : QObject(parent), m_isRunning(false),
      m_currentUsage(0), m_peakUsage(0), m_maximalUsage(0),
      m_updatePeriodMs(1000)
{
#ifdef _WIN32
    ZeroMemory(&m_memoryCounter, sizeof(m_memoryCounter));
    ZeroMemory(&m_memoryStatus, sizeof(m_memoryStatus));
#endif
    determineMaxmialUsage();
}

MemoryMonitorObject::~MemoryMonitorObject()
{
    stopMonitor();
}

void MemoryMonitorObject::startMonitor()
{
    determineMaxmialUsage();

    QString msg;

    m_isRunning = true;

    while (m_isRunning) {
        msg = QString("RAM usage: %1 MB").arg(pollMemoryUsage() / (1024. * 1024.));
        emit memoryUsageUpdated(msg);
        emit memoryUsageUpdated(m_currentUsage, m_maximalUsage);
        sleepMs(m_updatePeriodMs);
    }
}

void MemoryMonitorObject::stopMonitor()
{
    m_isRunning = false;
}

void MemoryMonitorObject::setUpdatePeriod(MemoryMonitorObject::UpdatePeriod period)
{
    m_updatePeriodMs = static_cast<size_t>(period);
}

size_t MemoryMonitorObject::pollMemoryUsage()
{
#ifdef _WIN32
    if (auto appHandle = GetCurrentProcess()) {

        if (GetProcessMemoryInfo(appHandle,
                                 &m_memoryCounter,
                                 sizeof(m_memoryCounter))) {
            m_peakUsage = m_memoryCounter.PeakWorkingSetSize;
            return m_currentUsage = static_cast<size_t>(m_memoryCounter.WorkingSetSize);
        }
    }
#endif
    return 0;
}

void MemoryMonitorObject::determineMaxmialUsage()
{
#ifdef _WIN32
    const size_t systemUpperBound = (~0x00) >> 1;

    GlobalMemoryStatus(&m_memoryStatus);
    m_maximalUsage = qMin<size_t>(m_memoryStatus.dwTotalVirtual,
                                  systemUpperBound);
#endif
}
