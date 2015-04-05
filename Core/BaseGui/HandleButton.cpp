#include <QRect>
#include <QBrush>
#include <QPainter>
#include <QPixmap>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QtGlobal>
#include <QPushButton>
#include <QToolButton>
#include <QPaintEvent>
#include <QLabel>
#include <QPixmap>
#include <QHBoxLayout>

#include "HandleButton.h"
#include "ScreenPara.h"


#define LABEL_WIDTH 16
#define HEAD_PHOTO_WIDTH 36

HandleButton::HandleButton(QWidget *parent) : GPushButton(parent)
{
    this->initializeWidget();
}

void HandleButton::initializeWidget()
{
    //按钮图片
    m_pLblPhoto = new QLabel(this);
    m_pLblPhoto->hide();
    m_pLblPhoto->setMargin(0);
    m_pLblPhoto->setStyleSheet("border-radius: 3px; border-width: 0px;");

    //按钮标题名称
    m_pLblName = new QLabel(this);
    m_pLblName->hide();
    m_pLblName->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_pLblName->setStyleSheet("font:bold 14px; color:black; border-width: 0px;");

    //上预留显示
    m_pLblUpReserve = new QLabel(this);
    m_pLblUpReserve->hide();
    m_pLblUpReserve->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_pLblUpReserve->setStyleSheet("color:gray; padding-top: 5px; font: 10px; border-width: 0px;");

    QHBoxLayout* pHUpLayout = new QHBoxLayout();
    pHUpLayout->addWidget(m_pLblName);
    pHUpLayout->addWidget(m_pLblUpReserve);
    pHUpLayout->setMargin(0);
    pHUpLayout->setSpacing(0);

    //状态
    m_pLblStatus = new QLabel(this);
    m_pLblStatus->hide();
    m_pLblStatus->setStyleSheet("color:#8c8c8c; font: 10px; border-width: 0px;");

    //最新消息内容
    m_pLblDescribe = new QLabel(this);
    m_pLblDescribe->hide();
    m_pLblDescribe->setAlignment(Qt::AlignLeft);
    m_pLblDescribe->setStyleSheet("color:#8c8c8c; font: 10px; border-width: 0px;");

    //下预留显示
    m_pLblDownReserve = new QLabel(this);
    m_pLblDownReserve->hide();
    m_pLblDownReserve->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_pLblDownReserve->setStyleSheet("color:gray; padding-top: 5px; font: 10px; border-width: 0px;");

    QHBoxLayout* pHDownLayout = new QHBoxLayout();
    pHDownLayout->addWidget(m_pLblStatus);
    pHDownLayout->addWidget(m_pLblDescribe);
    pHDownLayout->addWidget(m_pLblDownReserve);
    pHDownLayout->setSpacing(5);

    QVBoxLayout* pVboxLayout = new QVBoxLayout();
    pVboxLayout->addStretch(0);
    pVboxLayout->addLayout(pHUpLayout);
    pVboxLayout->addSpacing(5);
    pVboxLayout->addLayout(pHDownLayout);
    pVboxLayout->addStretch(0);
    pVboxLayout->setSpacing(0);

    m_pHboxLayout = new QHBoxLayout(this);
    m_pHboxLayout->addWidget(m_pLblPhoto);
    m_pHboxLayout->addLayout(pVboxLayout);
    m_pHboxLayout->setMargin(0);
    this->setLayout(m_pHboxLayout);
}

void HandleButton::setText(const QString& name)
{
    m_name = name;
    m_pLblName->show();
    m_pLblName->setText(m_name);
}

void HandleButton::setPixMap(const QPixmap &pixMap, int w, int h)
{
    m_pLblPhoto->show();
    m_pixMap = pixMap;

    if(w == 0 && h == 0){
         m_pixMap.setDevicePixelRatio(2.0);
         m_pLblPhoto->setFixedHeight(m_pixMap.height()/2);
         m_pLblPhoto->setFixedWidth(m_pixMap.width()/2);
    }else{
        m_pLblPhoto->setFixedHeight(h);
        m_pLblPhoto->setFixedWidth(w);
        m_pLblPhoto->setAutoFillBackground(true);
    }
    m_pLblPhoto->setPixmap(m_pixMap);
}

void HandleButton::setUpReserve(const QString &reserve)
{
   m_upReserve = reserve;
   m_pLblUpReserve->show();
   m_pLblUpReserve->setText(m_upReserve);
}

void HandleButton::setDownReserve(const QString &reserve)
{
    m_downReserve = reserve;
    m_pLblDownReserve->show();
    m_pLblDownReserve->setText(m_downReserve);
}

void HandleButton::setStatus(const QString &status)
{
    m_status = status;
    m_pLblStatus->show();
    m_pLblStatus->setText(status);
}

void HandleButton::setDescribe(const QString &describe)
{
    m_describe = describe;
    m_pLblDescribe->show();
    m_pLblDescribe->setText(m_describe);
}

void HandleButton::setContentsMargins(int left, int top, int right, int bottom)
{
    m_pHboxLayout->setMargin(0);
    QWidget::setContentsMargins(left, top, right, bottom);
}

HandleButton::~HandleButton()
{

}


