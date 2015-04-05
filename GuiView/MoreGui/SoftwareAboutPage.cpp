#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>

#include "SoftwareAboutPage.h"
#include "UserProtocol.hpp"
#include "GlobalDefine.hpp"
#include "GroupButton.h"
#include "ImageConfig.hpp"
#include "NavigationBar.h"
#include "SheetStyleConfig.hpp"

SoftwareAboutPage::SoftwareAboutPage(QWidget* parent, quint64 accountID)
    : BasePage(parent)
    , curAccountID(accountID)
{
    this->initializeWidget();
}

void SoftwareAboutPage::initializeWidget()
{
    //导航栏
    QString strTitle = tr("关于");
    QString strLeftBtn = tr("取消");
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

    //软件图标
    QPixmap pixmap(ImagePath::SOFTWARE_ICON);
    pixmap.setDevicePixelRatio(2);
    lblSoftwareIcon = new QLabel(this);
    lblSoftwareIcon->setPixmap(pixmap);
    lblSoftwareIcon->setAlignment(Qt::AlignCenter);
    lblSoftwareIcon->setFixedHeight(this->screenHeight()*0.14);

    //软件版本
    QString versionIllustrate = SoftwareVer::versionContent();
    lblSoftwareVersion = new QLabel(this);
    lblSoftwareVersion->setText(versionIllustrate);
    lblSoftwareVersion->setAlignment(Qt::AlignCenter);
    lblSoftwareVersion->setFixedHeight(this->screenHeight()*0.06);
    lblSoftwareVersion->setStyleSheet("color:rgb(136,136,136); font: 18px;");

    //功能介绍
    btnFunctionIntroduce = new GroupButton(this);
    this->setGroupButton(btnFunctionIntroduce, tr("功能介绍"));
    btnFunctionIntroduce->setStyleSheet(SheetStyle::GROUPBUTTON_BOTTOMBORDER);
    connect(btnFunctionIntroduce, SIGNAL(clicked()), this, SLOT(on_btnFunIntroduce_clicked()));

    QVBoxLayout* versionLayout = new QVBoxLayout;
    versionLayout->addWidget(lblSoftwareIcon);
    versionLayout->addWidget(lblSoftwareVersion);
    versionLayout->setMargin(0);

    //公司名称
    lblCompanyName = new QLabel(this);
    lblCompanyName->setText(tr("深圳市合信自动化技术有限公司 版权所有"));
    lblCompanyName->setFixedHeight(this->screenHeight()*0.022);
    lblCompanyName->setAlignment(Qt::AlignCenter);
    lblCompanyName->setStyleSheet("color:rgb(136,136,136); font: 12px;");

    //版权声明
    lblCopyRightDeclare = new QLabel(this);
    lblCopyRightDeclare->setAlignment(Qt::AlignCenter);
    lblCopyRightDeclare->setFixedHeight(this->screenHeight()*0.03);
    lblCopyRightDeclare->setStyleSheet("color:rgb(136,136,136); font: 12px;");
    lblCopyRightDeclare->setText(tr("Copyright © 2014-2015 CO-TRUST.All Rights Reserved."));

    QVBoxLayout* copyRightLayout = new QVBoxLayout;
    copyRightLayout->addWidget(lblCompanyName);
    copyRightLayout->addWidget(lblCopyRightDeclare);
    copyRightLayout->setMargin(0);

    QVBoxLayout* vTotalLayout = new QVBoxLayout;
    vTotalLayout->addLayout(versionLayout);
    vTotalLayout->addWidget(btnFunctionIntroduce);
    vTotalLayout->addSpacing(this->screenHeight()*0.45);
    vTotalLayout->addLayout(copyRightLayout);
    vTotalLayout->setMargin(0);
    this->setBodyPartLayout(vTotalLayout);

    //屏幕触摸滚动设置
    this->installScrollViewportArea();
}

void SoftwareAboutPage::fetchData(InputPacket&)
{
    //////////////////Do Nothing//////////////////
}

void SoftwareAboutPage::setGroupButton(GroupButton* btn, const QString& title, const quint16 height)
{
    QPixmap pixmap(ImagePath::RIGHT_HOLLOW_ARROWS);
    btn->setReserve(pixmap);
    btn->setText(title);
    btn->setFixedWidth(this->screenWidth());
    btn->setAttribute(Qt::WA_LayoutUsesWidgetRect);

    if(height == 0)
        btn->setFixedHeight(this->screenHeight()*0.0775);
    else
        btn->setFixedHeight(height);
}


void SoftwareAboutPage::on_btnPrePage_clicked()
{
    this->hide();
    this->deleteLater();
}

void SoftwareAboutPage::on_btnFunIntroduce_clicked()
{

}

SoftwareAboutPage::~SoftwareAboutPage()
{

}

