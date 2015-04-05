#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QLabel>
#include <QPixmap>
#include <QHBoxLayout>
#include <QEvent>

#include "GroupButton.h"

GroupButton::GroupButton(QWidget* parent) : GPushButton(parent), selectFlag(false)
{
    this->initializeWidget();
    this->grabGesture(Qt::TapGesture);
}

void GroupButton::initializeWidget()
{

    //按钮图标
    lblHead = new QLabel(this);
    lblHead->setStyleSheet("border-width: 0px; border-radius:5px;");
    lblHead->setMargin(0);
    lblHead->hide();

    //按钮标题
    lblText = new QLabel(this);
    lblText->hide();
    lblText->setMargin(0);
    lblText->setStyleSheet("font:16px Bold; color:black; border-width: 0px;");

    //标题备注
    lblRemarks = new QLabel(this);
    lblRemarks->hide();
    lblRemarks->setMargin(0);
    lblRemarks->setStyleSheet("font:13px; color:#8c8c8c; border-width: 0px;");

    QVBoxLayout* vblTitleLayout = new QVBoxLayout;
    vblTitleLayout->addStretch(0);
    vblTitleLayout->addWidget(lblText);
    vblTitleLayout->addWidget(lblRemarks);
    vblTitleLayout->addStretch(0);

    QHBoxLayout* hbTitleLayout = new QHBoxLayout;
    hbTitleLayout->addWidget(lblHead);
    hbTitleLayout->addLayout(vblTitleLayout);
    hbTitleLayout->setMargin(1);

    //标题说明
    lblDescribe = new QLabel(this);
    lblDescribe->hide();
    lblDescribe->setStyleSheet("font:15px; color:#9e9e9e; border-width: 0px;");

    //预留内容
    lblReserve = new QLabel(strReserve,this);
    lblReserve->setStyleSheet("font:bold 15px; color:#9e9e9e; border-width: 0px; border-radius:5px;");

    QHBoxLayout* hbTitleDetailLayout = new QHBoxLayout;
    hbTitleDetailLayout->addWidget(lblDescribe);
    hbTitleDetailLayout->addWidget(lblReserve);
    hbTitleDetailLayout->setMargin(1);

    hblTotal = new QHBoxLayout(this);
    hblTotal->addLayout(hbTitleLayout);
    hblTotal->addStretch(0);
    hblTotal->addLayout(hbTitleDetailLayout);
    this->setLayout(hblTotal);
}

bool GroupButton::event(QEvent *e)
{
    QMouseEvent* mEvent = (QMouseEvent*)e;
    switch(e->type())
    {
    case QEvent::MouseButtonPress:
        pressedPos = mEvent->pos();
        break;
    case QEvent::MouseMove:
        //////////////////////
        break;
    case QEvent::MouseButtonRelease:
        releasedPos = mEvent->pos();
        int movePosX = releasedPos.x() - pressedPos.x();
        if( movePosX > 5 || movePosX < -5){//降低按钮点击灵敏度
            return true;
        }
        break;
    }
    return QWidget::event(e);
}

void GroupButton::setPixMap(const QPixmap &pixMap, int w, int h)
{
    lblHead->show();
    pixPortrait = pixMap;

    if(w == 0 && h == 0){
         pixPortrait.setDevicePixelRatio(2.0);
         lblHead->setFixedHeight(pixPortrait.height()/2);
         lblHead->setFixedWidth(pixPortrait.width()/2);
    }else{
        lblHead->setFixedHeight(h);
        lblHead->setFixedWidth(w);
        lblHead->setAutoFillBackground(true);
    }
    lblHead->setPixmap(pixPortrait);
}

void GroupButton::setText(const QString& title)
{
    lblText->show();
    strTitle = title;
    lblText->setText(strTitle);
}

void GroupButton::setDescribe(const QString& describe)
{
    lblDescribe->show();
    strDescribe = describe;
    lblDescribe->setText(strDescribe);
}

void GroupButton::setDescribe(const QPixmap& describe, int w, int h)
{
    lblDescribe->show();
    pixDescribe = describe;

    if(w == 0 && h == 0){
        pixDescribe.setDevicePixelRatio(2.0);
        lblDescribe->setFixedHeight(pixDescribe.height()/2);
        lblDescribe->setFixedWidth(pixDescribe.width()/2);
    }else{
        lblDescribe->setFixedHeight(h);
        lblDescribe->setFixedWidth(w);
        lblDescribe->setAutoFillBackground(true);
    }
    lblDescribe->setPixmap(pixDescribe);
}

void GroupButton::setRemarks(const QString &remarks)
{
    lblRemarks->show();
    strRemarks = remarks;
    lblRemarks->setText(strRemarks);
}

void GroupButton::setReserve(const QString& reserve)
{
    lblReserve->show();
    strReserve = reserve;
    lblReserve->setText(strReserve);
}

void GroupButton::setReserve(QPixmap reserve)
{
    lblReserve->show();
    pixReserve = reserve;
    pixReserve.setDevicePixelRatio(2.0);
    lblReserve->setPixmap(pixReserve);
}

void GroupButton::setSelectFlag(bool isSelect)
{
    selectFlag = isSelect;
}

void GroupButton::setPortraitPath(const QString &strPath)
{
    pathPortrait = strPath;
}

void GroupButton::setContentsMargins(int left, int top, int right, int bottom)
{
    hblTotal->setMargin(0);
    QWidget::setContentsMargins(left, top, right, bottom);
}

GroupButton::~GroupButton()
{

}
