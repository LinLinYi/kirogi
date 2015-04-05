#include <QVBoxLayout>
#include <QHBoxLayout>

#include "GroupButton.h"
#include "DClusterUserInfo.h"
#include "UserProtocol.hpp"
#include "ImageConfig.hpp"
#include "NavigationBar.h"
#include "SheetStyleConfig.hpp"
#include "DClusterUserDetailPage.h"

DClusterUserDetailPage::DClusterUserDetailPage(QWidget* parent, quint64 clusID, quint64 userID)
    : BasePage(parent)
    , curUserID(userID)
    , curClusterID(clusID)
    , btnTitleInfo(NULL)
    , userInfo(NULL)
{
    this->initializeWidget();
}

void DClusterUserDetailPage::initializeWidget()
{
    //导航栏
    QString strTitle = tr("成员资料");
    QString strLeftBtn = tr("成员");
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

    //群成员标题信息
    QPixmap pixmap(ImagePath::USER_PORTRAIT);
    btnTitleInfo = new GroupButton(this);
    btnTitleInfo->setPixMap(pixmap);
    btnTitleInfo->setFixedHeight(this->screenHeight()*0.12);
    btnTitleInfo->setFixedWidth(this->screenWidth());
    btnTitleInfo->setContentsMargins(this->screenHeight()*0.02, 0, this->screenHeight()*0.05, 0);
    btnTitleInfo->setAttribute(Qt::WA_LayoutUsesWidgetRect);

    //用户详细信息
    btnDescribe = new GroupButton(this);
    btnDescribe->setAttribute(Qt::WA_LayoutUsesWidgetRect);
    btnDescribe->setStyleSheet(SheetStyle::GROUPBUTTON_NOBORDER);
    btnDescribe->setText(tr("描述"));
    btnDescribe->setFixedHeight(this->screenHeight()*0.08);
    btnDescribe->setContentsMargins(this->screenWidth()*0.05,0,this->screenWidth()*0.05, 0);

    QVBoxLayout* vbTotalLayout = new QVBoxLayout;
    vbTotalLayout->addWidget(btnTitleInfo);
    vbTotalLayout->addWidget(btnDescribe);
    vbTotalLayout->addSpacing(this->screenHeight()*0.05);
    vbTotalLayout->setMargin(0);
    vbTotalLayout->setSpacing(0);
    vbTotalLayout->addStretch(0);
    this->setBodyPartLayout(vbTotalLayout);

    //屏幕触摸滚动设置
    this->installScrollViewportArea();
    this->loadLocalData();
}

void DClusterUserDetailPage::fetchData(InputPacket&)
{
    ///////////////////////////////////////////
}

void DClusterUserDetailPage::loadLocalData()
{
    SAFE_DELETE(userInfo);
    userInfo = new DClusterUserInfo;
    userInfo->loadDataSource(curClusterID, curUserID);
    if(userInfo->getUserName().length() == 0 && userInfo->getRemarks().length() == 0) return;

    if(userInfo->getRemarks().length() == 0){
        btnTitleInfo->setText(userInfo->getUserName());
    }else if(userInfo->getRemarks().length() != 0){
        QString strText = QString(tr("账号：")) + userInfo->getUserName();
        btnTitleInfo->setText(userInfo->getRemarks());
        btnTitleInfo->setRemarks(strText);
    }
    btnDescribe->setDescribe(userInfo->getDescribe());
}

void DClusterUserDetailPage::on_btnPrePage_clicked()
{
    this->hide();
    this->deleteLater();
}

DClusterUserDetailPage::~DClusterUserDetailPage()
{
    SAFE_DELETE(userInfo);
}
