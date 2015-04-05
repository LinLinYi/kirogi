#include <QVBoxLayout>

#include "InputPacket.h"
#include "AssemblyData.h"
#include "UserProtocol.hpp"
#include "NavigationBar.h"
#include "ImageConfig.hpp"
#include "ListWidget.h"
#include "GroupButton.h"
#include "NavigationBar.h"
#include "DClusterInfo.h"
#include "HintMsgPage.h"
#include "LoadingPage.h"
#include "DeviceTransNotify.h"
#include "SheetStyleConfig.hpp"
#include "DeviceTransferInfoList.h"
#include "DClusterTransferReviewPage.h"

DClusterTransferReviewPage::DClusterTransferReviewPage(QWidget* parent, quint64 userID, quint64 srcID, quint64 destID)
    :BasePage(parent)
    , srcUserID(userID)
    , srcClusterID(srcID)
    , destClusterID(destID)
    , deviceInfoList(NULL)
    , hintMsgPage(NULL)
    , loadingPage(NULL)
{
    curFixedContentHeight = 0;
    this->initializeWidget();
}

void DClusterTransferReviewPage::initializeWidget()
{
    //提示信息页面操作
    hintMsgPage = new HintMsgPage(this);
    hintMsgPage->hide();
    connect(hintMsgPage, SIGNAL(submitClicked(bool)), this, SLOT(on_hintMsgPage(bool)));

    //导航栏
    QString strLeftBtn = tr("拒绝");
    QString strRightBtn = tr("同意");
    QString strTitle = tr("移交设备列表");
    navigationBar = new NavigationBar(this);
    navigationBar->setLeftText(strLeftBtn);
    navigationBar->setRightText(strRightBtn);
    navigationBar->setTitleText(strTitle);
    connect(navigationBar, SIGNAL(leftClicked()), this, SLOT(on_btnRefuse_clicked()));
    connect(navigationBar, SIGNAL(rightClicked()), this, SLOT(on_btnAgree_clicked()));

    QHBoxLayout* pHLTop = new QHBoxLayout();
    pHLTop->addWidget(navigationBar);
    pHLTop->setSpacing(0);
    pHLTop->setMargin(0);
    this->setTopbarLayout(pHLTop);

    //群设备布局
    devListWidget = new ListWidget(this);
    devListWidget->setStyleSheet(SheetStyle::LISTWIDGET);
    devListWidget->setContentsMargins(this->screenWidth()*0.03,0,0,0);
    curFixedContentHeight += devListWidget->height();

    QVBoxLayout* vblTotalLayout = new QVBoxLayout();
    vblTotalLayout->addWidget(devListWidget);
    vblTotalLayout->addStretch(0);
    vblTotalLayout->setMargin(0);
    vblTotalLayout->setSpacing(0);
    vblTotalLayout->setAlignment(Qt::AlignTop);
    this->setBodyPartLayout(vblTotalLayout);

    //初始化页面数据
    deviceInfoList = new DeviceTransferInfoList;
    deviceInfoList->loadDataSource(srcClusterID, destClusterID);
    this->deployDeviceList(deviceInfoList->fetchList());

    //屏幕触摸滚动设置
    this->installScrollViewportArea();
    this->setBodyRealHeight(curFixedContentHeight);
}

void DClusterTransferReviewPage::fetchData(InputPacket &inpack)
{
    quint16 cmdCode = inpack.getCmdCode();
    DPRINT("-->DevClusterDeviceListPage receive data, the cmdCode is:0x%x", cmdCode);

    switch(cmdCode)
    {
        case CommandCode::DCLUSTER_DEV_TRANSFER_APPROVAL:
            this->handleApprovalResult(inpack);
            break;
        case CommandCode::NET_ERROR:
            this->handleNetError();
            break;
    }
}

void DClusterTransferReviewPage::handleApprovalResult(InputPacket &inpack)
{
    quint16 ansCode = inpack.getAnswerCode();
    if(ansCode == Mi::Success){
        DeviceTransNotify notifyInfo;
        notifyInfo.updateInfoStatus(curReviewStatus, srcUserID, srcClusterID, destClusterID);

        QString strText = tr("审核成功");
        hintMsgPage->setStatus(true);
        this->handleHintInfo(strText);
    }else if(ansCode == Mi::Failure){
        QString strText = tr("审核失败,请重试");
        this->handleHintInfo(strText);
    }else
        SAFE_DELETE(loadingPage)
}

void DClusterTransferReviewPage::handleHintInfo(QString &str)
{
    hintMsgPage->setText(str);
    hintMsgPage->show();
    SAFE_DELETE(loadingPage);
}

void DClusterTransferReviewPage::handleNetError()
{
    if(loadingPage == NULL) return;
    QString strText = tr("网络错误,请重试");
    this->handleHintInfo(strText);
}

void DClusterTransferReviewPage::deployDeviceList(QHash<quint64, DeviceTransferInfo *> list)
{
    QHashIterator<quint64, DeviceTransferInfo*> iter(list);
    while(iter.hasNext())
    {
        iter.next();
        DeviceTransferInfo* devInfo = dynamic_cast<DeviceTransferInfo*>(iter.value());
        this->appendDeviceToListWidget(devInfo);
    }

    qint16 height = curFixedContentHeight + devListWidget->height();
    this->setBodyRealHeight(height);
}

void DClusterTransferReviewPage::appendDeviceToListWidget(DeviceTransferInfo *info)
{
    GroupButton* btn = NULL;
    for(int i = 0; i < devListWidget->count(); i++){
        QWidget* widget = devListWidget->itemAt(i);
        btn = dynamic_cast<GroupButton*>(widget);

        if(btn->getIndexOne() == info->getDeviceID()){//存在，则更新
            this->setDeviceButton(btn, info);
            return;
        }
    }

    //不存在，则加入
    btn = new GroupButton(this);
    this->setDeviceButton(btn, info);
    devListWidget->addWidget(btn);

    qint16 height = curFixedContentHeight + devListWidget->height();
    this->setBodyRealHeight(height);
}

void DClusterTransferReviewPage::setDeviceButton(GroupButton* btn, DeviceTransferInfo* info)
{
    if(btn == NULL || info == NULL) return;

    //设备按钮设置
    QPixmap pixmap(ImagePath::DEVICE_PORTRAIT);
    pixmap.setDevicePixelRatio(2.0);
    btn->setIndexOne(info->getDeviceID());
    btn->setText(info->getDeviceName());
    btn->setPixMap(pixmap);
    btn->setFixedHeight(this->screenHeight()*0.1);
    btn->setFixedWidth(this->screenWidth());
    btn->setAttribute(Qt::WA_LayoutUsesWidgetRect);
    btn->setContentsMargins(0,0,0,0);
    this->listWidgetLastWidgetStyle(devListWidget);
    //connect(btn, SIGNAL(clicked()), this, SLOT(on_btnDevClusterDeviceInfo_clicked()));
}

void DClusterTransferReviewPage::displayConcretePage(BasePage *page)
{
    if(page == NULL) return;
    page->show();
    connect(page, SIGNAL(destroyed()), this, SLOT(on_deleteSubPages()));
    connect(page, SIGNAL(readyRead(quint16,char*,quint16)), this, SLOT(on_recvDataFromOtherPage(quint16,char*,quint16)));
}

void DClusterTransferReviewPage::listWidgetLastWidgetStyle(ListWidget *listWidget)
{
    for(int i = 0; i < listWidget->count(); i++){
        QWidget* widget = listWidget->itemAt(i);
        widget->setStyleSheet(SheetStyle::GROUPBUTTON_BOTTOMBORDER);
        if(i == (listWidget->count() - 1)){
            widget->setStyleSheet(SheetStyle::GROUPBUTTON_NOBORDER);
        }
    }
}

void DClusterTransferReviewPage::transferDataToAnotherPage(InputPacket &inpack, BasePage *basePage)
{
    if(basePage == NULL) return;
    basePage->fetchData(inpack);
}

void DClusterTransferReviewPage::on_recvDataFromOtherPage(quint16 cmdCode, char *data, quint16 dataLen)
{

}

void DClusterTransferReviewPage::on_btnRefuse_clicked()
{
    quint8 ansCode = Mi::Refused;
    AssemblyData assemblyData;
    assemblyData.append(ansCode);
    assemblyData.append(srcUserID);
    assemblyData.append(srcClusterID);
    assemblyData.append(destClusterID);

    char* data = assemblyData.getAssemblyData();
    quint16 dataLen = assemblyData.getAssemblyDataLen();
    emit readyRead(CommandCode::DCLUSTER_DEV_TRANSFER_APPROVAL, data, dataLen);
}

void DClusterTransferReviewPage::on_btnAgree_clicked()
{
    quint8 ansCode = Mi::Agreed;
    AssemblyData assemblyData;
    assemblyData.append(ansCode);
    assemblyData.append(srcUserID);
    assemblyData.append(srcClusterID);
    assemblyData.append(destClusterID);

    DeviceTransferInfoList infoList;
    infoList.loadDataSource(srcClusterID, destClusterID);

    QHash<quint64, DeviceTransferInfo*> hashList = infoList.fetchList();
    QHashIterator<quint64, DeviceTransferInfo*> iter(hashList);

    quint16 devCount = hashList.count();
    assemblyData.append(devCount);

    while (iter.hasNext()) {
        iter.next();
        DeviceTransferInfo* info = iter.value();
        quint64 devID = info->getDeviceID();
        assemblyData.append(devID);
    }

    char* data = assemblyData.getAssemblyData();
    quint16 dataLen = assemblyData.getAssemblyDataLen();
    emit readyRead(CommandCode::DCLUSTER_DEV_TRANSFER_APPROVAL, data, dataLen);
}

void DClusterTransferReviewPage::on_hintMsgPage(bool isJump)
{
    hintMsgPage->hide();
    if(!isJump) return;
    this->hide();
    this->deleteLater();
}

DClusterTransferReviewPage::~DClusterTransferReviewPage()
{
    SAFE_DELETE(deviceInfoList);
}


