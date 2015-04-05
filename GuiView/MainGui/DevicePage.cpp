#include <QGridLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QDateTime>
#include <cstring>
#include <QLabel>

#include "GroupButton.h"
#include "DevicePage.h"
#include "ServerNotifyInfo.h"
#include "UserProtocol.hpp"
#include "ImageConfig.hpp"
#include "SheetStyleConfig.hpp"
#include "NavigationBar.h"
#include "InputPacket.h"
#include "ListWidget.h"

#include "MyDeviceInfoList.h"
#include "DClusterInfoList.h"
#include "DNotifySummaryList.h"
#include "OnlineStatusInfoList.h"
#include "DeviceTransferInfoList.h"
#include "DClusterUserInfoList.h"
#include "DClusterDevInfoList.h"

#include "DClusterJoinNotify.h"
#include "DeviceTransNotify.h"
#include "HintMsgButton.h"
#include "HintToolButton.h"
#include "DispersalData.h"
#include "AssemblyData.h"

#include "DeviceManagementPage.h"
#include "DClusterNotifyListPage.h"
#include "DClusterManagementPage.h"
#include "DClusterTransferNotifyListPage.h"

DevicePage::DevicePage(QWidget *parent, PageType type)
    :BasePage(parent, type)
    , curFixedContentHeight(0)
    , transferDeviceNotifyListPage(NULL)
    , clusterNotifyListPage(NULL)
    , clusterManagementPage(NULL)
    , deviceManagementPage(NULL)
    , clusterInfoList(NULL)
    , myDeviceInfoList(NULL)
    , myDeviceOnlineList(NULL)
    , notifySummaryList(NULL)
{
    this->initializeWidget();
}

void DevicePage::initializeWidget()
{
    //导航栏
    QString strTitle = tr("设备");
    QPixmap pixRightBtn = QPixmap(ImagePath::ADDTION);
    navigationBar = new NavigationBar(this);
    navigationBar->setTitleText(strTitle);
    navigationBar->setRightPixMap(pixRightBtn);
    connect(navigationBar, SIGNAL(rightClicked()), this, SLOT(on_btnAddDevices_clicked()));

    QHBoxLayout* pHLTop = new QHBoxLayout();
    pHLTop->addWidget(navigationBar);
    pHLTop->setSpacing(0);
    pHLTop->setMargin(0);
    this->setTopbarLayout(pHLTop);

    //信息分类
    btnMessage = new HintToolButton(this);
    btnMessage->setText(QString(tr("消息")));
    this->setToolButton(btnMessage, ImagePath::DEV_MESSAGE_CANCELED);
    connect(btnMessage, SIGNAL(clicked()), this, SLOT(on_toolBtnMessage_clicked()));

    btnCluster = new QToolButton(this);
    btnCluster->setText(QString(tr("群组")));
    this->setToolButton(btnCluster, ImagePath::DEV_CLUSTER_CANCELED);
    connect(btnCluster, SIGNAL(clicked()), this, SLOT(on_toolBtnCluster_clicked()));

    btnDevice = new QToolButton(this);
    btnDevice->setText(QString(tr("设备")));
    this->setToolButton(btnDevice, ImagePath::MYDEVICE_SELECTED);
    btnMessage->setStyleSheet(SheetStyle::TOOLBUTTON_BLUE_15);
    connect(btnDevice, SIGNAL(clicked()), this, SLOT(on_toolBtnDevice_clicked()));

    //信息详细内容
    deviceListWidget = new ListWidget(this);
    deviceListWidget->hide();
    this->setListWidget(deviceListWidget);

    messageListWidget = new ListWidget(this);
    messageListWidget->hide();
    this->setListWidget(messageListWidget);

    clusterListWidget = new ListWidget(this);
    clusterListWidget->hide();
    this->setListWidget(clusterListWidget);

    curListWidget = deviceListWidget;
    curListWidget->show();

    QHBoxLayout* msgBtnLayout = new QHBoxLayout();
    msgBtnLayout->addWidget(btnDevice);
    msgBtnLayout->addWidget(btnMessage);
    msgBtnLayout->addWidget(btnCluster);
    msgBtnLayout->setContentsMargins(this->screenWidth()*0.05, 0 , this->screenWidth()*0.05, 0);

    QFrame* selectFrame = new QFrame(this);
    selectFrame->setLayout(msgBtnLayout);
    selectFrame->setStyleSheet("border:none;background-color:white;");
    selectFrame->setFixedHeight(this->screenHeight()*0.11);
    curFixedContentHeight += selectFrame->height();

    //设备提示
    lblInfoHint = new QLabel(tr("我的设备"), this);
    lblInfoHint->setFixedHeight(this->screenHeight()*0.05);
    lblInfoHint->setFixedWidth(this->screenWidth());
    lblInfoHint->setContentsMargins(this->screenWidth()*0.03,0,0,0);
    lblInfoHint->setStyleSheet(SheetStyle::QLABEL);
    curFixedContentHeight += lblInfoHint->height();

    widgetsLayout = new QVBoxLayout();
    widgetsLayout->addWidget(selectFrame);
    widgetsLayout->addWidget(lblInfoHint);
    widgetsLayout->addWidget(curListWidget);
    widgetsLayout->setMargin(0);
    widgetsLayout->setSpacing(0);
    widgetsLayout->setAlignment(Qt::AlignTop);
    this->setBodyPartLayout(widgetsLayout);

    //屏幕触摸滚动设置
    this->setBodyScreenHeight(this->scrollAreaHasBottomBarHeight());
    this->installScrollViewportArea();
    this->loadLocalData();
    timerID = this->startTimer(ConstNum::MAX_TIME_INTERVAL);
}

void DevicePage::timerEvent( QTimerEvent * )
{
    emit readyRead(CommandCode::FETCH_DCLUSTER_LIST, NULL, 0);
    emit readyRead(CommandCode::FETCH_MY_DEVICE_LIST, NULL, 0);
    this->killTimer(timerID);
}

void DevicePage::loadLocalData()
{
    //消息通知信息列表
    notifySummaryList = new DNotifySummaryList;
    notifySummaryList->loadDataSource();
    this->deployMsgNotifyList(notifySummaryList->fetchList());

    //初始化设备群信息列表
    clusterInfoList = new DClusterInfoList;
    clusterInfoList->loadDataSource();
    if(clusterInfoList->fetchList().count() <= 0){
        lblInfoHint->setText(QString(tr("你还没有已授权设备")));
    }
    this->deployClusterInfoList(clusterInfoList->fetchList());

    //初始化我的设备列表
    myDeviceInfoList = new MyDeviceInfoList;
    myDeviceInfoList->loadDataSource();
    this->deployMyDeviceList(myDeviceInfoList->fetchList());
}

void DevicePage::fetchData(InputPacket& inpack)
{
    quint16 cmdID = inpack.getCmdCode();
    DPRINT("-->DevicePage receive data, the cmdCode is:0x%x", cmdID);

    switch(cmdID)
    {
        case CommandCode::FETCH_MY_DEVICE_LIST:
            this->recvMyDeviceListFromServer(inpack);
            break;
        case CommandCode::FETCH_MY_DEVICE_NETSTATUS_LIST:
            this->recvMyDeviceOnlineListFromServer(inpack);
            break;
        case CommandCode::FETCH_DCLUSTER_LIST:
            this->recvClusterListFromServer(inpack);
            break;
        case CommandCode::DCLUSTER_CREATE:
            this->recvCreatClusterResultFromServer(inpack);
            break;
        case CommandCode::FETCH_DCLUSTER_INFO:
            this->recvClusterInfoFromServer(inpack);
            break;
        case CommandCode::FETCH_DEVICE_INFO:
            this->recvDeviceInfoFromServer(inpack);
            break;
        case CommandCode::DCLUSTER_INVITE_APPROVAL:
            this->recvClusterJoinApprovalResult(inpack);
            break;
        case CommandCode::DCLUSTER_EXIT:
        case CommandCode::DCLUSTER_DELETE_VERIFY:
            this->recvDeleteClusterResult(inpack);
            break;
        case CommandCode::SERVER_NOTIFY:
            this->recvNotifyInfoFromServer(inpack);
            break;
    }

    this->labelHintInfoDisplay();
    this->transferDataToAnotherPage(inpack, deviceManagementPage);
    this->transferDataToAnotherPage(inpack, transferDeviceNotifyListPage);
    this->transferDataToAnotherPage(inpack, clusterManagementPage);
    this->transferDataToAnotherPage(inpack, clusterNotifyListPage);
}

void DevicePage::recvMyDeviceListFromServer(InputPacket& inpack)
{
    char* data = inpack.getPackData();
    quint16 dataLen = inpack.getPackDataLen();

    SAFE_DELETE(myDeviceInfoList);
    myDeviceInfoList = new MyDeviceInfoList;
    myDeviceInfoList->appendList(data, dataLen);
    this->deployMyDeviceList(myDeviceInfoList->fetchList());
}

void DevicePage::recvMyDeviceOnlineListFromServer(InputPacket& inpack)
{
    SAFE_DELETE(myDeviceOnlineList);
    myDeviceOnlineList = new OnlineStatusInfoList;

    char* data = inpack.getPackData();
    quint16 dataLen = inpack.getPackDataLen();
    myDeviceOnlineList->appendList(data, dataLen);

    QHash<quint64, OnlineStatusInfo*> onlineList = myDeviceOnlineList->fetchList();
    QHashIterator<quint64, OnlineStatusInfo*> iter(onlineList);
    while(iter.hasNext())
    {
        iter.next();
        quint64 onlineDevID = iter.value()->getID();
        Mi::NetStatus status = NetLine::numToNetstatus(iter.value()->getStatus());
        this->sortDeviceNetStatus(deviceListWidget, onlineDevID, status);
    }
}

void DevicePage::recvClusterListFromServer(InputPacket& inpack)
{
    char* data = inpack.getPackData();
    quint16 dataLen = inpack.getPackDataLen();

    SAFE_DELETE(clusterInfoList);
    clusterInfoList = new DClusterInfoList;
    clusterInfoList->appendList(data, dataLen);
    QHash<quint64, DClusterInfo*> list = clusterInfoList->fetchList();
    this->deployClusterInfoList(list);
}

void DevicePage::recvDeleteClusterResult(InputPacket &input)
{
    quint64 clusID = 0;
    quint16 cmdCode = input.getCmdCode();
    quint16 ansCode = input.getAnswerCode();

    //获取退出群ID
    DispersalData dispersalData;
    dispersalData.outlet(clusID, input.getPackData());

    //删除、退出、踢出群成功
    bool isExitCluster = (cmdCode == CommandCode::DCLUSTER_EXIT) && (ansCode == Mi::Success);
    bool isDelCluster = (cmdCode == CommandCode::DCLUSTER_DELETE_VERIFY) && (ansCode == Mi::Success);
    bool isKickoutCluster = (cmdCode == CommandCode::DCLUSTER_DELETE_USER);

    if(!(isExitCluster || isDelCluster) || isKickoutCluster) {
        return;
    }

    //从群列表删除群
    this->deleteClusterListWidgetItem(clusID);

    //删除群已授权设备(我的设备)
    this->deleteDeviceListWidgetItem(clusID);
}

void DevicePage::recvCreatClusterResultFromServer(InputPacket &inpack)
{
    quint8 ansCode = inpack.getAnswerCode();
    if(ansCode == Mi::Success){
        DispersalData dispersalData;
        quint64 clusterID = 0;
        dispersalData.outlet(clusterID, inpack.getPackData());
        this->fetchClusterInfoReq(clusterID);
    }
}

void DevicePage::recvClusterInfoFromServer(InputPacket& inpack)
{
    char* data = inpack.getPackData();
    if(data == NULL) return;

    DClusterInfo* info = new DClusterInfo;
    info->unpack(data);
    clusterInfoList->append(info);
    this->appendClusterToListWidget(clusterListWidget, info);
}

void DevicePage::recvDeviceInfoFromServer(InputPacket &inpack)
{
    char* data = inpack.getPackData();
    if(data == NULL) return;
    MyDeviceInfo* info = new MyDeviceInfo;
    info->unpack(data);
    myDeviceInfoList->append(info);
    this->appendDeviceToListWidget(deviceListWidget, info);
    this->listWidgetLastWidgetStyle(deviceListWidget);
    emit readyRead(CommandCode::FETCH_MY_DEVICE_NETSTATUS_LIST, NULL, 0);//更新设备网络状态
}

void DevicePage::recvClusterJoinApprovalResult(InputPacket& inpack)
{
    quint64 clusterID = 0;
    quint8 result = 0;
    int offset = 0;
    char* data = inpack.getPackData();

    DispersalData dispersalData;
    offset += dispersalData.outlet(result, data + offset);
    dispersalData.outlet(clusterID, data + offset);

    if(result == Mi::Agreed){
        this->fetchClusterInfoReq(clusterID);
    }
}

void DevicePage::recvNotifyInfoFromServer(InputPacket inpack)
{
    //通知标识头
    char* data = inpack.getPackData();
    quint16 dataLen = inpack.getPackDataLen();
    ServerNotifyInfo serverNotifyInfo(data, dataLen);
    quint16 cmdCode = serverNotifyInfo.getCmdCode();
    char* msgdata= serverNotifyInfo.getData();
    quint16 msgLen = serverNotifyInfo.getDataLen();

    inpack.setCmdCode(cmdCode);
    inpack.setPackData(msgdata, msgLen);
    switch (cmdCode)
    {
        case CommandCode::DCLUSTER_APPLY_REQ:
        case CommandCode::DCLUSTER_INVITE_REQ:
        case CommandCode::DCLUSTER_APPLY_APPROVAL:
        case CommandCode::DCLUSTER_INVITE_APPROVAL:
            this->handleClusterNotify(inpack);
            break;
        case CommandCode::DCLUSTER_DELETE_USER:
            this->recvDeleteClusterResult(inpack);
            break;
        case CommandCode::DCLUSTER_DEVICE_DELETE_USER:
        case CommandCode::DCLUSTER_USER_DELETE_DEVICE:
            this->handleDeleteDeviceNotify(inpack);
            break;
        case CommandCode::DCLUSTER_USER_ADD_DEVICE:
        case CommandCode::DCLUSTER_DEVICE_ADD_USER:
            this->handleAddDeviceNotify(inpack);
            break;
        case CommandCode::DCLUSTER_EXIT://用户退出群，删除本地用户信息
            this->handleExitClusterNotify(inpack);
            break;
        case CommandCode::MY_DEVICE_NETSTAUS_NOTIFY:
            this->handleMyDeviceNetStatusNotify();
            break;
        case CommandCode::DCLUSTER_CLAIM_DEVICE:
            this->handleDeviceClaimNotify(inpack);
            break;
        case CommandCode::DCLUSTER_DEV_TRANSFER_REQ:
        case CommandCode::DCLUSTER_DEV_TRANSFER_APPROVAL:
            this->handleClusterTransferDevicesNotify(inpack);
            break;
    }
}

void DevicePage::handleClusterNotify(InputPacket& inpack)
{
    QString strContent;
    quint16 cmdCode = inpack.getCmdCode();

    DClusterJoinNotify info;
    DNotifySummary* newNotify = new DNotifySummary;
    if(cmdCode == CommandCode::DCLUSTER_APPLY_REQ || cmdCode == CommandCode::DCLUSTER_INVITE_REQ){//保存消息到群通知列表
        if(cmdCode == CommandCode::DCLUSTER_APPLY_REQ)
        {
            info.unpack(inpack.getPackData(), Mi::ClusterApplyReq);
            strContent = info.getUserName() + QString(tr("申请加入群")) + info.getClusterName();
        }else{
            info.unpack(inpack.getPackData(), Mi::ClusterInviteReq);
            strContent = info.getUserName() + QString(tr("邀请您加入群")) + info.getClusterName();
        }
        info.setNotifyContent(strContent);
        info.saveToLocal();

        newNotify->setNotifyName(tr("群通知"));
        newNotify->setNotifyID(Mi::ClusterJoinNotifyID);
        newNotify->setNotifyType(Mi::ClusterJoinNotify);
        newNotify->setNotifyImg(ImagePath::DCLUSTER_PORTRAIT);
    }

    if(cmdCode == CommandCode::DCLUSTER_APPLY_APPROVAL || cmdCode == CommandCode::DCLUSTER_INVITE_APPROVAL){//群最新消息保存并显示
        if(cmdCode == CommandCode::DCLUSTER_APPLY_APPROVAL)
            info.unpack(inpack.getPackData(), Mi::ClusterApplyApproval);
        else{
            info.unpack(inpack.getPackData(), Mi::ClusterInviteApproval);
        }

        if(cmdCode != CommandCode::DCLUSTER_INVITE_APPROVAL && info.getMsgResult() == Mi::Agreed){//同意
            strContent = QString(tr("你已经是群成员了，点击了解更多..."));
            newNotify->setNotifyName(info.getClusterName());
            newNotify->setNotifyID(info.getClusterID());
            newNotify->setNotifyType(Mi::DeviceClusterManageNotify);
            newNotify->setNotifyImg(ImagePath::DCLUSTER_PORTRAIT);
            this->recvClusterJoinApprovalResult(inpack);//获取群信息
        }else
            return;
    }

    newNotify->setNotifyContent(strContent);
    newNotify->setNotifyTime(info.getNotifyTime());
    DNotifySummary* existInfo = this->findNotifyInfo(newNotify->getNotifyType(), newNotify->getNotifyID());

    if(existInfo == NULL){//未读消息计数保存
        newNotify->updateUnreadMsgCount(1);
    }else{
        newNotify->updateUnreadMsgCount(existInfo->getUnreadCount() + 1);
    }
    notifySummaryList->append(newNotify);
    this->appendNotifyToListWidget(messageListWidget, newNotify);
}

void DevicePage::handleClusterTransferDevicesNotify(InputPacket& inpack)
{
    QString strContent;
    quint16 cmdCode = inpack.getCmdCode();
    char* packData = inpack.getPackData();

    DeviceTransNotify info;
    DNotifySummary* newNotify = new DNotifySummary;
    if(cmdCode == CommandCode::DCLUSTER_DEV_TRANSFER_REQ){//保存消息到群通知列表
        info.unpack(packData, Mi::DeviceTransferReq);
        strContent = info.getSrcUserName() + QString(tr("申请移交设备到群")) + info.getDestClusterName();
        info.setNotifyContent(strContent);
        info.saveToLocal();

        newNotify->setNotifyName(tr("设备移交"));
        newNotify->setNotifyID(Mi::DeviceTransferNotifyID);
        newNotify->setNotifyType(Mi::DeviceTransferNotify);
        newNotify->setNotifyImg(ImagePath::DCLUSTER_PORTRAIT);
    }else if(cmdCode == CommandCode::DCLUSTER_DEV_TRANSFER_APPROVAL){
        info.unpack(packData, Mi::DeviceTransferApproval);
        return;
    }

    newNotify->setNotifyContent(strContent);
    newNotify->setNotifyTime(info.getNotifyTime());
    DNotifySummary* existInfo = this->findNotifyInfo(newNotify->getNotifyType(), newNotify->getNotifyID());

    if(existInfo == NULL){//未读消息计数保存
        newNotify->updateUnreadMsgCount(1);
    }else{
        newNotify->updateUnreadMsgCount(existInfo->getUnreadCount() + 1);
    }
    notifySummaryList->append(newNotify);
    this->appendNotifyToListWidget(messageListWidget, newNotify);
}

void DevicePage::handleAddDeviceNotify(InputPacket&)
{
    emit readyRead(CommandCode::FETCH_MY_DEVICE_LIST, NULL, 0);
}

void DevicePage::handleDeleteDeviceNotify(InputPacket &inpack)
{
    quint16 ansCode = inpack.getAnswerCode();
    if(ansCode == Mi::Failure) return;

    quint64 clusID = 0;
    quint64 devID = 0;
    int offset = 0;
    quint16 cmdCode = inpack.getCmdCode();
    char* packData = inpack.getPackData();

    if(cmdCode == CommandCode::DCLUSTER_USER_DELETE_DEVICE){
        quint16 devCount = 0;
        DispersalData dispersalData;
        offset += dispersalData.outlet(clusID, packData + offset);
        offset += dispersalData.outlet(devCount, packData + offset);
        
        for(int i = 0; i < devCount; i++){
            offset += dispersalData.outlet(devID, packData + offset);
            this->deleteMyDeviceFromListWidget(devID);
        }
    }
    else if(cmdCode == CommandCode::DCLUSTER_DEVICE_DELETE_USER){
        DispersalData dispersalData;
        offset += dispersalData.outlet(clusID, packData + offset);
        offset += dispersalData.outlet(devID, packData + offset);
        this->deleteMyDeviceFromListWidget(devID);
    }
}

void DevicePage::handleExitClusterNotify(InputPacket &inpack)
{
    int offset = 0;
    quint64 clusID = 0;
    QString clusName;
    quint64 userID = 0;
    QString userName;

    char* data = inpack.getPackData();
    DispersalData dispersalData;
    offset += dispersalData.outlet(clusID, data + offset);
    offset += dispersalData.outlet(clusName, data + offset);
    offset += dispersalData.outlet(userID, data + offset);
    dispersalData.outlet(userName, data + offset);
    DClusterUserInfo::deleteUserInfo(clusID, userID);//删除本地数据
}

void DevicePage::handleDeviceClaimNotify(InputPacket &inpack)
{
    DispersalData dispersal;
    char* data = inpack.getPackData();

    int offset = 0;
    quint8 result = 0;
    quint64 clusterID = 0;
    quint64 deviceID = 0;
    QString deviceName;
    quint64 deviceMac = 0;
    quint32 deviceIP = 0;
    quint16 devicePort = 0;

    offset += dispersal.outlet(result, data + offset);
    offset += dispersal.outlet(clusterID, data + offset);
    offset += dispersal.outlet(deviceID, data + offset);
    offset += dispersal.outlet(deviceName, data + offset);
    offset += dispersal.outlet(deviceMac, data + offset);
    offset += dispersal.outlet(deviceIP, data + offset);
    dispersal.outlet(devicePort, data + offset);

    QString strContent;
    DClusterJoinNotify info;
    DNotifySummary* newNotify = new DNotifySummary;

    strContent = deviceName + QString(tr("设备认领成功"));
    newNotify->setNotifyName(tr("设备通知"));
    newNotify->setNotifyID(Mi::DeviceClaimNotifyID);
    newNotify->setNotifyType(Mi::DeviceClaimNotify);
    newNotify->setNotifyImg(ImagePath::DEVICE_PORTRAIT);
    info.setNotifyContent(strContent);
    info.saveToLocal();

    newNotify->setNotifyContent(strContent);
    newNotify->setNotifyTime(info.getNotifyTime());
    DNotifySummary* existInfo = this->findNotifyInfo(newNotify->getNotifyType(), newNotify->getNotifyID());

    if(existInfo == NULL){//未读消息计数保存
        newNotify->updateUnreadMsgCount(1);
    }else{
        newNotify->updateUnreadMsgCount(existInfo->getUnreadCount() + 1);
    }
    notifySummaryList->append(newNotify);
    this->appendNotifyToListWidget(messageListWidget, newNotify);
}

void DevicePage::handleMyDeviceNetStatusNotify()
{
    //收到在线、离线通知时，重新获取在线列表
    QHash<quint64, MyDeviceInfo*> infoList = myDeviceInfoList->fetchList();
    quint16 devCount = infoList.count();
    AssemblyData assemblyData;
    assemblyData.append(devCount);

    QHashIterator<quint64, MyDeviceInfo*> iter(infoList);
    while(iter.hasNext()){
        iter.next();
        MyDeviceInfo* info = dynamic_cast<MyDeviceInfo*>(iter.value());
        assemblyData.append(info->getDeviceID());
    }

    char* buf = assemblyData.getAssemblyData();
    quint16 len = assemblyData.getAssemblyDataLen();
    emit readyRead(CommandCode::FETCH_MY_DEVICE_NETSTATUS_LIST, buf, len);
}

void DevicePage::deployMsgNotifyList(QList<DNotifySummary *> list)
{
    for(int i = 0; i < list.count(); i++){
        DNotifySummary* info = list.at(i);
        this->appendNotifyToListWidget(messageListWidget, info);
    }
}

void DevicePage::deployClusterInfoList(QHash<quint64, DClusterInfo *> list)
{
    QHashIterator<quint64, DClusterInfo*> iter(list);
    while(iter.hasNext()){
        iter.next();
        DClusterInfo* info = dynamic_cast<DClusterInfo*>(iter.value());
        this->appendClusterToListWidget(clusterListWidget, info);
    }
}

void DevicePage::deployMyDeviceList(QHash<quint64, MyDeviceInfo*> list)
{
    quint16 devCount = list.count();
    AssemblyData assemblyData;
    assemblyData.append(devCount);

    QHashIterator<quint64, MyDeviceInfo*> iter(list);
    while(iter.hasNext()){
        iter.next();
        MyDeviceInfo* info = dynamic_cast<MyDeviceInfo*>(iter.value());
        this->appendDeviceToListWidget(deviceListWidget, info);
        assemblyData.append(info->getDeviceID());
    }

    char* buf = assemblyData.getAssemblyData();
    quint16 len = assemblyData.getAssemblyDataLen();
    emit readyRead(CommandCode::FETCH_MY_DEVICE_NETSTATUS_LIST, buf, len);
}

void DevicePage::appendNotifyToListWidget(ListWidget* listWidget, DNotifySummary *info)
{
    HintMsgButton* btn = NULL;
    QWidget* widget = this->findWidgetFromListWidget(listWidget, info);
    btnMessage->setUnreadMsgCount(notifySummaryList->getTotalUnreadCount());

    if(widget != NULL){//已存在，则更新
        btn = dynamic_cast<HintMsgButton*>(widget);
        this->setNotifyButton(btn, info);
    }else{//不存在，则加入
        btn = new HintMsgButton(this);
        this->setNotifyButton(btn, info);
        this->sortWithNotifyTime(listWidget, btn);

        int height = curFixedContentHeight + listWidget->height();
        this->setBodyRealHeight(height);
    }
    this->listWidgetLastWidgetStyle(messageListWidget);
}

void DevicePage::appendClusterToListWidget(ListWidget* listWidget, DClusterInfo *info)
{
    GroupButton* btn;
    QWidget* widget = this->findWidgetFromListWidget(listWidget, info->getClusterID());

    if(widget != NULL){//已存在，则更新
        btn = dynamic_cast<GroupButton*>(widget);
        this->setClusterButton(btn, info);
    }
    else{//不存在，则加入
        btn = new GroupButton(this);
        this->setClusterButton(btn, info);
        this->sortWithClusterRole(listWidget, btn);

        int height = curFixedContentHeight + listWidget->height() ;
        this->setBodyRealHeight(height);
    }
    this->listWidgetLastWidgetStyle(clusterListWidget);
}

void DevicePage::appendDeviceToListWidget(ListWidget* listWidget, MyDeviceInfo *info)
{
    GroupButton* btn;
    QWidget* widget = this->findWidgetFromListWidget(listWidget, info->getDeviceID());
    if(widget != NULL){//已存在，则更新
        btn = dynamic_cast<GroupButton*>(widget);
        this->setDeviceButton(btn, info);
    }
    else{//不存在，则加入
        btn = new GroupButton(this);
        this->setDeviceButton(btn, info);
        listWidget->addWidget(btn);

        int height = curFixedContentHeight + listWidget->height();
        this->setBodyRealHeight(height);
    }
    this->listWidgetLastWidgetStyle(deviceListWidget);
}

void DevicePage::setDeviceButton(GroupButton* btn, MyDeviceInfo* info)
{
    //设备按钮设置
    if(btn == NULL || info == NULL) return;
    QPixmap pixmap(ImagePath::DEVICE_PORTRAIT);
    pixmap.setDevicePixelRatio(2.0);
    btn->setIndexOne(info->getDeviceID());
    btn->setIndexTwo(info->getClusterID());
    btn->setPixMap(pixmap);
    btn->setDescribe(QString(tr("[离线]")));
    btn->setText(info->getDeviceName());
    btn->setFixedHeight(this->screenHeight()*0.099);
    btn->setFixedWidth(this->screenWidth());
    btn->setContentsMargins(0,0,this->screenWidth()*0.03, 0);
    btn->setAttribute(Qt::WA_LayoutUsesWidgetRect);
    btn->setStyleSheet(SheetStyle::GROUPBUTTON_BOTTOMBORDER);
    this->listWidgetLastWidgetStyle(deviceListWidget);
    connect(btn, SIGNAL(clicked()), this, SLOT(on_btnMyDeviceGroup_clicked()));
}

void DevicePage::setClusterButton(GroupButton* btn, DClusterInfo* info)
{
    //设备群按钮设置
    if(btn == NULL || info == NULL) return;
    QString title = info->getClusterName();
    btn->setText(title);

    QPixmap pixmap(ImagePath::DCLUSTER_PORTRAIT);
    pixmap.setDevicePixelRatio(2.0);
    btn->setPixMap(pixmap);
    btn->setIndexOne(info->getClusterID());

    Mi::AuthLevel role = AuthContent::numToAuthlevel(info->getUserRole());
    QString strRole = AuthContent::GetRole(role);

    btn->setReserve(strRole);
    btn->setFixedHeight(this->screenHeight()*0.099);
    btn->setAttribute(Qt::WA_LayoutUsesWidgetRect);
    btn->setContentsMargins(0,0,this->screenWidth()*0.03, 0);
    btn->setStyleSheet(SheetStyle::GROUPBUTTON_BOTTOMBORDER);
    this->listWidgetLastWidgetStyle(clusterListWidget);
    connect(btn, SIGNAL(clicked()), this, SLOT(on_btnClusterManagementPage_clicked()));
}

void DevicePage::setNotifyButton(HintMsgButton* btn, DNotifySummary* info)
{
    if(btn == NULL || info == NULL) return;
    QPixmap pixmap(ImagePath::DCLUSTER_PORTRAIT);
    btn->setMsgName(info->getNotifyName());
    btn->setMsgPixMap(pixmap);
    btn->setMsgTime(info->getNotifyTime().right(11));
    btn->setMsgDescribe(info->getNotifyContent());
    btn->setFixedHeight(this->screenHeight()*0.11);
    btn->setMsgType(info->getNotifyType());
    btn->setIndexOne(info->getNotifyID());
    btn->setAttribute(Qt::WA_LayoutUsesWidgetRect);
    btn->setContentsMargins(0,0,0,0);
    btn->setUnreadMsgCount(info->getUnreadCount());
    btn->setStyleSheet(SheetStyle::GROUPBUTTON_BOTTOMBORDER);
    this->listWidgetLastWidgetStyle(messageListWidget);
    connect(btn, SIGNAL(deleteClicked()), this, SLOT(on_btnDeleteNotify_clicked()));

    if(info->getNotifyType() == Mi::ClusterJoinNotify){
        connect(btn, SIGNAL(clicked()), this, SLOT(on_btnClusterNotifyPage_clicked()));
    }else if(info->getNotifyType() == Mi::DeviceTransferNotify){
        connect(btn, SIGNAL(clicked()), this, SLOT(on_btnTransferNotifyListPage_clicked()));
    }else if(info->getNotifyType() == Mi::DeviceClusterManageNotify){
        connect(btn, SIGNAL(clicked()), this, SLOT(on_btnClusterNotifyManagementPage_clicked()));
    }
}

void DevicePage::setToolButton(QToolButton *btn, QString imagePath)
{
    QIcon icon = QIcon(imagePath);
    btn->setIcon(icon);
    btn->setIconSize(QSize(btn->height()*0.9, btn->height()*0.9));
    btn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    btn->setStyleSheet(SheetStyle::TOOLBUTTON_GREY_15);
}

void DevicePage::setListWidget(ListWidget *listWidget)
{
    listWidget->hide();
    listWidget->setStyleSheet(SheetStyle::LISTWIDGET);
    listWidget->setContentsMargins(this->screenWidth()*0.03,0,0,0);
}

void DevicePage::deleteClusterListWidgetItem(quint64 clusID)
{
    int clusCount = clusterListWidget->count();
    for(int i = 0; i < clusCount; i++)
    {
        QWidget* widget = clusterListWidget->itemAt(i);
        GroupButton* clusBtn = dynamic_cast<GroupButton*>(widget);

        if(clusBtn->getIndexOne() == clusID){
            clusterListWidget->takeAt(i);//delete clusBtn
            clusterInfoList->remove(clusID);//删除群信息

            DClusterUserInfoList::deleteUsers(clusID);//删除群成员信息
            DClusterDevInfoList::deleteDevices(clusID);//删除群设备信息
            myDeviceInfoList->deleteDevices(clusID);//删除与群相关的我的设备信息

            int height = curFixedContentHeight + clusterListWidget->height();
            this->setBodyRealHeight(height);
            return;
        }
    }
}

void DevicePage::deleteDeviceListWidgetItem(quint64 clusID)
{
    int i = 0;
    int devCount = deviceListWidget->count();

    for(i = 0; i < devCount; i++){
        QWidget* widget = deviceListWidget->itemAt(i);
        GroupButton* devBtn = dynamic_cast<GroupButton*>(widget);

        quint64 devID = devBtn->getIndexOne();
        quint64 clusterID = devBtn->getIndexTwo();
        if(clusterID == clusID){
            deviceListWidget->takeAt(devBtn);
            myDeviceInfoList->remove(devID);

            i = -1;//初始化动态变化的列表
            devCount = deviceListWidget->count();

            int height = curFixedContentHeight + deviceListWidget->height();
            this->setBodyRealHeight(height);
        }
    }
}

QWidget* DevicePage::findWidgetFromListWidget(ListWidget *listWidget, quint64 id)
{
    int count = listWidget->count();
    if(count == 0) return NULL;

    for(int i = 0; i < count; i++){
        QWidget* widget = listWidget->itemAt(i);
        GPushButton* gBtn = dynamic_cast<GPushButton*>(widget);

        if(gBtn->getIndexOne() == id)
            return widget;
    }

    return NULL;
}

QWidget* DevicePage::findWidgetFromListWidget(ListWidget *listWidget, DNotifySummary *info)
{
    int count = listWidget->count();
    for(int i = 0; i < count; i++){
        QWidget* widget = listWidget->itemAt(i);
        HintMsgButton* hBtn = dynamic_cast<HintMsgButton*>(widget);

        if(hBtn->getMsgType() == info->getNotifyType() && hBtn->getIndexOne() == info->getNotifyID())
            return widget;
    }

    return NULL;
}

DClusterInfo* DevicePage::findClusterInfo(quint64 clusID)
{
    QHash<quint64, DClusterInfo*> infoList = clusterInfoList->fetchList();
    DClusterInfo* info = infoList.value(clusID);
    return info;
}

DNotifySummary* DevicePage::findNotifyInfo(Mi::NotifyCategory type, quint64 id)
{
    QList<DNotifySummary*> infoList = notifySummaryList->fetchList();
    QListIterator<DNotifySummary*> iter(infoList);
    while (iter.hasNext()) {
        DNotifySummary* info = iter.next();
        if(info->getNotifyType() == type && info->getNotifyID() == id){
            return info;
        }
    }

    return NULL;
}

DNotifySummary* DevicePage::findNotifyFromNotifyList(Mi::NotifyCategory type, quint64 notifyID)
{
    QList<DNotifySummary*> infoList = notifySummaryList->fetchList();
    for(int i = 0; i < infoList.count(); i++){
        DNotifySummary* info = infoList.at(i);
        if(info->getNotifyType() == type && info->getNotifyID() == notifyID)
            return info;
    }
    return NULL;
}

void DevicePage::sortDeviceNetStatus(ListWidget* listWidget, quint64 id, Mi::NetStatus status)
{
    QString strDisplay = NetLine::GetNetStatus(status);
    int count = listWidget->count();
    for(int i = 0; i < count; i++)
    {
        QWidget* widget = listWidget->itemAt(i);
        GroupButton* imBtn = dynamic_cast<GroupButton*>(widget);

        if(imBtn->getIndexOne() == id){
            imBtn->setDescribe(strDisplay);
            if(status == Mi::OnLine){
                QWidget* widgt = listWidget->removeWidget(i);
                listWidget->insertWidget(0, widgt);
            }else if(status == Mi::OffLine){
                QWidget* widgt = listWidget->removeWidget(i);
                int count = listWidget->count();
                listWidget->insertWidget(count, widgt);
            }
            this->listWidgetLastWidgetStyle(deviceListWidget);
            return;
        }
    }
}

void DevicePage::sortWithClusterRole(ListWidget* listWidget, GroupButton* gBtn)
{
    int count = listWidget->count();
    quint64 newClusID = gBtn->getIndexOne();
    DClusterInfo* newInfo = this->findClusterInfo(newClusID);
    for(int i = 0; i < count; i++)
    {
        QWidget* widget = listWidget->itemAt(i);
        GroupButton* imBtn = dynamic_cast<GroupButton*>(widget);
        quint64 oldClusID = imBtn->getIndexOne();
        DClusterInfo* oldInfo = this->findClusterInfo(oldClusID);

        if(oldInfo == NULL || newInfo == NULL) continue;
        if(newInfo->getUserRole() <= oldInfo->getUserRole()){//角色小于等于时，插入当前位置
            listWidget->insertWidget(i, gBtn);
            this->listWidgetLastWidgetStyle(clusterListWidget);
            return;
        }
    }
    this->listWidgetLastWidgetStyle(clusterListWidget);
    listWidget->insertWidget(count, gBtn);//没有找到，直接插入
}

void DevicePage::sortWithNotifyTime(ListWidget *listWidget, HintMsgButton *hBtn)
{
    int count = listWidget->count();
    for(int i = 0; i < count; i++){
        QWidget* wdg = listWidget->itemAt(i);
        HintMsgButton* imBtn = dynamic_cast<HintMsgButton*>(wdg);

        if(hBtn->getMsgTime() >= imBtn->getMsgTime()){
            listWidget->insertWidget(i, hBtn);
            this->listWidgetLastWidgetStyle(messageListWidget);
            return;
        }
    }
    this->listWidgetLastWidgetStyle(clusterListWidget);
    listWidget->insertWidget(0, hBtn);//没有找到，直接插入
}

void DevicePage::fetchDeviceInfoFromServer(quint64 devID)
{
    AssemblyData assemblyData;
    assemblyData.append(devID);
    char* data = assemblyData.getAssemblyData();
    quint16 dataLen = assemblyData.getAssemblyDataLen();
    emit readyRead(CommandCode::FETCH_DEVICE_INFO, data, dataLen);
}

void DevicePage::fetchClusterInfoReq(quint64 clusID)
{
    AssemblyData assemblyData;
    assemblyData.append(clusID);
    char* data = assemblyData.getAssemblyData();
    quint16 dataLen = assemblyData.getAssemblyDataLen();
    emit readyRead(CommandCode::FETCH_DCLUSTER_INFO, data, dataLen);
}

void DevicePage::createClusterManagementPage(quint64 clusID)
{
    SAFE_DELETE(clusterManagementPage);
    clusterManagementPage = new DClusterManagementPage(this, clusID);
    this->displayConcretePage(clusterManagementPage);
    emit readyRead(CommandCode::BOTTOM_BAR_UNDISPLAY, NULL, 0);//隐藏底部任务栏
}

void DevicePage::deleteMyDeviceFromListWidget(quint64 devID)
{
    //从我的设备列表删除设备
    int count = deviceListWidget->count();
    for(int i = 0; i < count; i++)
    {
        QWidget* widget = deviceListWidget->itemAt(i);
        GroupButton* devBtn = dynamic_cast<GroupButton*>(widget);
        if(devBtn->getIndexOne() == devID){
            deviceListWidget->takeAt(i);//delete device button
            myDeviceInfoList->remove(devID);

            int height = curFixedContentHeight +  deviceListWidget->height();
            this->setBodyRealHeight(height);
            return;
        }
    }
}

void DevicePage::listWidgetLastWidgetStyle(ListWidget *listWidget)
{
    for(int i = 0; i < listWidget->count(); i++){
        QWidget* widget = listWidget->itemAt(i);
        widget->setStyleSheet(SheetStyle::GROUPBUTTON_BOTTOMBORDER);
        if(i == (listWidget->count() - 1)){
            widget->setStyleSheet(SheetStyle::GROUPBUTTON_NOBORDER);
        }
    }
}

void DevicePage::displayConcretePage(BasePage *page)
{
    if(page == NULL) return;
    page->show();
    connect(page, SIGNAL(destroyed()), this, SLOT(on_deleteSubPages()));
    connect(page, SIGNAL(readyRead(quint16,char*,quint16)), this, SLOT(on_recvDataFromOtherPage(quint16,char*,quint16)));
}

void DevicePage::adjustListWidgets(ListWidget *listwidget)
{
    curListWidget->hide();
    widgetsLayout->removeWidget(curListWidget);
    widgetsLayout->addWidget(listwidget);
    listwidget->show();//列表切换
    curListWidget = listwidget;

    int height = curFixedContentHeight + listwidget->height();
    this->setBodyRealHeight(height);
}

void DevicePage::adjustUnreadNotifyMessageCount(HintMsgButton *btn)
{
    DNotifySummary* info = this->findNotifyInfo(btn->getMsgType(), btn->getIndexOne());
    if(info != NULL){
        curHintMsgButton = btn;
        info->updateUnreadMsgCount(0);
        btn->setUnreadMsgCount(0);//按通知类型分类的未读消息归零

        int unreadCount = notifySummaryList->getTotalUnreadCount();
        btnMessage->setUnreadMsgCount(unreadCount);//未读消息总数设置
    }
}

void DevicePage::labelHintInfoDisplay()
{
    if(curListWidget == messageListWidget){
        if(messageListWidget->count() <= 0){
            lblInfoHint->setText(QString(tr("暂时没有新消息")));
        }else{
            lblInfoHint->setText(QString(tr("消息通知")));
        }

        btnMessage->setIcon(QPixmap(ImagePath::DEV_MESSAGE_SELECTED));
        btnMessage->setStyleSheet(SheetStyle::TOOLBUTTON_BLUE_15);
        btnDevice->setIcon(QPixmap(ImagePath::MYDEVICE_CANCELED));
        btnDevice->setStyleSheet(SheetStyle::TOOLBUTTON_GREY_15);
        btnCluster->setIcon(QPixmap(ImagePath::DEV_CLUSTER_CANCELED));
        btnCluster->setStyleSheet(SheetStyle::TOOLBUTTON_GREY_15);

    }else if(curListWidget == deviceListWidget){
        if(deviceListWidget->count() <= 0){
            lblInfoHint->setText(QString(tr("你还没有已授权设备")));
        }else{
            lblInfoHint->setText(QString(tr("我的设备")));
        }

        btnDevice->setIcon(QPixmap(ImagePath::MYDEVICE_SELECTED));
        btnDevice->setStyleSheet(SheetStyle::TOOLBUTTON_BLUE_15);
        btnMessage->setIcon(QPixmap(ImagePath::DEV_MESSAGE_CANCELED));
        btnMessage->setStyleSheet(SheetStyle::TOOLBUTTON_GREY_15);
        btnCluster->setIcon(QPixmap(ImagePath::DEV_CLUSTER_CANCELED));
        btnCluster->setStyleSheet(SheetStyle::TOOLBUTTON_GREY_15);

    }else if(curListWidget == clusterListWidget){
        if(clusterListWidget->count() <= 0){
            lblInfoHint->setText(QString(tr("你还没有加入或创建群组")));
        }else{
            lblInfoHint->setText(QString(tr("群组信息")));
        }

        btnCluster->setIcon(QPixmap(ImagePath::DEV_CLUSTER_SELECTED));
        btnCluster->setStyleSheet(SheetStyle::TOOLBUTTON_BLUE_15);
        btnDevice->setIcon(QPixmap(ImagePath::MYDEVICE_CANCELED));
        btnDevice->setStyleSheet(SheetStyle::TOOLBUTTON_GREY_15);
        btnMessage->setIcon(QPixmap(ImagePath::DEV_MESSAGE_CANCELED));
        btnMessage->setStyleSheet(SheetStyle::TOOLBUTTON_GREY_15);
    }
}

void DevicePage::transferDataToAnotherPage(InputPacket &inpack, BasePage *basePage)
{
    if(basePage == NULL)  return;
    basePage->fetchData(inpack);
}

void DevicePage::on_toolBtnMessage_clicked()
{
    this->adjustListWidgets(messageListWidget);
    this->labelHintInfoDisplay();
}

void DevicePage::on_toolBtnDevice_clicked()
{
    this->adjustListWidgets(deviceListWidget);
    this->labelHintInfoDisplay();
}

void DevicePage::on_toolBtnCluster_clicked()
{
    this->adjustListWidgets(clusterListWidget);
    this->labelHintInfoDisplay();
}

void DevicePage::on_btnAddDevices_clicked()
{
    SAFE_DELETE(deviceManagementPage);
    deviceManagementPage = new DeviceManagementPage(this);
    this->displayConcretePage(deviceManagementPage);
    emit readyRead(CommandCode::BOTTOM_BAR_UNDISPLAY, NULL, 0);//隐藏底部任务栏
}

void DevicePage::on_btnClusterManagementPage_clicked()
{
    GPushButton* btnThis = dynamic_cast<GPushButton*>(sender());
    quint64 clusterID = btnThis->getIndexOne();
    this->createClusterManagementPage(clusterID);
}

void DevicePage::on_btnClusterNotifyManagementPage_clicked()
{
    HintMsgButton* thisBtn = dynamic_cast<HintMsgButton*>(sender());
    this->adjustUnreadNotifyMessageCount(thisBtn);

    quint64 clusterID = thisBtn->getIndexOne();
    this->createClusterManagementPage(clusterID);
}

void DevicePage::on_btnClusterNotifyPage_clicked()
{
    HintMsgButton* thisBtn = dynamic_cast<HintMsgButton*>(sender());
    this->adjustUnreadNotifyMessageCount(thisBtn);

    SAFE_DELETE(clusterNotifyListPage);
    clusterNotifyListPage = new DClusterNotifyListPage(this);
    clusterNotifyListPage->setObjectName(QString("DClusterNotifyListPage"));
    this->displayConcretePage(clusterNotifyListPage);
    emit readyRead(CommandCode::BOTTOM_BAR_UNDISPLAY, NULL, 0);//隐藏底部任务栏
}


void DevicePage::on_btnMyDeviceGroup_clicked()
{
    ////////////////////////////////////////////////////////////////
}

void DevicePage::on_btnTransferNotifyListPage_clicked()
{
    HintMsgButton* thisBtn = dynamic_cast<HintMsgButton*>(sender());
    this->adjustUnreadNotifyMessageCount(thisBtn);

    SAFE_DELETE(transferDeviceNotifyListPage);
    transferDeviceNotifyListPage = new DClusterTransferNotifyListPage(this);
    this->displayConcretePage(transferDeviceNotifyListPage);
    emit readyRead(CommandCode::BOTTOM_BAR_UNDISPLAY, NULL, 0);//隐藏底部任务栏
}

void DevicePage::on_btnDeleteNotify_clicked()
{
    HintMsgButton* thisBtn = dynamic_cast<HintMsgButton*>(sender());
    this->adjustUnreadNotifyMessageCount(thisBtn);

    Mi::NotifyCategory type = thisBtn->getMsgType();
    quint64 notifyID = thisBtn->getIndexOne();
    notifySummaryList->remove(type, notifyID);

    messageListWidget->takeAt(thisBtn);
    int height = curFixedContentHeight + messageListWidget->height();
    this->setBodyRealHeight(height);
    this->labelHintInfoDisplay();
    this->update();
}

void DevicePage::on_recvDataFromOtherPage(quint16 cmdCode, char *data, quint16 dataLen)
{
    emit readyRead(cmdCode, data, dataLen);
}

void DevicePage::on_deleteSubPages()
{
    QString name = this->sender()->objectName();
    if(name == QString("DClusterNotifyListPage"))
    {
        DNotifySummary* info = this->findNotifyInfo(curHintMsgButton->getMsgType(), curHintMsgButton->getIndexOne());
        if(info != NULL){
            info->updateUnreadMsgCount(0);
            curHintMsgButton->setUnreadMsgCount(0);//按通知类型分类的未读消息归零

            int unreadCount = notifySummaryList->getTotalUnreadCount();
            btnMessage->setUnreadMsgCount(unreadCount);//未读消息总数设置
        }
    }

    clusterManagementPage = NULL;
    deviceManagementPage = NULL;
    clusterNotifyListPage = NULL;
    transferDeviceNotifyListPage = NULL;
    emit readyRead(CommandCode::BOTTOM_BAR_DISPLAY, NULL, 0);//显示底部任务栏
}

DevicePage::~DevicePage()
{
    SAFE_DELETE(myDeviceOnlineList);
    SAFE_DELETE(clusterInfoList);
    SAFE_DELETE(myDeviceOnlineList);
    SAFE_DELETE(notifySummaryList);
}

