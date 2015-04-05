#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>

#include "GlobalDefine.hpp"
#include "UserProtocol.hpp"
#include "ListWidget.h"
#include "NavigationBar.h"
#include "ImageConfig.hpp"
#include "GroupButton.h"
#include "DClusterDevInfo.h"
#include "SheetStyleConfig.hpp"
#include "DClusterDeviceDetailPage.h"
#include "DClusterDevAlterDescribePage.h"
#include "DClusterDevAlterRemarksPage.h"

DClusterDeviceDetailPage::DClusterDeviceDetailPage(QWidget* parent, quint64 clusID, quint64 devID)
    : BasePage(parent)
    , curClusterID(clusID)
    , curDeviceID(devID)
    , btnTitleInfo(NULL)
    , btnMacAddr(NULL)
    , btnDevRemarks(NULL)
    , btnDevDescribe(NULL)
    , curDeviceInfo(NULL)
    , devAlterRemarksPage(NULL)
    , devAlterDescribePage(NULL)
{
    this->initializeWidget();
}

void DClusterDeviceDetailPage::initializeWidget()
{
    //导航栏
    QString strTitle = tr("设备资料");
    QString strLeftBtn = tr("设备");
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

    //设备标题信息
    QPixmap pixmap(ImagePath::DEVICE_PORTRAIT);
    btnTitleInfo = new GroupButton(this);
    btnTitleInfo->setPixMap(pixmap);
    btnTitleInfo->setFixedHeight(this->screenHeight()*0.12);
    btnTitleInfo->setFixedWidth(this->screenWidth());
    btnTitleInfo->setAttribute(Qt::WA_LayoutUsesWidgetRect);

    //设备详细信息
    btnDevRemarks = new GroupButton(this);
    btnDevRemarks->setAttribute(Qt::WA_LayoutUsesWidgetRect);
    btnDevRemarks->setStyleSheet(SheetStyle::GROUPBUTTON_BOTTOMBORDER);
    btnDevRemarks->setText(tr("备注"));
    btnDevRemarks->setFixedHeight(this->screenHeight()*0.08);
    btnDevRemarks->setContentsMargins(0,0,this->screenWidth()*0.03, 0);
    connect(btnDevRemarks, SIGNAL(clicked()), this, SLOT(on_btnRemarks_clicked()));

    btnDevDescribe = new GroupButton(this);
    btnDevDescribe->setAttribute(Qt::WA_LayoutUsesWidgetRect);
    btnDevDescribe->setStyleSheet(SheetStyle::GROUPBUTTON_BOTTOMBORDER);
    btnDevDescribe->setText(tr("描述"));
    btnDevDescribe->setFixedHeight(this->screenHeight()*0.08);
    btnDevDescribe->setContentsMargins(0,0,this->screenWidth()*0.03, 0);
    connect(btnDevDescribe, SIGNAL(clicked()), this, SLOT(on_btnDescribe_clicked()));

    btnMacAddr = new GroupButton(this);
    btnMacAddr->setAttribute(Qt::WA_LayoutUsesWidgetRect);
    btnMacAddr->setStyleSheet(SheetStyle::GROUPBUTTON_NOBORDER);
    btnMacAddr->setText(tr("MAC地址"));
    btnMacAddr->setFixedHeight(this->screenHeight()*0.08);
    btnMacAddr->setContentsMargins(0,0,this->screenWidth()*0.03, 0);

    ListWidget* devInfoList = new ListWidget(this);
    devInfoList->setStyleSheet(SheetStyle::LISTWIDGET);
    devInfoList->addWidget(btnDevRemarks);
    devInfoList->addWidget(btnDevDescribe);
    devInfoList->addWidget(btnMacAddr);
    devInfoList->setContentsMargins(this->screenWidth()*0.05,0,0,0);

    QVBoxLayout* vbTotalLayout = new QVBoxLayout;
    vbTotalLayout->addWidget(btnTitleInfo);
    vbTotalLayout->addWidget(devInfoList);
    vbTotalLayout->addSpacing(this->screenHeight()*0.05);
    vbTotalLayout->setMargin(0);
    vbTotalLayout->setSpacing(0);
    vbTotalLayout->addStretch(0);
    this->setBodyPartLayout(vbTotalLayout);

    //屏幕触摸滚动设置
    this->installScrollViewportArea();
    this->loadLocalData();
}

void DClusterDeviceDetailPage::fetchData(InputPacket& inpack)
{
    this->transferDataToAnotherPage(inpack, devAlterDescribePage);
    this->transferDataToAnotherPage(inpack, devAlterRemarksPage);
}

void DClusterDeviceDetailPage::loadLocalData()
{
    SAFE_DELETE(curDeviceInfo);
    curDeviceInfo = new DClusterDevInfo;
    curDeviceInfo->loadDataSource(curClusterID, curDeviceID);

    QString strDevID = QString(tr("设备ID：")) + QString::number(curDeviceInfo->getDeviceID());
    btnTitleInfo->setText(curDeviceInfo->getDeviceName());
    btnTitleInfo->setRemarks(strDevID);
    btnMacAddr->setDescribe(curDeviceInfo->getDeviceMac());

    QPixmap pixmap = QPixmap(ImagePath::RIGHT_HOLLOW_ARROWS);
    btnDevRemarks->setReserve(pixmap);
    btnDevRemarks->setDescribe(curDeviceInfo->getRemarks());
    btnDevDescribe->setReserve(pixmap);
    btnDevDescribe->setDescribe(curDeviceInfo->getDescribe());
}

void DClusterDeviceDetailPage::on_btnPrePage_clicked()
{
    this->hide();
    this->deleteLater();
}

void DClusterDeviceDetailPage::on_btnRemarks_clicked()
{
    devAlterRemarksPage = new DClusterDevAlterRemarksPage(this, curClusterID, curDeviceID);
    this->displayConcretePage(devAlterRemarksPage);
}

void DClusterDeviceDetailPage::on_btnDescribe_clicked()
{
    devAlterDescribePage = new DClusterDevAlterDescribePage(this, curClusterID, curDeviceID);
    this->displayConcretePage(devAlterDescribePage);
}

void DClusterDeviceDetailPage::on_recvDataFromOtherPage(quint16 cmdCode, char *data, quint16 dataLen)
{
    emit readyRead(cmdCode, data, dataLen);
}

void DClusterDeviceDetailPage::on_deleteSubPages()
{
    devAlterDescribePage = NULL;
    devAlterRemarksPage = NULL;
    this->loadLocalData();
}

void DClusterDeviceDetailPage::transferDataToAnotherPage(InputPacket& inpack, BasePage* page)
{
    if(page == NULL) return;
    page->fetchData(inpack);
}

void DClusterDeviceDetailPage::displayConcretePage(BasePage *page)
{
    if(page == NULL) return;
    page->show();
    connect(page, SIGNAL(destroyed()), this, SLOT(on_deleteSubPages()));
    connect(page, SIGNAL(readyRead(quint16,char*,quint16)), this, SLOT(on_recvDataFromOtherPage(quint16,char*,quint16)));
}

DClusterDeviceDetailPage::~DClusterDeviceDetailPage()
{
    SAFE_DELETE(curDeviceInfo)
}
