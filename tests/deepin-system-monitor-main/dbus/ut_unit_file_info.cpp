/*
* Copyright (C) 2019 ~ 2021 Uniontech Software Technology Co.,Ltd
*
* Author:     xuezifan<xuezifan@uniontech.com>
*
* Maintainer: xuezifan<xuezifan@uniontech.com>
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

//self
#include "dbus/unit_file_info.h"

//qt
#include <QString>
#include <QDebug>
#include <QExplicitlySharedDataPointer>
#include <QList>
#include <QtDBus>

//gtest
#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

/***************************************STUB begin*********************************************/

/***************************************STUB end**********************************************/

class UT_UnitFileInfo : public ::testing::Test
{
public:
    UT_UnitFileInfo() : m_tester(nullptr), m_tester1(nullptr), m_tester2(nullptr) {}

public:
    virtual void SetUp()
    {
        const QString name1("DEF");
        const QString status("GHI");
        m_tester = new UnitFileInfo();
        m_tester1 = new UnitFileInfo(name1,"");
        m_tester2 = new UnitFileInfo(name1,status);
    }

    virtual void TearDown()
    {
        delete m_tester;
        delete m_tester1;
        delete m_tester2;
    }


protected:
    UnitFileInfo *m_tester;
    UnitFileInfo *m_tester1;
    UnitFileInfo *m_tester2;
};

TEST_F(UT_UnitFileInfo, initTest)
{

}

TEST_F(UT_UnitFileInfo, test_setName_01)
{
    const QString name("ABC");

    m_tester->setName(name);

    QString retName = m_tester->getName();

    EXPECT_EQ(name, retName);
}

TEST_F(UT_UnitFileInfo, test_getName_01)
{
    const QString name("DEF");
    QString retName = m_tester1->getName();

    EXPECT_EQ(name, retName);
}

TEST_F(UT_UnitFileInfo, test_getStatus_01)
{
    const QString status("GHI");

    QString retStatus = m_tester2->getStatus();

    EXPECT_EQ(status, retStatus);
}

TEST_F(UT_UnitFileInfo, test_setStatus_001)
{
    const QString status("ABC");
    m_tester->setStatus(status);

    QString retStatus = m_tester->getStatus();

    EXPECT_EQ(status, retStatus);
}