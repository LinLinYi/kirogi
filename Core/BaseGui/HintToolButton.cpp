#include <QPainter>
#include "ImageConfig.hpp"
#include "HintToolButton.h"

HintToolButton::HintToolButton(QWidget* parent) : QToolButton(parent)
{
    unreadMsgCount = 0;
    this->setAttribute(Qt::WA_NoSystemBackground);
}

void HintToolButton::setUnreadMsgCount(quint16 count)
{
    unreadMsgCount = count;
    this->repaint();
}

void HintToolButton::paintEvent(QPaintEvent *event)
{
    QToolButton::paintEvent(event);
    if(unreadMsgCount <= 0){
        return;
    }

    QRect curWinRect;
    QRect unreadPicRect;
    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform,true);
    curWinRect = this->rect();

    QPixmap pixmap;
    QString textTip = QString::number(unreadMsgCount);
    if(unreadMsgCount < 10){//未读消息为个位数
        QPixmap pixmapUnit(ImagePath::HINT_UNIT);
        pixmap = pixmapUnit;
        unreadPicRect = QRect(curWinRect.right()- curWinRect.right()*0.50, curWinRect.top(), pixmap.width()/2, pixmap.height()/2);
    }
    else if(unreadMsgCount >= 10 && unreadMsgCount <= 99){//未读消息为十位数
        QPixmap pixmapDecade(ImagePath::HINT_DECADE);
        pixmap = pixmapDecade;
        unreadPicRect = QRect(curWinRect.right()- curWinRect.right()*0.65, curWinRect.top(), pixmap.width()/2, pixmap.height()/2);
    }else{//未读消息为百位数
        QPixmap pixmapDecade(ImagePath::HINT_HUNDRED);
        pixmap = pixmapDecade;
        textTip = QString("99+");
        unreadPicRect = QRect(curWinRect.right()- curWinRect.right()*0.70, curWinRect.top(), pixmap.width()/2, pixmap.height()/2);
    }

    pixmap.setDevicePixelRatio(2);
    painter.setPen(Qt::white);
    painter.drawPixmap(unreadPicRect, pixmap);//绘制图片

    QFont font;
    font.setFamily("Times");
    font.setPixelSize(13);
    font.setBold(true);
    painter.setFont(font);
    painter.drawText(unreadPicRect, Qt::AlignCenter, textTip);//绘制文本
}

HintToolButton::~HintToolButton()
{

}

