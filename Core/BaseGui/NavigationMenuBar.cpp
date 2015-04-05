#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QPixmap>
#include <QMenu>
#include <QMenuBar>
#include <QAction>

#include "MenuBar.h"
#include "NavigationMenuBar.h"
#include "ImageConfig.hpp"
#include "SheetStyleConfig.hpp"

NavigationMenuBar::NavigationMenuBar(QWidget* parent) : BaseWidget(parent)
{
    this->initializeWidget();
}

void NavigationMenuBar::initializeWidget()
{
    m_leftButton = new QPushButton(this);
    m_leftButton->setFixedWidth(this->screenWidth()*0.2);
    m_leftButton->setStyleSheet(SheetStyle::NAVIGATION_BUTTON);
    m_leftButton->setAttribute(Qt::WA_LayoutUsesWidgetRect);
    connect(m_leftButton, SIGNAL(clicked()), this, SLOT(on_leftBtn_clicked()));

    m_titleLabel = new QLabel(this);
    m_titleLabel->setMinimumWidth(this->screenWidth()*0.6);
    m_titleLabel->setAlignment(Qt::AlignCenter);
    m_titleLabel->setStyleSheet(SheetStyle::NEVIGATION_LABEL);
    m_titleLabel->setAttribute(Qt::WA_LayoutUsesWidgetRect);

    m_menuBar = new MenuBar(this);
    QPixmap pixmap(ImagePath::MENUBAR);
    m_menuBar->setPixmap(pixmap);
    m_menuBar->setFixedWidth(this->screenWidth()*0.2);
    m_menuBar->setAttribute(Qt::WA_LayoutUsesWidgetRect);
    m_menuBar->setStyleSheet("background:rgb(0,144,255); border:none; color:white;");

    QHBoxLayout* pHLTop = new QHBoxLayout(this);
    pHLTop->addWidget(m_leftButton);
    pHLTop->addWidget(m_titleLabel);
    pHLTop->addWidget(m_menuBar);
    pHLTop->addSpacing(0);
    pHLTop->addStretch(0);
    pHLTop->setMargin(0);
    this->setLayout(pHLTop);
}

void NavigationMenuBar::setTitleText(QString &text)
{
    titleText = text;
    m_titleLabel->setText(titleText);
}

void NavigationMenuBar::setLeftText(QString &text)
{
    leftButtonText = text;
    m_leftButton->setText(leftButtonText);
}

void NavigationMenuBar::setMenuText(QString &text)
{
    menuBarText = text;
    m_menuBar->setTitleText(menuBarText);
}

void NavigationMenuBar::setLeftPixMap(QPixmap &pixmap)
{
    leftButtonPixMap = pixmap;
    leftButtonPixMap.setDevicePixelRatio(2.0);

    m_leftButton->setIcon(leftButtonPixMap);
    m_leftButton->setIconSize(QSize(leftButtonPixMap.width()/2, leftButtonPixMap.height()/2));
}

void NavigationMenuBar::setMenuPixMap(QPixmap &pixmap)
{
    menuBarPixMap = pixmap;
    m_menuBar->setPixmap(menuBarPixMap);
}

void NavigationMenuBar::on_leftBtn_clicked()
{
    emit leftClicked();
}

NavigationMenuBar::~NavigationMenuBar()
{

}

