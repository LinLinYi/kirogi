#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QPixmap>

#include "ScreenPara.h"
#include "NavigationBar.h"
#include "SheetStyleConfig.hpp"

NavigationBar::NavigationBar(QWidget* parent) : QWidget(parent)
{
    this->initializeWidget();
}

void NavigationBar::initializeWidget()
{
    screenHeight = ScreenPara::getInstance()->screenHeight();
    screenWidth = ScreenPara::getInstance()->screenWidth();
    this->setFixedWidth(screenWidth);

    leftButton = new QPushButton(this);
    leftButton->setFixedWidth(screenWidth*0.25);
    leftButton->setStyleSheet(SheetStyle::NAVIGATION_BUTTON);
    connect(leftButton, SIGNAL(clicked()), this, SLOT(on_leftBtn_clicked()));

    titleLabel = new QLabel(this);
    titleLabel->setMinimumWidth(screenWidth*0.5);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet(SheetStyle::NEVIGATION_LABEL);

    rightButton = new QPushButton(this);
    rightButton->setFixedWidth(screenWidth*0.25);
    rightButton->setStyleSheet(SheetStyle::NAVIGATION_BUTTON);
    connect(rightButton, SIGNAL(clicked()), this, SLOT(on_rightBtn_clicked()));

    QHBoxLayout* pHLTop = new QHBoxLayout();
    pHLTop->addWidget(leftButton);
    pHLTop->addWidget(titleLabel);
    pHLTop->addWidget(rightButton);
    pHLTop->setSpacing(0);
    pHLTop->setMargin(0);
    pHLTop->addStretch(0);

    this->setLayout(pHLTop);
}

void NavigationBar::setTitleText(const QString &tilText)
{
    titleText = tilText;
    titleLabel->setText(titleText);
}

void NavigationBar::setLeftText(const QString &leftText)
{
    leftButtonText = leftText;
    leftButton->setText(leftButtonText);
}

void NavigationBar::setRightText(const QString &rightText)
{
    rightButtonText = rightText;
    rightButton->setText(rightButtonText);
}

void NavigationBar::setLeftPixMap(const QPixmap &leftPix)
{
    leftButtonPixMap = leftPix;
    leftButtonPixMap.setDevicePixelRatio(2.0);

    leftButton->setIcon(leftButtonPixMap);
    leftButton->setIconSize(QSize(leftButtonPixMap.width()/2, leftButtonPixMap.height()/2));
}

void NavigationBar::setRightPixMap(const QPixmap &rightPix)
{
    rightButtonPixMap = rightPix;
    rightButtonPixMap.setDevicePixelRatio(2.0);

    rightButton->setIcon(rightButtonPixMap);
    rightButton->setIconSize(QSize(rightButtonPixMap.width()/2, rightButtonPixMap.height()/2));
}

void NavigationBar::on_leftBtn_clicked()
{
    emit leftClicked();
}

void NavigationBar::on_rightBtn_clicked()
{
    emit rightClicked();
}

NavigationBar::~NavigationBar()
{

}
