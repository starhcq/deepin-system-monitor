/*
* Copyright (C) 2019 ~ 2020 Uniontech Software Technology Co.,Ltd
*
* Author:      maojj <maojunjie@uniontech.com>
* Maintainer:  maojj <maojunjie@uniontech.com>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef APPLICATION_H
#define APPLICATION_H

#include <DApplication>

#include "gui/main_window.h"

DCORE_USE_NAMESPACE
DWIDGET_USE_NAMESPACE

#if defined(gApp)
#undef gApp
#endif
#define gApp (qobject_cast<Application *>(Application::instance()))

class MainWindow;

static const QEvent::Type kMonitorStartEventType = static_cast<QEvent::Type>(QEvent::User + 1);
static const QEvent::Type kNetifStartEventType = static_cast<QEvent::Type>(QEvent::User + 2);
class MonitorStartEvent : public QEvent
{
public:
    explicit MonitorStartEvent()
        : QEvent(kMonitorStartEventType)
    {
    }
    virtual ~MonitorStartEvent();
};

class NetifStartEvent : public QEvent
{
public:
    explicit NetifStartEvent()
        : QEvent(kNetifStartEventType)
    {
    }
    virtual ~NetifStartEvent();
};

class Application : public DApplication
{
    Q_OBJECT

public:
    Application(int &argc, char **argv);

    virtual ~Application() override;

    enum TaskState {
        kTaskStarted,
        kTaskRunning,
        kTaskFinished
    };
    Q_ENUM(Application::TaskState)

    void setMainWindow(MainWindow *mw);
    //!
    //! \brief raiseWindow 窗口置顶显示
    //!
    void raiseWindow();

    MainWindow *mainWindow();
    //!
    //! \brief getCurrentTaskState 获取当前窗口运行状态
    //! \return 返回当前窗口运行状态
    //!
    TaskState getCurrentTaskState();

    //!
    //! \brief setCurrentTaskState 设置当前窗口运行状态
    //! \param taskState 运行状态参数
    //!
    void setCurrentTaskState(TaskState taskState);

signals:
    void backgroundTaskStateChanged(Application::TaskState state);

protected:
    bool event(QEvent *event) override;

private:
    MainWindow *m_mainWindow {};

    //!
    //! \brief m_currentTaskStat 当前窗口运行状态
    //!
    TaskState m_currentTaskStat = TaskState::kTaskFinished;
};

inline void Application::setMainWindow(MainWindow *mw)
{
    m_mainWindow = mw;
}

inline void Application::raiseWindow()
{
    m_mainWindow->raiseWindow();
}

inline MainWindow *Application::mainWindow()
{
    return m_mainWindow;
}

#endif // APPLICATION_H