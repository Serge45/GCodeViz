#include "openhistorymanager.h"
#include <QFile>
#include <QActionGroup>
#include <QAction>

namespace detail {
    const int MAX_NUM_RECORDED_PATHS_IN_LIST = 10;
}

OpenHistoryManager::OpenHistoryManager(QObject *parent)
    : QObject(parent),
      m_historyFileActions(new QActionGroup(this))
{

}

void OpenHistoryManager::addNewFilePath(const QString &path)
{
    QAction *action = new QAction(path, this);
    action->setVisible(true);
    action->setEnabled(QFile(path).exists());
    m_historyFileActions->addAction(action);

    auto actions = m_historyFileActions->actions();

    if (actions.size() > detail::MAX_NUM_RECORDED_PATHS_IN_LIST) {
        m_historyFileActions->removeAction(actions.first());
    }
}

QActionGroup *OpenHistoryManager::filePathActions()
{
    return m_historyFileActions;
}

void OpenHistoryManager::clear()
{
    for (QAction *action : m_historyFileActions->actions()) {
        m_historyFileActions->removeAction(action);
    }
}
