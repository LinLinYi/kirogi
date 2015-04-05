#include <QHBoxLayout>

#include "UserProtocol.hpp"
#include "ListWidget.h"
#include "DClusterInfo.h"
#include "GroupButton.h"
#include "ImageConfig.hpp"
#include "NavigationBar.h"
#include "SheetStyleConfig.hpp"
#include "DClusterDetailInfoPage.h"

DClusterDetailInfoPage::DClusterDetailInfoPage(QWidget* parent, quint64 clusID)
    : BasePage(parent)
    , curClusterID(clusID)
    , btnTitleInfo(NULL)
    , btnRemaks(NULL)
    , btnDescribe(NULL)
    , clusterInfo(NULL)
{
    this->initializeWidget();
}

void DClusterDetailInfoPage::initializeWidget()
{
    //导航栏
    QString strTitle = tr("群资料");
    QString strLeftBtn = tr("返回");
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

    //群标题信息
    QPixmap pixmap(ImagePath::DCLUSTER_PORTRAIT);
    btnTitleInfo = new GroupButton(this);
    btnTitleInfo->setPixMap(pixmap);
    btnTitleInfo->setFixedHeight(this->screenHeight()*0.12);
    btnTitleInfo->setFixedWidth(this->screenWidth());
    btnTitleInfo->setAttribute(Qt::WA_LayoutUsesWidgetRect);

    //群详细信息
    btnRemaks = new GroupButton(this);
    btnRemaks->setAttribute(Qt::WA_LayoutUsesWidgetRect);
    btnRemaks->setStyleSheet(SheetStyle::GROUPBUTTON_BOTTOMBORDER);
    btnRemaks->setText(tr("备注"));
    btnRemaks->setFixedHeight(this->screenHeight()*0.08);
    btnRemaks->setContentsMargins(0,0,this->screenWidth()*0.03, 0);

    btnDescribe = new GroupButton(this);
    btnDescribe->setAttribute(Qt::WA_LayoutUsesWidgetRect);
    btnDescribe->setStyleSheet(SheetStyle::GROUPBUTTON_NOBORDER);
    btnDescribe->setText(tr("描述"));
    btnDescribe->setFixedHeight(this->screenHeight()*0.08);
    btnDescribe->setContentsMargins(0,0,this->screenWidth()*0.03, 0);

    ListWidget* clusInfoList = new ListWidget(this);
    clusInfoList->setStyleSheet(SheetStyle::LISTWIDGET);
    clusInfoList->addWidget(btnRemaks);
    clusInfoList->addWidget(btnDescribe);
    clusInfoList->setContentsMargins(this->screenWidth()*0.05,0,0,0);

    QVBoxLayout* vbTotalLayout = new QVBoxLayout;
    vbTotalLayout->addWidget(btnTitleInfo);
    vbTotalLayout->addWidget(clusInfoList);
    vbTotalLayout->addSpacing(this->screenHeight()*0.05);
    vbTotalLayout->setMargin(0);
    vbTotalLayout->setSpacing(0);
    vbTotalLayout->addStretch(0);
    this->setBodyPartLayout(vbTotalLayout);

    //屏幕触摸滚动设置
    this->installScrollViewportArea();
    this->loadLocalData();
}

void DClusterDetailInfoPage::fetchData(InputPacket&)
{
    return;
}

void DClusterDetailInfoPage::loadLocalData()
{
    SAFE_DELETE(clusterInfo);
    clusterInfo = new DClusterInfo;
    clusterInfo->loadDataSource(curClusterID);

    QString clusterName = clusterInfo->getClusterName();
    QString clusterID = QString(tr("群ID：")) + QString::number(clusterInfo->getClusterID() & ConstNum::DEVICE_ID_BASIC_POSTFIX);
    QString clusterDescribe = clusterInfo->getDescribe();
    QString strDetail = clusterDescribe.leftJustified(15);

    btnTitleInfo->setText(clusterName);
    btnTitleInfo->setRemarks(clusterID);
    btnDescribe->setDescribe(strDetail);
    btnRemaks->setDescribe(clusterInfo->getRemarks());
}

void DClusterDetailInfoPage::on_btnPrePage_clicked()
{
    this->hide();
    this->deleteLater();
}

DClusterDetailInfoPage::~DClusterDetailInfoPage()
{
    SAFE_DELETE(clusterInfo);
}

