#ifndef MEMORYMONITOROBJECT_H
#define MEMORYMONITOROBJECT_H

#include <QObject>

#ifdef __WIN32
#include <windows.h>
#include <psapi.h>
#endif

class MemoryMonitorObject : public QObject
{
    Q_OBJECT

public:
    enum UpdatePeriod : size_t {
        Fast = 250,
        Normal = 1000,
        Slow = 2000
    };

public:
    explicit MemoryMonitorObject(QObject *parent = 0);
    ~MemoryMonitorObject() override;

public:
    void startMonitor();
    void stopMonitor();
    size_t maximalUsage() const {return m_maximalUsage;}
    size_t peakUsage() const {return m_peakUsage;}
    void setUpdatePeriod(UpdatePeriod period);

protected:
    virtual size_t pollMemoryUsage();
    virtual void determineMaxmialUsage();

signals:
    void memoryUsageUpdated(const QString &msg);
    void memoryUsageUpdated(size_t curUsage, size_t peakUsage);

private:
    bool m_isRunning;
    size_t m_currentUsage;
    size_t m_peakUsage;
    size_t m_maximalUsage;
    size_t m_updatePeriodMs;

#ifdef __WIN32
    PROCESS_MEMORY_COUNTERS m_memoryCounter;
    MEMORYSTATUS m_memoryStatus;
#endif

};

#endif // MEMORYMONITOROBJECT_H
