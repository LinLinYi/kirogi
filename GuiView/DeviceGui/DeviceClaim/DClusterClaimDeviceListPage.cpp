#include <QVBoxLayout>
#include <QHBoxLayout>

#include "UserProtocol.hpp"
#include "AssemblyData.h"
#include "NavigationBar.h"
#include "ImageConfig.hpp"
#include "InputPacket.h"
#include "CheckButton.h"
#include "NavigationBar.h"
#include "DClusterDevInfoList.h"
#include "DClusterClaimDevicePage.h"
#include "DClusterClaimDevicePwdPage.h"
#include "SheetStyleConfig.hpp"
#include "DClusterClaimDeviceListPage.h"

DClusterClaimDeviceListPage::DClusterClaimDeviceListPage(QWidget* parent, quint64 clusID)
    :BasePage(parent)
    ,  curClusterID(clusID)
    , claimDeviceList(NULL)
    , clusterClaimDevicePage(NULL)
    , clusterClaimDevicePwdPage(NULL)
{
    this->initializeWidget();
}

void DClusterClaimDeviceListPage::initializeWidget()
{
    //导航栏
    QString strTitle = tr("设备认领");
    QString strLeftBtn = tr("返回");
    navigationBar = new NavigationBar(this);
    navigationBar->setTitleText(strTitle);
    navigationBar->setLeftText(strLeftBtn);
    navigationBar->setLeftPixMap(QPixmap(ImagePath::NAVIGATION_LEFT_ARROWS));
    connect(navigationBar, SIGNAL(leftClicked()), this, SLOT(on_btnPrePage_clicked()));

    QHBoxLayout* pHLTop = new QHBoxLayout();
    pHLTop->addWidget(navigationBar);
    pHLTop->setSpacing(0);
    pHLTop->setMargin(0);
    this->setTopbarLayout(pHLTop);

    //设备列表
    devClaimListLayout = new QVBoxLayout;
    devClaimListLayout->setMargin(0);
    devClaimListLayout->setSpacing(0);

    //初始化消息列表
    this->setBodyPartLayout(devClaimListLayout);
    this->installScrollViewportArea();
    timerID = this->startTimer(ConstNum::MAX_TIME_INTERVAL);
}

void DClusterClaimDeviceListPage::fetchData(InputPacket &inpack)
{
    quint16 cmdCode = inpack.getCmdCode();
    switch(cmdCode)
    {
        case CommandCode::DCLUSTER_CLAIM_DEVICE:
            this->handleDeviceClaim(inpack);
            break;
        case CommandCode::DCLUSTER_CLAIM_DEVICE_LIST:
            this->handleDeviceClaimList(inpack);
            break;
    }
    this->transferDataToAnotherPage(inpack, clusterClaimDevicePage);
    this->transferDataToAnotherPage(inpack, clusterClaimDevicePwdPage);
}

void DClusterClaimDeviceListPage::timerEvent(QTimerEvent*)
{
    AssemblyData assemblyData;
    assemblyData.append(curClusterID);
    char* data = assemblyData.getAssemblyData();
    quint16 dataLen = assemblyData.getAssemblyDataLen();
    emit readyRead(CommandCode::DCLUSTER_CLAIM_DEVICE_LIST, data, dataLen);
    this->killTimer(timerID);
}

void DClusterClaimDeviceListPage::handleDeviceClaim(InputPacket &inpack)
{
    if(inpack.getAnswerCode() == Mi::Success){
        curCheckButton->setReviewText(tr("认领中..."));
        curCheckButton->setReviewEnable(false);
    }
}

void DClusterClaimDeviceListPage::handleDeviceClaimList(InputPacket &inpack)
{
    if(claimDeviceList == NULL)
        claimDeviceList = new DClusterDevInfoList;

    char* data = inpack.getPackData();
    quint16 dataLen = inpack.getPackDataLen();
    claimDeviceList->appendList(data, dataLen);
    QHash<quint64, DClusterDevInfo*> claimList = claimDeviceList->fetchList();

    QHashIterator<quint64, DClusterDevInfo*> iter(claimList);
    while(iter.hasNext()){
        iter.next();
        DClusterDevInfo* info = iter.value();
        this->setDeviceClaimWidget(info);
    }
}

void DClusterClaimDeviceListPage::displayConcretePage(BasePage *page)
{
    if(page == NULL) return;
    page->show();
    connect(page, SIGNAL(destroyed()), this, SLOT(on_deleteSubPages()));
    connect(page, SIGNAL(readyRead(quint16,char*,quint16)), this, SLOT(on_recvDataFromOtherPage(quint16,char*,quint16)));
}

void DClusterClaimDeviceListPage::transferDataToAnotherPage(InputPacket &inpack, BasePage *basePage)
{
    if(basePage == NULL) return;
    basePage->fetchData(inpack);
}

void DClusterClaimDeviceListPage::setDeviceClaimWidget(DClusterDevInfo* info)
{
    CheckButton* btn = new CheckButton(this);
    btn->setUserID(info->getDeviceID());
    btn->setUserName(info->getDeviceName());
    btn->setClusterID(info->getClusterID());

    QPixmap pixmap(ImagePath::USER_PORTRAIT);
    pixmap.setDevicePixelRatio(2.0);
    btn->setUserName(info->getDeviceName());
    btn->setPixMap(pixmap);
    btn->setFixedHeight(this->screenHeight()*0.1);
    btn->setFixedWidth(this->screenWidth());
    btn->setStyleSheet(SheetStyle::CHECKBUTTON_BOTTOM_BORDER);
    btn->setReviewText("认领");

    connect(btn, SIGNAL(clicked()), this, SLOT(on_btnClaimPage_clicked()));
    connect(btn, SIGNAL(reviewClicked()), this, SLOT(on_btnClaim_clicked()));

    if(devClaimListLayout->count() == 0){
        devClaimListLayout->addWidget(btn);
        devClaimListLayout->addStretch(0);
    }else{
        devClaimListLayout->insertWidget(devClaimListLayout->count() - 1, btn);
    }
}

void DClusterClaimDeviceListPage::on_btnClaimPage_clicked()
{
    CheckButton* btnThis = dynamic_cast<CheckButton*>(sender());
    curCheckButton = btnThis;
    quint64 devID = btnThis->getUserID();

    clusterClaimDevicePage = new DClusterClaimDevicePage(this, curClusterID, devID);
    this->displayConcretePage(clusterClaimDevicePage);
}

void DClusterClaimDeviceListPage::on_btnClaim_clicked()
{
    CheckButton* btnThis = dynamic_cast<CheckButton*>(sender());
    curCheckButton = btnThis;
    quint64 devID =btnThis->getUserID();

    clusterClaimDevicePwdPage = new DClusterClaimDevicePwdPage(this, curClusterID, devID);
    this->displayConcretePage(clusterClaimDevicePwdPage);
}

void DClusterClaimDeviceListPage::on_deleteSubPages()
{
    clusterClaimDevicePage = NULL;
    clusterClaimDevicePwdPage = NULL;
}

void DClusterClaimDeviceListPage::on_recvDataFromOtherPage(quint16 cmdCode, char *data, quint16 dataLen)
{
    emit readyRead(cmdCode, data, dataLen);
}

void DClusterClaimDeviceListPage::setClusterID(quint64 clusID)
{
    curClusterID = clusID;
}

void DClusterClaimDeviceListPage::on_btnPrePage_clicked()
{
    this->hide();
    this->deleteLater();
}

DClusterClaimDeviceListPage::~DClusterClaimDeviceListPage()
{
    SAFE_DELETE(claimDeviceList);
}
