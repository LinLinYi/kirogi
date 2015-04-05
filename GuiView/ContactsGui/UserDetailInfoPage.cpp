#include <QDebug>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include "BasePage.h"
#include "InputPacket.h"
#include "NavigationBar.h"
#include "UserProtocol.hpp"
#include "ImageConfig.hpp"
#include "UserDetailInfoPage.h"

UserDetailInfoPage::UserDetailInfoPage(QWidget* parent) : BasePage(parent)
{
    this->initializeWidget();
}

UserDetailInfoPage::UserDetailInfoPage(quint64 id, QString name, QWidget *parent)
    :BasePage(parent)
    , userID(id)
    , userName(name)
{
    this->initializeWidget();
}

void UserDetailInfoPage::initializeWidget()
{
    //导航栏
    QString strTitle = tr("个人资料");
    QString strLeftBtn = tr("联系人");
    QPixmap leftArrowsPixmap(ImagePath::NAVIGATION_LEFT_ARROWS);
    navigationBar = new NavigationBar(this);
    navigationBar->setTitleText(strTitle);
    navigationBar->setLeftText(strLeftBtn);
    navigationBar->setLeftPixMap(leftArrowsPixmap);
    connect(navigationBar, SIGNAL(leftClicked()), this, SLOT(on_btnPrePage_clicked()));

    QHBoxLayout* pHLTop = new QHBoxLayout();
    pHLTop->addWidget(navigationBar);
    pHLTop->setSpacing(0);
    pHLTop->setMargin(0);
    this->setTopbarLayout(pHLTop);

    //屏幕触摸滚动设置
    this->installScrollViewportArea();
}

void UserDetailInfoPage::fetchData(InputPacket&)
{

}

void UserDetailInfoPage::on_btnPrePage_clicked()
{
    this->hide();
    this->deleteLater();
}

UserDetailInfoPage::~UserDetailInfoPage(){}

