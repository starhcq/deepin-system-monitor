/*
* Copyright (C) 2019 ~ 2021 Uniontech Software Technology Co.,Ltd
*
* Author:      wangchao <wangchao@uniontech.com>
* Maintainer:  wangchao <wangchao@uniontech.com>
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

#include "common/error_context.h"
#include "process/process_controller.h"
#include "application.h"

#include <QFile>
#include <QProcess>
#include <QObject>

#include "ut_process_controller.h"

Ut_ProcessController::Ut_ProcessController()
{
}

TEST(UT_ProcessController_ProcessController, UT_ProcessController_ProcessController_001)
{
    pid_t testpid = 123;
    int testsignal = 1;
    ProcessController* ctrl = new ProcessController(testpid, testsignal, nullptr);

    EXPECT_NE(ctrl, nullptr);

    ctrl->deleteLater();
}

TEST(UT_ProcessController_execute, UT_ProcessController_execute_001)
{
    pid_t testpid = 123;
    int testsignal = 1;
    ProcessController* ctrl = new ProcessController(testpid, testsignal, nullptr);

    EXPECT_NE(ctrl, nullptr);

    ctrl->deleteLater();
}

#if 0
TEST(UT_DesktopEntryCacheUpdater_DesktopEntryCacheUpdater, UT_DesktopEntryCacheUpdater_DesktopEntryCacheUpdater_001)
{
    using namespace core::process;
    DesktopEntryCacheUpdater* updater = new DesktopEntryCacheUpdater();
    EXPECT_NE(updater, nullptr);
    updater->deleteLater();
}

TEST(UT_DesktopEntryCacheUpdater_createEntry, UT_DesktopEntryCacheUpdater_createEntry_001)
{
    using namespace core::process;

    const QString filePath("test.desktop");
    const QString name = "test1";
    const QString displayName = "DisplayTest1";
    const QString exec = QString("/usr/bin/%1").arg(name);
    const QString icon = "/usr/share/test1/icon";
    const QString startup = name;

    QFile file(filePath);

    // 创建desktop文件
    if(file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QByteArray data;
        file.write("[Desktop Entry]\n");
        file.write(QString("Name=%1\n").arg(name).toUtf8());
        file.write(QString("GenericName=%1\n").arg(displayName).toUtf8());
        file.write("Type=Application\n");
        file.write(QString("Exec=%1\n").arg(exec).toUtf8());
        file.write(QString("Icon=%1\n").arg(icon).toUtf8());
        file.write("Categories=Utility\n");
        file.write(QString("StartupWMClass=%1\n").arg(startup).toUtf8());
        file.close();
    }

    QFileInfo info(filePath);
    DesktopEntry entry = DesktopEntryCacheUpdater::createEntry(info);

    EXPECT_EQ(entry->name, name);
    EXPECT_EQ(entry->displayName, name);
    EXPECT_EQ(entry->exec.first(), exec);
    EXPECT_EQ(entry->icon, icon);
    EXPECT_EQ(entry->startup_wm_class, startup);

    if(file.exists()) {
        file.remove();
    }
}
#endif