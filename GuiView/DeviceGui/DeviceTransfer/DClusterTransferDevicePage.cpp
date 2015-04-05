#include <QVBoxLayout>

#include <QLabel>
#include "GlobalDefine.hpp"
#include "UserProtocol.hpp"
#include "HintMsgPage.h"
#include "InputPacket.h"
#include "AssemblyData.h"
#include "NavigationBar.h"
#include "GroupButton.h"
#include "LoadingPage.h"
#include "ImageConfig.hpp"
#include "SheetStyleConfig.hpp"
#include "DClusterDevInfoList.h"
#include "DClusterTransferDevicePage.h"

DClusterTransferDevicePage::DClusterTransferDevicePage(QWidget* parent, quint64 clusterID)
    : BasePage(parent)
    , curClusterID(clusterID)
    , curFixedContentHeight(0)
    , deviceTransferCount(0)
    , loadingPage(NULL)
    , deviceInfoList(NULL)
{
    this->initializeWidget();
}

void DClusterTransferDevicePage::initializeWidget()
{
    //提示信息页面操作
    hintMsgPage = new HintMsgPage(this);
    hintMsgPage->hide();
    connect(hintMsgPage, SIGNAL(submitClicked(bool)), this, SLOT(on_hintMsgPage(bool)));

    //导航栏
    QString strTitle = "移交设备";
    QString strLeftBtn = "返回";
    QString strRightBtn = "移交";
    QPixmap leftArrowsPixmap(ImagePath::NAVIGATION_LEFT_ARROWS);
    navigationBar = new NavigationBar(this);
    navigationBar->setTitleText(strTitle);
    navigationBar->setLeftText(strLeftBtn);
    navigationBar->setRightText(strRightBtn);
    navigationBar->setLeftPixMap(leftArrowsPixmap);
    connect(navigationBar, SIGNAL(leftClicked()), this, SLOT(on_btnPrePage_clicked()));
    connect(navigationBar, SIGNAL(rightClicked()), this, SLOT(on_transferDevice_clicked()));

    //提示信息
    lblHint = new QLabel(this);
    lblHint->setAlignment(Qt::AlignCenter);
    lblHint->hide();

    QHBoxLayout* pHLTop = new QHBoxLayout();
    pHLTop->addWidget(navigationBar);
    pHLTop->setSpacing(0);
    pHLTop->setMargin(0);
    this->setTopbarLayout(pHLTop);

    //群成员布局
    deviceListLayout = new QVBoxLayout();
    deviceListLayout->setMargin(0);
    deviceListLayout->setSpacing(0);
    deviceListLayout->setContentsMargins(0, 0, 0, 0);

    QVBoxLayout* vblTotalLayout = new QVBoxLayout();
    vblTotalLayout->addWidget(lblHint);
    vblTotalLayout->addLayout(deviceListLayout);
    vblTotalLayout->addStretch(0);
    vblTotalLayout->setMargin(0);
    vblTotalLayout->setSpacing(0);
    vblTotalLayout->setAlignment(Qt::AlignTop);
    this->setBodyPartLayout(vblTotalLayout);

    //屏幕触摸滚动设置
    this->installScrollViewportArea();
    timerID = this->startTimer(ConstNum::MAX_TIME_INTERVAL);
}

void DClusterTransferDevicePage::fetchData(InputPacket &inpack)
{
    quint16 cmdCode = inpack.getCmdCode();
    switch(cmdCode)
    {
        case CommandCode::NET_ERROR:
            this->handleNetError();
            break;
        case CommandCode::DCLUSTER_DEV_TRANSFER_REQ:
            this->handleTransferDeviceInfo(inpack);
            break;
        case CommandCode::FETCH_DCLUSTER_DEVICE_LIST:
            this->handleTransferDeviceList(inpack);
            break;
    }
    DPRINT("-->DevClusterTransferDevicePage receive data, the cmdCode is:0x%x", cmdCode);
}

void DClusterTransferDevicePage::timerEvent(QTimerEvent*)
{
    AssemblyData assemblyData;
    assemblyData.append(curClusterID);
    char* data = assemblyData.getAssemblyData();
    quint16 dataLen = assemblyData.getAssemblyDataLen();
    emit readyRead(CommandCode::FETCH_DCLUSTER_DEVICE_LIST, data, dataLen);
    this->killTimer(timerID);
}

void DClusterTransferDevicePage::handleNetError()
{
    if(loadingPage == NULL) return;
    QString lblText = "网络错误, 请重试";
    this->handleHintInfo(lblText);
}

void DClusterTransferDevicePage::handleHintInfo(QString& text)
{
    hintMsgPage->setText(text);
    hintMsgPage->show();
    SAFE_DELETE(loadingPage);
}

void DClusterTransferDevicePage::setTranClusterID(quint64 id)
{
    tranClusterID = id;
}

void DClusterTransferDevicePage::handleTransferDeviceList(InputPacket &inpack)
{
    char* data = inpack.getPackData();
    quint16 dataLen = inpack.getPackDataLen();

    SAFE_DELETE(deviceInfoList);
    deviceInfoList = new DClusterDevInfoList;
    deviceInfoList->appendList(data, dataLen);
    QHash<quint64, DClusterDevInfo*> devList = deviceInfoList->fetchList();
    if(devList.count() < 1){
        lblHint->setText(tr("不存在可移交的设备"));
        return;
    }

    QHashIterator<quint64, DClusterDevInfo*> iter(devList);
    while(iter.hasNext())
    {
        iter.next();
        DClusterDevInfo* devInfo = dynamic_cast<DClusterDevInfo*>(iter.value());
        this->setDeviceButton(devInfo);

        if(curFixedContentHeight <= this->bodyPartHeight())
            this->setBodyRealHeight(this->bodyPartHeight());
        else
            this->setBodyRealHeight(curFixedContentHeight);
    }
}

void DClusterTransferDevicePage::handleTransferDeviceInfo(InputPacket& inpack)
{
    quint8 ansCode = inpack.getAnswerCode();
    if(ansCode == Mi::Success){
        QString strText = "设备移交发送成功";
        hintMsgPage->setStatus(true);
        this->handleHintInfo(strText);
    }else if(ansCode == Mi::Failure){
        QString strText = "设备移交发送失败, 请重试";
        this->handleHintInfo(strText);
    }
}

void DClusterTransferDevicePage::setDeviceButton(DClusterDevInfo *devInfo)
{
    if(devInfo == NULL) return;

    QPixmap pixmap(ImagePath::DEVICE_PORTRAIT);
    pixmap.setDevicePixelRatio(2.0);

    QPixmap pixmapReserve(ImagePath::SELECT_CANCEL);
    pixmapReserve.setDevicePixelRatio(2.0);

    //设备按钮设置
    GroupButton* btn = new GroupButton(this);
    btn->setIndexOne(devInfo->getDeviceID());
    btn->setText(devInfo->getDeviceName());
    btn->setPixMap(pixmap);
    btn->setFixedHeight(this->screenHeight()*0.1);
    btn->setFixedWidth(this->screenWidth());
    btn->setStyleSheet(SheetStyle::GROUPBUTTON_BOTTOMBORDER);
    btn->setReserve(pixmapReserve);
    btn->setAttribute(Qt::WA_LayoutUsesWidgetRect);
    devicesBtnList.insert(devInfo->getDeviceID(), btn);
    connect(btn, SIGNAL(clicked()), this, SLOT(on_btnDeviceButton_clicked()));

    deviceListLayout->addWidget(btn);
    curFixedContentHeight += btn->height();
    this->repaint();
}

void DClusterTransferDevicePage::on_transferDevice_clicked()
{
    if(deviceTransferCount == 0){
        QString strText = "请选择移交的设备";
        this->handleHintInfo(strText);
        return;
    }

    QHashIterator<quint64, GroupButton*> iter(devicesBtnList);
    AssemblyData assemblyData;
    assemblyData.append(curClusterID);
    assemblyData.append(tranClusterID);
    assemblyData.append(deviceTransferCount);

    while(iter.hasNext()){
        iter.next();
        GroupButton* groupButton = iter.value();
        if(groupButton->getSelectFlag()){
            quint64 deviceID = groupButton->getIndexOne();
            assemblyData.append(deviceID);
        }
    }

    char* data = assemblyData.getAssemblyData();
    quint16 dataLen = assemblyData.getAssemblyDataLen();
    emit readyRead(CommandCode::DCLUSTER_DEV_TRANSFER_REQ, data, dataLen);

    //页面提示信息
    if(loadingPage == NULL){
        loadingPage = new LoadingPage(this);
        loadingPage->show();
    }
}

void DClusterTransferDevicePage::on_btnDeviceButton_clicked()
{
    GroupButton* groupButton = dynamic_cast<GroupButton*>(sender());
    QPixmap pixmapBtnDelete(ImagePath::SELECT_SUBMIT);
    pixmapBtnDelete.setDevicePixelRatio(2.0);

    QPixmap pixmapBtnCancel(ImagePath::SELECT_CANCEL);
    pixmapBtnCancel.setDevicePixelRatio(2.0);

    if(groupButton->getSelectFlag() == false){
        groupButton->setReserve(pixmapBtnDelete);
        groupButton->setSelectFlag(true);
        deviceTransferCount++;
    }
    else{
        groupButton->setReserve(pixmapBtnCancel);
        groupButton->setSelectFlag(false);
        deviceTransferCount--;
    }

    if(deviceTransferCount > 0){
        QString strLeftBtn = "取消";
        QPixmap leftArrowsPixmap("");
        navigationBar->setLeftText(strLeftBtn);
        navigationBar->setLeftPixMap(leftArrowsPixmap);
        disconnect(navigationBar, SIGNAL(leftClicked()), this, SLOT(on_btnPrePage_clicked()));
        connect(navigationBar, SIGNAL(leftClicked()), this, SLOT(on_btnCancel_clicked()));
    }
    else{
        QString strLeftBtn = "返回";
        QPixmap leftArrowsPixmap(ImagePath::NAVIGATION_LEFT_ARROWS);
        navigationBar->setLeftText(strLeftBtn);
        navigationBar->setLeftPixMap(leftArrowsPixmap);
        disconnect(navigationBar, SIGNAL(leftClicked()), this, SLOT(on_btnCancel_clicked()));
        connect(navigationBar, SIGNAL(leftClicked()), this, SLOT(on_btnPrePage_clicked()));
    }
}

void DClusterTransferDevicePage::on_btnCancel_clicked()
{
    QHashIterator<quint64, GroupButton*> iter(devicesBtnList);
    QPixmap pixmapBtnCancel(ImagePath::SELECT_CANCEL);
    pixmapBtnCancel.setDevicePixelRatio(2.0);

    while(iter.hasNext()){
        iter.next();
        GroupButton* groupButton = iter.value();
        if(groupButton->getSelectFlag()){
            groupButton->setReserve(pixmapBtnCancel);
            groupButton->setSelectFlag(false);
            deviceTransferCount--;
        }
    }

    QString strLeftBtn = "返回";
    QPixmap leftArrowsPixmap(ImagePath::NAVIGATION_LEFT_ARROWS);
    navigationBar->setLeftText(strLeftBtn);
    navigationBar->setLeftPixMap(leftArrowsPixmap);
    disconnect(navigationBar, SIGNAL(leftClicked()), this, SLOT(on_btnCancel_clicked()));
    connect(navigationBar, SIGNAL(leftClicked()), this, SLOT(on_btnPrePage_clicked()));
}

void DClusterTransferDevicePage::on_hintMsgPage(bool isJump)
{
    if(!isJump)
        hintMsgPage->hide();
    else
        emit readyRead(CommandCode::BACK_CLUSTER_MANAGE_PAGE, NULL, 0);
}

void DClusterTransferDevicePage::on_btnPrePage_clicked()
{
    this->hide();
    this->deleteLater();
}

DClusterTransferDevicePage::~DClusterTransferDevicePage()
{
    SAFE_DELETE(deviceInfoList);
}
