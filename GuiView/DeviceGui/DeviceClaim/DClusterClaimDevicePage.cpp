#include <QVBoxLayout>
#include <QHBoxLayout>

#include "UserProtocol.hpp"
#include "ListWidget.h"
#include "InputPacket.h"
#include "GlobalDefine.hpp"
#include "ImageConfig.hpp"
#include "NavigationBar.h"
#include "AssemblyData.h"
#include "HintMsgPage.h"
#include "DClusterDevInfo.h"
#include "GroupButton.h"
#include "SheetStyleConfig.hpp"
#include "DClusterClaimDevicePage.h"
#include "DClusterClaimDevicePwdPage.h"

DClusterClaimDevicePage::DClusterClaimDevicePage(QWidget* parent, quint64 clusID, quint64 devID)
    :BasePage(parent)
    , curClusterID(clusID)
    , curDeviceID(devID)
    , deviceInfo(NULL)
    , clusterClaimDevicePwdPage(NULL)
{
    this->initializeWidget();
}

void DClusterClaimDevicePage::initializeWidget()
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
    btnMacAddr = new GroupButton(this);
    btnMacAddr->setAttribute(Qt::WA_LayoutUsesWidgetRect);
    btnMacAddr->setStyleSheet(SheetStyle::GROUPBUTTON_BOTTOMBORDER);
    btnMacAddr->setText(tr("MAC地址"));
    btnMacAddr->setFixedHeight(this->screenHeight()*0.08);
    btnMacAddr->setContentsMargins(0,0,this->screenWidth()*0.03, 0);

    btnDescribe = new GroupButton(this);
    btnDescribe->setAttribute(Qt::WA_LayoutUsesWidgetRect);
    btnDescribe->setStyleSheet(SheetStyle::GROUPBUTTON_NOBORDER);
    btnDescribe->setText(tr("描述"));
    btnDescribe->setFixedHeight(this->screenHeight()*0.08);
    btnDescribe->setContentsMargins(0,0,this->screenWidth()*0.03, 0);

    ListWidget* clusInfoList = new ListWidget(this);
    clusInfoList->setStyleSheet(SheetStyle::LISTWIDGET);
    clusInfoList->addWidget(btnMacAddr);
    clusInfoList->addWidget(btnDescribe);
    clusInfoList->setContentsMargins(this->screenWidth()*0.05,0,0,0);

    btnClaimDevice = new QPushButton(this);
    btnClaimDevice->setText(tr("设备认领"));
    btnClaimDevice->setFixedHeight(this->bottomBarHeight());
    btnClaimDevice->setStyleSheet("background-color:white; border-radius:3px; font:15px; color:#0090ff;");
    connect(btnClaimDevice, SIGNAL(clicked()), this, SLOT(on_btnClaimDevice_clicked()));

    QVBoxLayout* vbClaimDevice = new QVBoxLayout;
    vbClaimDevice->addWidget(btnClaimDevice);
    vbClaimDevice->setSpacing(0);
    vbClaimDevice->setMargin(0);
    this->setBottombarLayout(vbClaimDevice);

    QVBoxLayout* vbTotalLayout = new QVBoxLayout;
    vbTotalLayout->addWidget(btnTitleInfo);
    vbTotalLayout->addWidget(clusInfoList);
    vbTotalLayout->addSpacing(this->screenHeight()*0.05);
    vbTotalLayout->setMargin(0);
    vbTotalLayout->setSpacing(0);
    vbTotalLayout->addStretch(0);
    this->setBodyPartLayout(vbTotalLayout);

    //屏幕触摸滚动设置
    this->setBodyScreenHeight(this->scrollAreaHasBottomBarHeight());
    this->installScrollViewportArea();
    timerID = this->startTimer(ConstNum::MAX_TIME_INTERVAL);
}

void DClusterClaimDevicePage::fetchData(InputPacket &inpack)
{
    quint16 cmdCode = inpack.getCmdCode();
    switch (cmdCode) {
    case CommandCode::FETCH_DCLUSTER_DEVICE_INFO:
        handleDeviceInfo(inpack);
        break;
    }
    this->transferDataToAnotherPage(inpack, clusterClaimDevicePwdPage);
}

void DClusterClaimDevicePage::timerEvent(QTimerEvent*)
{
    //获取设备详细信息
    AssemblyData assemblyData;
    assemblyData.append(curClusterID);
    assemblyData.append(curDeviceID);
    char* data = assemblyData.getAssemblyData();
    quint16 dataLen = assemblyData.getAssemblyDataLen();
    emit readyRead(CommandCode::FETCH_DCLUSTER_DEVICE_INFO, data, dataLen);
    this->killTimer(timerID);
}

void DClusterClaimDevicePage::handleDeviceInfo(InputPacket &inpack)
{
    char* data = inpack.getPackData();
    SAFE_DELETE(deviceInfo);
    deviceInfo = new DClusterDevInfo;
    deviceInfo->unpack(data);
}

void DClusterClaimDevicePage::displayConcretePage(BasePage *page)
{
    if(page == NULL) return;
    page->show();
    connect(page, SIGNAL(destroyed()), this, SLOT(on_deleteSubPages()));
    connect(page, SIGNAL(readyRead(quint16,char*,quint16)), this, SLOT(on_recvDataFromOtherPage(quint16,char*,quint16)));
}

void DClusterClaimDevicePage::transferDataToAnotherPage(InputPacket &inpack, BasePage *basePage)
{
    if(basePage == NULL) return;
    basePage->fetchData(inpack);
}

void DClusterClaimDevicePage::on_btnClaimDevice_clicked()
{
    clusterClaimDevicePwdPage = new DClusterClaimDevicePwdPage(this, curClusterID, curDeviceID);
    this->displayConcretePage(clusterClaimDevicePwdPage);
}

void DClusterClaimDevicePage::on_deleteSubPages()
{
    clusterClaimDevicePwdPage = NULL;
}

void DClusterClaimDevicePage::on_recvDataFromOtherPage(quint16 cmdCode, char* data, quint16 dataLen)
{
    emit readyRead(cmdCode, data, dataLen);
}

void DClusterClaimDevicePage::on_btnPrePage_clicked()
{
    this->hide();
    this->deleteLater();
}

DClusterClaimDevicePage::~DClusterClaimDevicePage()
{

}
