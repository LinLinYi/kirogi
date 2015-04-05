#include <QPalette>
#include <QPainter>
#include "StartPage.h"
#include "ScreenPara.h"
#include "ImageConfig.hpp"

StartPage::StartPage(QWidget* parent) : QWidget(parent)
{
    //屏幕参数设置
    screenHeight = ScreenPara::getInstance()->screenHeight();
    screenWidth = ScreenPara::getInstance()->screenWidth();
    this->setFixedHeight(screenHeight);
    this->setFixedWidth(screenWidth);
    this->setAutoFillBackground(true);
}

void StartPage::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPixmap pixmap(ImagePath::APP_START);
    pixmap.setDevicePixelRatio(2.0);
    QPainter painter(this);
    painter.drawPixmap(0,0,screenWidth,screenHeight,pixmap);
}

StartPage::~StartPage()
{

}
