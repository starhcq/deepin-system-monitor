/*
* Copyright (C) 2011 ~ 2020 Uniontech Software Technology Co.,Ltd
*
* Author:      Wang Yong <wangyong@deepin.com>
* Maintainer:  maojj <maojunjie@uniontech.com>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* any later version.
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "memory_widget.h"
#include "../common/utils.h"
#include "common/datacommon.h"
#include "datadealsingleton.h"

#include <DApplication>
#include <DApplicationHelper>
#include <DPalette>
#include <DStyleHelper>

#include <QApplication>
#include <QDebug>
#include <QPainter>
#include <QPainterPath>
#include <QtMath>
#include <QBrush>
#include <QPaintEvent>
#include <QFontMetrics>

DWIDGET_USE_NAMESPACE

using namespace Utils;

MemoryWidget::MemoryWidget(QWidget *parent)
    : QWidget(parent)
{
     auto *dAppHelper = DApplicationHelper::instance();
    connect(dAppHelper, &DApplicationHelper::themeTypeChanged, this, &MemoryWidget::changeTheme);
    changeTheme(dAppHelper->themeType());

    setFixedSize(280, 153);

    changeFont(DApplication::font());
    connect(dynamic_cast<QGuiApplication *>(DApplication::instance()), &DApplication::fontChanged,
            this, &MemoryWidget::changeFont);

    connect(&DataDealSingleton::getInstance(), &DataDealSingleton::sigDataUpdate, this, &MemoryWidget::updateStatus);

    installEventFilter(this);
}
MemoryWidget::~MemoryWidget() {}


void MemoryWidget::updateStatus()
{
    QString memUsage, memTotal, memPercent, swapUsage, swapTotal, swapPercent;
    if (!DataDealSingleton::getInstance().readMemInfo(memUsage, memTotal, memPercent, swapUsage, swapTotal, swapPercent))
        qInfo()<<"failed";
//    qInfo()<<"swapUsage: "<<swapUsage;
//    qInfo()<<"swapTotal: "<<swapTotal;

    m_memPercent = memPercent;
    m_swapPercent = swapPercent;

    QStringList strsMemUsage = memUsage.split(" ");
    if (strsMemUsage.size() == 2)
    {
        m_memUsage = strsMemUsage.at(0);
        m_memUsageUnit = strsMemUsage.at(1);
    }
    else
        return;

    m_memTotal = memTotal;

    QStringList strsSwapUsage = swapUsage.split(" ");
    if (strsSwapUsage.size() == 2)
    {
        m_swapUsage = strsSwapUsage.at(0);
        m_swapUnit = strsSwapUsage.at(1);
    }
    else
        return;

    m_swapTotal = swapTotal;

    update();
}

void MemoryWidget::changeTheme(DApplicationHelper::ColorType themeType)
{
    switch (themeType) {
    case DApplicationHelper::LightType:
        numberColor.setRgb(0,26,46);
        m_titleTrans = Globals::TitleTransLight;
        m_contentTrans = Globals::contentTransLight;
        m_hoverTrans = Globals::hoverTransLight;
        m_icon = QIcon(QString(":/icons/icon_memory_light.png"));
        break;
    case DApplicationHelper::DarkType:
        numberColor.setRgb(192,198,212);
        m_titleTrans = Globals::TitleTransDark;
        m_contentTrans = Globals::contentTransDark;
        m_hoverTrans = Globals::hoverTransDark;
//        m_icon = QIcon(iconPathFromQrc("dark/icon_network_light.svg"));
        break;
    default:
        break;
    }

    // init colors
    auto *dAppHelper = DApplicationHelper::instance();
    auto palette = dAppHelper->applicationPalette();

    textColor = palette.color(DPalette::Text);
    summaryColor = palette.color(DPalette::TextTips);
}

void MemoryWidget::paintEvent(QPaintEvent *e)
{
    setFixedWidth(280);
    QPainter painter;
    painter.begin(this);

    //裁剪绘制区域
    QPainterPath path;
    path.addRoundedRect(rect(), 8, 8);
    painter.setClipPath(path);
    if (m_isHover) {
        painter.fillRect(rect(), QBrush(QColor(255, 255, 255, m_hoverTrans)));
    } else {
        painter.fillRect(rect(), QBrush(QColor(255, 255, 255, 0)));
    }

    //标题栏背景
    QRect titleRect(rect().x(), rect().y(), 280, 36);
    painter.fillRect(titleRect, QBrush(QColor(255, 255, 255, m_titleTrans)));
    QRect contentRect(rect().x(), rect().y()+36, 280, 167);
    painter.fillRect(contentRect, QBrush(QColor(255, 255, 255,m_contentTrans)));

    //标题
    painter.setFont(m_sectionFont);
    QFontMetrics fmTitle = painter.fontMetrics();
    int widthTitleTxt = fmTitle.width("内存");
    int heightTitleTxt = fmTitle.descent()+fmTitle.ascent();
    QRect netTitleRect(titleRect.x(), titleRect.y(), widthTitleTxt, heightTitleTxt);
    painter.drawText(titleRect, Qt::AlignHCenter | Qt::AlignVCenter,"内存");

    //图标
    int iconSize = 20;
    QRect iconRect(titleRect.x()+titleRect.width()/2-netTitleRect.width() -5, titleRect.y() + qCeil((titleRect.height() - iconSize) / 2.) + 2,iconSize, iconSize);
    m_icon.paint(&painter, iconRect);

    int sectionSize = 6;

    QString memoryTitleUnit = QString("%1 / %2")
                            .arg(m_memUsageUnit)
                            .arg(m_memTotal);

    QString memoryContent = QString("%1 (%2%)")
                          .arg(DApplication::translate("Process.Graph.View", "内存"))//Memory
                          .arg(m_memPercent);

    QString swapTitle = "";
    QString swapContent = "";
    if (m_swapTotal == "") {
        // After the memory and swap space text, add a space before the brackets
        swapTitle = QString("%1 (%2)")
                    .arg(DApplication::translate("Process.Graph.View", "交换内存"))//Swap
                    .arg(DApplication::translate("Process.Graph.View", "Not enabled"));
        swapContent = "";
    } else {
        swapTitle= QString("%1 / %2")
                                .arg(m_swapUnit)
                                .arg(m_swapTotal);

        swapContent = QString("%1 (%2%)")
                              .arg(DApplication::translate("Process.Graph.View", "交换内存"))//Memory
                              .arg(m_swapPercent);
    }

    QFontMetrics fmMem(m_memFont);
    QFontMetrics fmMemUnit(m_memUnitFont);
    QFontMetrics fmMemTxt(m_memTxtFont);
//    QFontMetrics fmMemTxt(m_memTxtFont);

    int letfsize = 36;
    int margin = 10;
    //内存数字
    QRect memRect(letfsize, contentRect.y(),// + topsize
                  fmMem.size(Qt::TextSingleLine, m_memUsage).width(), fmMem.height());
    //内存单位
    QRect memRectUnit(memRect.x()+memRect.width()-1, contentRect.y() + 10,
                  fmMemUnit.size(Qt::TextSingleLine, memoryTitleUnit).width(), fmMem.height());
    //内存txt
    QRect memTxtRect(letfsize, contentRect.y() + memRect.height(),
                      fmMemTxt.size(Qt::TextSingleLine, memoryContent).width(),
                      fmMemTxt.height());
    QRectF memIndicatorRect(memTxtRect.x() - margin, memTxtRect.y() + qCeil((memTxtRect.height() - sectionSize) / 2.),
                            sectionSize, sectionSize);

    QPainterPath section;
    section.addEllipse(memIndicatorRect);
    painter.fillPath(section, memoryColor);

//    m_memFont.setWeight(QFont::Medium);
    painter.setFont(m_memFont);
//    painter.setPen(QPen(textColor));
    painter.drawText(memRect, Qt::AlignLeft | Qt::AlignVCenter,
                     fmMem.elidedText(m_memUsage, Qt::ElideRight,
                                      rect().width() - memRect.x() - outsideRingRadius));

//    m_memFont.setWeight(QFont::Medium);
    painter.setFont(m_memUnitFont);
//    painter.setPen(QPen(textColor));
    painter.drawText(memRectUnit, Qt::AlignLeft | Qt::AlignHCenter, memoryTitleUnit);

    painter.setFont(m_memTxtFont);
//    painter.setPen(QPen(summaryColor));
    painter.drawText(memTxtRect, Qt::AlignLeft | Qt::AlignVCenter, memoryContent);

//    qInfo()<<"m_swapUsage: "<<m_swapUsage;
//    qInfo()<<"swapTitle: "<<swapTitle;

    //swap数字
    QRect swapRect(letfsize, memTxtRect.y() + memTxtRect.height()+margin,
                  fmMem.size(Qt::TextSingleLine, m_swapUsage).width(), fmMem.height());
    //swap单位
    QRect swapRectUnit(swapRect.x()+swapRect.width()-1, swapRect.y()+margin,
                  fmMemUnit.size(Qt::TextSingleLine, swapTitle).width(), fmMem.height());

    QRect swapTxtRect(letfsize, swapRect.y() + swapRect.height(),//+ topsize
                      fmMemTxt.size(Qt::TextSingleLine, swapContent).width(),
                      fmMemTxt.height());
    QRectF swapIndicatorRect(swapTxtRect.x() - margin, swapTxtRect.y() + qCeil((swapTxtRect.height() - sectionSize) / 2.),
                             sectionSize, sectionSize);


    QPainterPath section2;
    section2.addEllipse(swapIndicatorRect);
    painter.fillPath(section2, swapColor);

    painter.setFont(m_memFont);
//    painter.setPen(QPen(textColor));
    painter.drawText(swapRect, m_swapUsage);

    painter.setFont(m_memUnitFont);
//    painter.setPen(QPen(textColor));
    painter.drawText(swapRectUnit, swapTitle);

    painter.setFont(m_memTxtFont);
//    painter.setPen(QPen(summaryColor));
    painter.drawText(swapTxtRect, Qt::AlignLeft | Qt::AlignVCenter, swapContent);

    ringCenterPointerX = 200;
    // Draw memory ring.
    drawLoadingRing(painter, contentRect.x() + ringCenterPointerX, contentRect.y() + ringCenterPointerY,
                    outsideRingRadius, ringWidth, 270, 270, memoryForegroundColor,
                    memoryForegroundOpacity, memoryBackgroundColor, memoryBackgroundOpacity,
                    m_memPercent.toDouble()/100);

    // Draw swap ring.
    drawLoadingRing(painter, contentRect.x() + ringCenterPointerX, contentRect.y() + ringCenterPointerY,
                    insideRingRadius, ringWidth, 270, 270, swapForegroundColor,
                    swapForegroundOpacity, swapBackgroundColor, swapBackgroundOpacity, m_swapPercent.toDouble()/100);

    // Draw percent text.
    painter.setFont(m_memPercentFont);
    painter.setPen(numberColor);
    painter.drawText(QRect(contentRect.x() + ringCenterPointerX - insideRingRadius,
                           contentRect.y() + ringCenterPointerY - insideRingRadius, insideRingRadius * 2,
                           insideRingRadius * 2),
                     Qt::AlignHCenter | Qt::AlignVCenter,
                     QString("%1%").arg(QString::number(m_memPercent.toDouble(), 'f', 1)));
}

bool MemoryWidget::eventFilter(QObject *target, QEvent *event)
{
    if (target == this) {
        if (event->type() == QEvent::Enter) {
            m_isHover = true;
            update();
            return true;
        }
        if (event->type() == QEvent::Leave) {
            m_isHover = false;
            update();
            return true;
        }
    }
    return QWidget::eventFilter(target, event);
}

void MemoryWidget::changeFont(const QFont &font)
{
    m_sectionFont = font;
    m_sectionFont.setWeight(QFont::DemiBold);
    m_sectionFont.setPointSizeF(m_sectionFont.pointSizeF());

    m_memFont = font;
    m_memFont.setWeight(QFont::Normal);
    m_memFont.setPointSizeF(m_memFont.pointSizeF()+5);

    m_memUnitFont = font;
    m_memUnitFont.setWeight(QFont::Normal);
    m_memUnitFont.setPointSizeF(m_memUnitFont.pointSizeF());

    m_memTxtFont = font;
    m_memTxtFont.setWeight(QFont::ExtraLight);
    m_memTxtFont.setPointSizeF(m_memTxtFont.pointSizeF()-2 );
}
