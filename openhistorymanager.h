#ifndef OPENHISTORYMANAGER_H
#define OPENHISTORYMANAGER_H

#include <QObject>

class QActionGroup;

class OpenHistoryManager : public QObject
{
    Q_OBJECT
public:
    explicit OpenHistoryManager(QObject *parent = 0);

    void addNewFilePath(const QString &path);
    QActionGroup *filePathActions();

signals:

private slots:
    void clear();

private:
    QActionGroup *m_historyFileActions;
};

#endif // OPENHISTORYMANAGER_H
