#include <QGridLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QScrollArea>
#include <QByteArray>
#include <QDateTime>
#include <cstring>
#include <QScrollBar>
#include <QTextCodec>
#include <QLabel>
#include <QList>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QToolButton>

#include "HintToolButton.h"
#include "InputPacket.h"
#include "ListWidget.h"
#include "UserInfo.h"
#include "SearchBar.h"
#include "UserInfoList.h"
#include "UClusterInfoList.h"
#include "HintMsgButton.h"
#include "UNotifySummaryList.h"
#include "UserManagementPage.h"
#include "UserDetailInfoPage.h"
#include "UserJoinNotifyListPage.h"
#include "UClusterNotifyListPage.h"
#include "GroupButton.h"
#include "ContactPage.h"
#include "GlobalDefine.hpp"
#include "UserProtocol.hpp"
#include "ServerNotifyInfo.h"
#include "UClusterJoinNotify.h"
#include "UserJoinNotify.h"
#include "ImageConfig.hpp"
#include "DispersalData.h"
#include "NavigationBar.h"
#include "AssemblyData.h"
#include "SheetStyleConfig.hpp"

ContactPage::ContactPage(QWidget *parent, PageType type)
    :BasePage(parent, type)
    , curFixedContentHeight(0)
    , curListWidget(NULL)
    , curToolButton(NULL)
    , userManagementPage(NULL)
    , userJoinNotifyListPage(NULL)
    , clusterNotifyListPage(NULL)
    , userInfoList(NULL)
    , clusterInfoList(NULL)
    , notifySummaryList(NULL)
{
    this->initializeWidget();
}

void ContactPage::initializeWidget()
{
    //导航栏
    QString strTitle = tr("联系人");
    QPixmap pixRightBtn = QPixmap(ImagePath::ADDTION);
    navigationBar = new NavigationBar(this);
    navigationBar->setTitleText(strTitle);
    navigationBar->setRightPixMap(pixRightBtn);
    connect(navigationBar, SIGNAL(rightClicked()), this, SLOT(on_btnAddContactsPage_clicked()));

    QHBoxLayout* pHLTop = new QHBoxLayout();
    pHLTop->addWidget(navigationBar);
    pHLTop->setSpacing(0);
    pHLTop->setMargin(0);
    this->setTopbarLayout(pHLTop);

    //信息分类
    btnMessage = new HintToolButton(this);
    btnMessage->setText(QString(tr("消息")));
    this->setToolButton(btnMessage, ImagePath::USER_MESSAGE_SELECTED);
    btnMessage->setStyleSheet(SheetStyle::TOOLBUTTON_BLUE_15);
    connect(btnMessage, SIGNAL(clicked()), this, SLOT(on_toolBtnMessage_clicked()));

    btnCluster = new QToolButton(this);
    btnCluster->setText(QString(tr("群组")));
    this->setToolButton(btnCluster, ImagePath::USER_CLUSTER_CANCELED);
    connect(btnCluster, SIGNAL(clicked()), this, SLOT(on_toolBtnCluster_clicked()));

    btnBuddy = new QToolButton(this);
    btnBuddy->setText(QString(tr("好友")));
    this->setToolButton(btnBuddy, ImagePath::BUDDY_CANCELED);
    connect(btnBuddy, SIGNAL(clicked()), this, SLOT(on_toolBtnBuddy_clicked()));

    //信息详细内容
    buddyListWidget = new ListWidget(this);
    buddyListWidget->hide();
    this->setListWidget(buddyListWidget);

    messageListWidget = new ListWidget(this);
    messageListWidget->hide();
    this->setListWidget(messageListWidget);

    clusterListWidget = new ListWidget(this);
    clusterListWidget->hide();
    this->setListWidget(clusterListWidget);

    curListWidget = messageListWidget;
    curListWidget->show();

    QHBoxLayout* msgBtnLayout = new QHBoxLayout();
    msgBtnLayout->addWidget(btnMessage);
    msgBtnLayout->addWidget(btnBuddy);
    msgBtnLayout->addWidget(btnCluster);
    msgBtnLayout->setContentsMargins(this->screenWidth()*0.05, 0 , this->screenWidth()*0.05, 0);

    QFrame* selectFrame = new QFrame(this);
    selectFrame->setLayout(msgBtnLayout);
    selectFrame->setStyleSheet("border:none;background-color:white;");
    selectFrame->setFixedHeight(this->screenHeight()*0.11);
    selectFrame->setContentsMargins(0,0,0,0);
    curFixedContentHeight += selectFrame->height();

    //好友信息
    lblInfoHint = new QLabel(tr("消息通知"), this);
    lblInfoHint->setFixedHeight(this->screenHeight()*0.05);
    lblInfoHint->setFixedWidth(this->screenWidth());
    lblInfoHint->setContentsMargins(this->screenWidth()*0.03,0,0,0);
    lblInfoHint->setStyleSheet(SheetStyle::BUDDY_GROUPING_LABEL);
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

void ContactPage::timerEvent(QTimerEvent*)
{
    emit readyRead(CommandCode::FETCH_USER_INFO_LIST, NULL, 0);
    emit readyRead(CommandCode::FETCH_USER_CLUSTER_LIST, NULL, 0);
    this->killTimer(timerID);
}

void ContactPage::loadLocalData()
{
    //消息通知信息列表
    notifySummaryList = new UNotifySummaryList;
    notifySummaryList->loadDataSource();
    this->deployMsgNotifyList(notifySummaryList->fetchList());

    //初始化设备群信息列表
    clusterInfoList = new UClusterInfoList;
    clusterInfoList->loadDataSource();
    this->deployClusterInfoList(clusterInfoList->fetchList());

    //初始化我的好友列表
    userInfoList = new UserInfoList;
    userInfoList->loadDataSource();
    this->deployMyBuddyList(userInfoList->fetchList());
    this->labelHintInfoDisplay();
}

void ContactPage::fetchData(InputPacket& inpack)
{
    quint16 cmdCode = inpack.getCmdCode();
    DPRINT("-->ContactsPage receive data, the cmdCode:0x%x", inpack.getCmdCode());

    switch(cmdCode)
    {
        case CommandCode::FETCH_USER_INFO_LIST:
            this->recvUserInfoListFromServer(inpack);
            break;
        case CommandCode::FETCH_USER_INFO:
            this->recvUserInfoFromServer(inpack);
            break;
        case CommandCode::USER_JOIN_APPROVAL:
            this->recvUserJoinApprovalResult(inpack);
            break;
        case CommandCode::UCLUSTER_CREATE:
            this->recvCreatClusterResultFromServer(inpack);
            break;
        case CommandCode::FETCH_USER_CLUSTER_LIST:
            this->recvClusterListFromServer(inpack);
            break;
        case CommandCode::FETCH_USER_CLUSTER_INFO:
            this->recvClusterInfoFromServer(inpack);
            break;
        case CommandCode::UCLUSTER_INVITE_APPROVAL:
            this->recvClusterJoinApprovalResult(inpack);
            break;
        case CommandCode::SERVER_NOTIFY:
            this->recvNotifyInfoFromServer(inpack);
            break;
        default:
            break;
    }
    this->labelHintInfoDisplay();
    this->transferDataToAnotherPage(inpack, userJoinNotifyListPage);
    this->transferDataToAnotherPage(inpack, userManagementPage);
    this->transferDataToAnotherPage(inpack, clusterNotifyListPage);
}

void ContactPage::recvUserInfoListFromServer(InputPacket& inpack)
{
    char* data = inpack.getPackData();
    quint16 dataLen = inpack.getPackDataLen();

    SAFE_DELETE(userInfoList);
    userInfoList = new UserInfoList;
    userInfoList->appendList(data, dataLen);
    this->deployMyBuddyList(userInfoList->fetchList());
}

void ContactPage::recvUserInfoFromServer(InputPacket &inpack)
{
    char* data = inpack.getPackData();
    UserInfo* userInfo = new UserInfo;
    userInfo->unpack(data);
    userInfoList->append(userInfo);
    this->appendUserToListWidget(buddyListWidget, userInfo);
}

void ContactPage::recvClusterInfoFromServer(InputPacket &inpack)
{
    char* data = inpack.getPackData();
    UClusterInfo* clusInfo = new UClusterInfo;
    clusInfo->unpack(data);
    clusterInfoList->append(clusInfo);
    this->appendClusterToListWidget(clusterListWidget, clusInfo);
}

void ContactPage::recvUserJoinApprovalResult(InputPacket &inpack)
{
    quint64 userID = 0;
    quint8 result = 0;
    int offset = 0;
    char* data = inpack.getPackData();

    DispersalData dispersalData;
    offset += dispersalData.outlet(result, data + offset);
    dispersalData.outlet(userID, data + offset);

    if(result == Mi::Agreed){
        this->fetchUserInfoReq(userID);
    }
}

void ContactPage::recvClusterListFromServer(InputPacket &inpack)
{
    char* data = inpack.getPackData();
    quint16 dataLen = inpack.getPackDataLen();

    SAFE_DELETE(clusterInfoList);
    clusterInfoList = new UClusterInfoList;
    clusterInfoList->appendList(data, dataLen);
    this->deployClusterInfoList(clusterInfoList->fetchList());
}

void ContactPage::recvClusterJoinApprovalResult(InputPacket &inpack)
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

void ContactPage::recvCreatClusterResultFromServer(InputPacket &inpack)
{
    quint8 ansCode = inpack.getAnswerCode();
    if(ansCode == Mi::Success){
        DispersalData dispersalData;
        quint64 clusterID = 0;
        dispersalData.outlet(clusterID, inpack.getPackData());
        this->fetchClusterInfoReq(clusterID);
    }
}

void ContactPage::recvNotifyInfoFromServer(InputPacket inpack)
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
        case CommandCode::USER_JOIN_REQ:
        case CommandCode::USER_JOIN_APPROVAL:
            this->handleUserJoinNotify(inpack);
            break;
        case CommandCode::UCLUSTER_APPLY_REQ:
        case CommandCode::UCLUSTER_APPLY_APPROVAL:
        case CommandCode::UCLUSTER_INVITE_REQ:
        case CommandCode::UCLUSTER_INVITE_APPROVAL:
            this->handleClusterNotify(inpack);
            break;
    }
}

void ContactPage::handleUserJoinNotify(InputPacket& inpack)
{
    QString strContent;
    quint16 cmdCode = inpack.getCmdCode();
    UserJoinNotify info;
    UNotifySummary* newNotify = new UNotifySummary;

    if(cmdCode == CommandCode::USER_JOIN_REQ){
        info.unpack(inpack.getPackData(), Mi::UserJoinReq);
        strContent = info.getUserName() + QString(tr("申请加为好友"));
        newNotify->setNotifyName(QString(tr("好友申请")));
        newNotify->setNotifyID(Mi::UserJoinNotifyID);
        newNotify->setNotifyType(Mi::UserJoinNotify);

        info.setNotifyContent(strContent);
        info.saveToLocal();
    }else if(cmdCode == CommandCode::USER_JOIN_APPROVAL){
        info.unpack(inpack.getPackData(), Mi::UserJoinApproval);
        if(info.getMsgResult() == Mi::Agreed) {//同意加入
            strContent = QString(tr("你们已经是好友了，点击聊天..."));
            newNotify->setNotifyName(info.getUserName());
            newNotify->setNotifyID(info.getUserID());
            newNotify->setNotifyType(Mi::UserChatNotify);
            this->recvUserJoinApprovalResult(inpack);//获取好友信息
        }else
            return;
    }

    newNotify->setNotifyContent(strContent);
    newNotify->setNotifyTime(info.getNotifyTime());
    newNotify->setNotifyImg(ImagePath::USER_PORTRAIT);
    UNotifySummary* existInfo = this->findNotifyInfo(newNotify->getNotifyType(), newNotify->getNotifyID());

    if(existInfo == NULL){//未读消息计数保存
        newNotify->updateUnreadMsgCount(1);
    }
    else{
        newNotify->updateUnreadMsgCount(existInfo->getUnreadCount() + 1);
        existInfo->updateUnreadMsgCount(existInfo->getUnreadCount() + 1);
    }
    notifySummaryList->append(newNotify);
    this->appendNotifyToListWidget(messageListWidget, newNotify);
}

void ContactPage::handleClusterNotify(InputPacket &inpack)
{
    QString strContent;
    quint16 cmdCode = inpack.getCmdCode();
    UClusterJoinNotify info;
    UNotifySummary* newNotify = new UNotifySummary;

    if(cmdCode == CommandCode::UCLUSTER_APPLY_REQ || cmdCode == CommandCode::UCLUSTER_INVITE_REQ){//保存消息到群通知列表
        if(cmdCode == CommandCode::UCLUSTER_APPLY_REQ)
        {
            info.unpack(inpack.getPackData(), Mi::ClusterApplyReq);
            strContent = info.getUserName() + QString(tr("申请加入群")) + info.getClusterName();
        }else{
            info.unpack(inpack.getPackData(), Mi::ClusterInviteReq);
            strContent = info.getUserName() + QString(tr("邀请您加入群")) + info.getClusterName();
        }
        newNotify->setNotifyName(tr("群通知"));
        newNotify->setNotifyID(Mi::ClusterJoinNotifyID);
        newNotify->setNotifyType(Mi::ClusterJoinNotify);
        newNotify->setNotifyImg(ImagePath::UCLUSTER_PORTRAIT);
        info.setNotifyContent(strContent);
        info.saveToLocal();
    }else if(cmdCode == CommandCode::UCLUSTER_APPLY_APPROVAL || cmdCode == CommandCode::UCLUSTER_INVITE_APPROVAL){//群最新消息保存并显示
        if(cmdCode == CommandCode::UCLUSTER_APPLY_APPROVAL)
            info.unpack(inpack.getPackData(), Mi::ClusterApplyApproval);
        else{
            info.unpack(inpack.getPackData(), Mi::ClusterInviteApproval);
        }

        if(info.getMsgResult() == Mi::Agreed){//同意
            strContent = QString(tr("你已经是群成员了，和大家打个招呼吧..."));
            newNotify->setNotifyName(info.getClusterName());
            newNotify->setNotifyID(info.getClusterID());
            newNotify->setNotifyType(Mi::UserClusterChatNotify);
            newNotify->setNotifyImg(ImagePath::UCLUSTER_PORTRAIT);
            this->recvClusterJoinApprovalResult(inpack);//获取群信息
        }else
            return;
    }

    newNotify->setNotifyContent(strContent);
    newNotify->setNotifyTime(info.getNotifyTime());
    UNotifySummary* existInfo = this->findNotifyInfo(newNotify->getNotifyType(), newNotify->getNotifyID());

    if(existInfo == NULL){//未读消息计数保存
        newNotify->updateUnreadMsgCount(1);
    }else{
        newNotify->updateUnreadMsgCount(existInfo->getUnreadCount() + 1);
    }
    notifySummaryList->append(newNotify);
    this->appendNotifyToListWidget(messageListWidget, newNotify);
}

void ContactPage::deployMsgNotifyList(QList<UNotifySummary *> list)
{
    for(int i = 0; i < list.count(); i++){
        UNotifySummary* info = list.at(i);
        this->appendNotifyToListWidget(messageListWidget, info);
    }
}

void ContactPage::deployClusterInfoList(QHash<quint64, UClusterInfo *> list)
{
    QHashIterator<quint64, UClusterInfo*> iter(list);
    while(iter.hasNext()){
        iter.next();
        UClusterInfo* info = dynamic_cast<UClusterInfo*>(iter.value());
        this->appendClusterToListWidget(clusterListWidget, info);
    }
}

void ContactPage::deployMyBuddyList(QHash<quint64, UserInfo*> list)
{
    QHashIterator<quint64, UserInfo*> iter(list);
    while(iter.hasNext()){
        iter.next();
        UserInfo* info = dynamic_cast<UserInfo*>(iter.value());
        this->appendUserToListWidget(buddyListWidget, info);
    }
}

void ContactPage::appendUserToListWidget(ListWidget* listwidget, UserInfo *info)
{
    GroupButton* btn;
    quint64 userID = info->getUserID();
    QWidget* widget = this->findWidgetFromListWidget(listwidget, userID);

    if(widget != NULL){//已存在，则更新
        btn = dynamic_cast<GroupButton*>(widget);
        this->setUserButton(btn, info);
        this->listWidgetLastWidgetStyle(buddyListWidget);
    }
    else{//不存在，则加入
        btn = new GroupButton(this);
        this->setUserButton(btn, info);
        buddyListWidget->addWidget(btn);
        this->listWidgetLastWidgetStyle(buddyListWidget);

        quint32 height = curFixedContentHeight + buddyListWidget->height();
        this->setBodyRealHeight(height);
    }
}

void ContactPage::appendClusterToListWidget(ListWidget* listwidget, UClusterInfo *info)
{
    GroupButton* btn = NULL;
    QWidget* widget = this->findWidgetFromListWidget(listwidget, info->getClusterID());

    if(widget != NULL){//已存在，则更新
        btn = dynamic_cast<GroupButton*>(widget);
        this->setClusterButton(btn, info);
        this->listWidgetLastWidgetStyle(clusterListWidget);
    }
    else{//不存在，则加入
        btn = new GroupButton(this);
        this->setClusterButton(btn, info);
        this->sortWithClusterRole(clusterListWidget, btn);

        quint32 height = curFixedContentHeight + clusterListWidget->height() ;
        this->setBodyRealHeight(height);
    }
}

void ContactPage::appendNotifyToListWidget(ListWidget* listwidget, UNotifySummary *info)
{
    HintMsgButton* btn = NULL;
    QWidget* widget = this->findNotifyButtonFromListWidget(listwidget, info);
    btnMessage->setUnreadMsgCount(notifySummaryList->getTotalUnreadCount());

    if(widget != NULL){//已存在，则更新
        btn = dynamic_cast<HintMsgButton*>(widget);
        this->setNotifyButton(btn, info);
    }else{//不存在，则加入
        btn = new HintMsgButton(this);
        this->setNotifyButton(btn, info);
        this->sortWithNotifyTime(listwidget, btn);

        int height = curFixedContentHeight + listwidget->height();
        this->setBodyRealHeight(height);
    }
}

void ContactPage::setUserButton(GroupButton *btn, UserInfo *info)
{
    //设备按钮设置
    if(btn == NULL || info == NULL) return;
    QPixmap pixmap(ImagePath::USER_PORTRAIT);
    pixmap.setDevicePixelRatio(2.0);
    btn->setIndexOne(info->getUserID());
    btn->setPixMap(pixmap);
    btn->setText(info->getUserName());
    btn->setFixedHeight(this->screenHeight()*0.1);
    btn->setFixedWidth(this->screenWidth());
    btn->setAttribute(Qt::WA_LayoutUsesWidgetRect);
    btn->setContentsMargins(0,0,this->screenWidth()*0.05,0);
    btn->setStyleSheet(SheetStyle::GROUPBUTTON_BOTTOMBORDER);
    //connect(btn, SIGNAL(clicked()), this, SLOT(on_btnUser_clicked()));
}

void ContactPage::setClusterButton(GroupButton *btn, UClusterInfo *info)
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
    btn->setFixedHeight(this->screenHeight()*0.1);
    btn->setAttribute(Qt::WA_LayoutUsesWidgetRect);
    btn->setContentsMargins(0,0,this->screenWidth()*0.05, 0);
    btn->setStyleSheet(SheetStyle::GROUPBUTTON_BOTTOMBORDER);
    connect(btn, SIGNAL(clicked()), this, SLOT(on_btnClusterManagmentPage_clicked()));
}

void ContactPage::setToolButton(QToolButton *btn, QString imagePath)
{
    QIcon icon = QIcon(imagePath);
    btn->setIcon(icon);
    btn->setIconSize(QSize(btn->height()*0.9, btn->height()*0.9));
    btn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    btn->setStyleSheet(SheetStyle::TOOLBUTTON_GREY_15);
}

void ContactPage::setListWidget(ListWidget *listWidget)
{
    listWidget->setStyleSheet(SheetStyle::LISTWIDGET);
    listWidget->setContentsMargins(this->screenWidth()*0.03,0,0,0);
}

void ContactPage::setNotifyButton(HintMsgButton *btn, UNotifySummary *info)
{
    if(btn == NULL || info == NULL) return;
    btn->setMsgName(info->getNotifyName());
    btn->setMsgTime(info->getNotifyTime().right(11));
    btn->setMsgDescribe(info->getNotifyContent());
    btn->setFixedHeight(this->screenHeight()*0.11);
    btn->setMsgType(info->getNotifyType());
    btn->setIndexOne(info->getNotifyID());
    btn->setAttribute(Qt::WA_LayoutUsesWidgetRect);
    btn->setContentsMargins(0,0,0,0);
    btn->setUnreadMsgCount(info->getUnreadCount());
    btn->setStyleSheet(SheetStyle::GROUPBUTTON_BOTTOMBORDER);

    QPixmap pixmap = QPixmap(info->getNotifyImg());
    btn->setMsgPixMap(pixmap);
    connect(btn, SIGNAL(deleteClicked()), this, SLOT(on_btnDeleteNotify_clicked()));

    if(info->getNotifyType() == Mi::ClusterJoinNotify){//用户群加入通知
        connect(btn, SIGNAL(clicked()), this, SLOT(on_btnClusterJoinNotify_clicked()));
    }else if(info->getNotifyType() == Mi::UserClusterChatNotify){//用户群聊天通知
        //connect(btn, SIGNAL(clicked()), this, SLOT(on_btnClusterManagementPage_clicked()));
    }else if(info->getNotifyType() == Mi::UserJoinNotify){//用户加入通知
        connect(btn, SIGNAL(clicked()), this, SLOT(on_btnUserJoinNotify_clicked()));
    }else if(info->getNotifyType() == Mi::UserChatNotify){//用户聊天通知
        //connect(btn, SIGNAL(clicked()), this, SLOT(on_btnClusterManagementPage_clicked()));
    }
}

QWidget* ContactPage::findWidgetFromListWidget(ListWidget *listwidget, quint64 id)
{
    int count = listwidget->count();
    if(count == 0) return NULL;

    for(int i = 0; i < count; i++){
        QWidget* widget = listwidget->itemAt(i);
        GPushButton* gBtn = dynamic_cast<GPushButton*>(widget);

        if(gBtn->getIndexOne() == id)
            return widget;
    }
    return NULL;
}

UClusterInfo* ContactPage::findClusterInfo(quint64 clusID)
{
    QHash<quint64, UClusterInfo*> infoList = clusterInfoList->fetchList();
    UClusterInfo* info = infoList.value(clusID);
    return info;
}

UNotifySummary* ContactPage::findNotifyInfo(Mi::NotifyCategory type, quint64 id)
{
    QList<UNotifySummary*> infoList = notifySummaryList->fetchList();
    QListIterator<UNotifySummary*> iter(infoList);
    while (iter.hasNext()) {
        UNotifySummary* info = iter.next();
        if(info->getNotifyType() == type && info->getNotifyID() == id){
            return info;
        }
    }

    return NULL;
}

QWidget* ContactPage::findNotifyButtonFromListWidget(ListWidget *listwidget, UNotifySummary* info)
{
    int count = listwidget->count();
    for(int i = 0; i < count; i++){
        QWidget* widget = listwidget->itemAt(i);
        HintMsgButton* hBtn = dynamic_cast<HintMsgButton*>(widget);

        if(hBtn->getMsgType() == info->getNotifyType() && hBtn->getIndexOne() == info->getNotifyID())
            return widget;
    }
    return NULL;
}

void ContactPage::sortWithNotifyTime(ListWidget *listWidget, HintMsgButton *hBtn)
{
    int count = listWidget->count();
    for(int i = 0; i < count; i++){
        QWidget* wdg = listWidget->itemAt(i);
        HintMsgButton* imBtn = dynamic_cast<HintMsgButton*>(wdg);

        if(hBtn->getMsgTime() >= imBtn->getMsgTime()){
            listWidget->insertWidget(i, hBtn);
            this->listWidgetLastWidgetStyle(listWidget);
            return;
        }
    }
    this->listWidgetLastWidgetStyle(listWidget);
    listWidget->insertWidget(0, hBtn);//没有找到，直接插入
}

void ContactPage::sortWithClusterRole(ListWidget* listWidget, GroupButton* gBtn)
{
    int count = listWidget->count();
    quint64 newClusID = gBtn->getIndexOne();
    UClusterInfo* newInfo = this->findClusterInfo(newClusID);
    for(int i = 0; i < count; i++)
    {
        QWidget* widget = listWidget->itemAt(i);
        GroupButton* imBtn = dynamic_cast<GroupButton*>(widget);
        quint64 oldClusID = imBtn->getIndexOne();
        UClusterInfo* oldInfo = this->findClusterInfo(oldClusID);

        if(oldInfo == NULL || newInfo == NULL) continue;
        if(newInfo->getUserRole() <= oldInfo->getUserRole()){//角色小于等于时，插入当前位置
            listWidget->insertWidget(i, gBtn);
            this->listWidgetLastWidgetStyle(listWidget);
            return;
        }
    }

    this->listWidgetLastWidgetStyle(listWidget);
    listWidget->insertWidget(count, gBtn);//没有找到，直接插入
}

void ContactPage::fetchUserInfoReq(quint64 userID)
{
    AssemblyData assemblyData;
    assemblyData.append(userID);
    char* data = assemblyData.getAssemblyData();
    quint16 dataLen = assemblyData.getAssemblyDataLen();
    emit readyRead(CommandCode::FETCH_USER_INFO, data, dataLen);
}

void ContactPage::fetchClusterInfoReq(quint64 clusID)
{
    AssemblyData assemblyData;
    assemblyData.append(clusID);
    char* data = assemblyData.getAssemblyData();
    quint16 dataLen = assemblyData.getAssemblyDataLen();
    emit readyRead(CommandCode::FETCH_USER_CLUSTER_INFO, data, dataLen);
}

void ContactPage::adjustListWidgets(ListWidget *listwidget)
{
    curListWidget->hide();
    widgetsLayout->removeWidget(curListWidget);
    widgetsLayout->addWidget(listwidget);
    listwidget->show();//列表切换
    curListWidget = listwidget;

    int height = curFixedContentHeight + listwidget->height();
    this->setBodyRealHeight(height);
}

void ContactPage::adjustUnreadNotifyMessageCount(HintMsgButton *btn)
{
    UNotifySummary* info = this->findNotifyInfo(btn->getMsgType(), btn->getIndexOne());
    if(info != NULL){
        curHintMsgButton = btn;
        info->updateUnreadMsgCount(0);
        btn->setUnreadMsgCount(0);//按通知类型分类的未读消息归零

        int unreadCount = notifySummaryList->getTotalUnreadCount();
        btnMessage->setUnreadMsgCount(unreadCount);//未读消息总数设置
    }
}

void ContactPage::labelHintInfoDisplay()
{
    if(curListWidget == messageListWidget){
        if(messageListWidget->count() <= 0){
            lblInfoHint->setText(QString(tr("暂时没有新消息")));
        }else{
            lblInfoHint->setText(QString(tr("消息通知")));
        }

        btnMessage->setIcon(QPixmap(ImagePath::USER_MESSAGE_SELECTED));
        btnMessage->setStyleSheet(SheetStyle::TOOLBUTTON_BLUE_15);
        btnBuddy->setIcon(QPixmap(ImagePath::BUDDY_CANCELED));
        btnBuddy->setStyleSheet(SheetStyle::TOOLBUTTON_GREY_15);
        btnCluster->setIcon(QPixmap(ImagePath::USER_CLUSTER_CANCELED));
        btnCluster->setStyleSheet(SheetStyle::TOOLBUTTON_GREY_15);

    }else if(curListWidget == buddyListWidget){
        if(buddyListWidget->count() <= 0){
            lblInfoHint->setText(QString(tr("你还没有添加好友哦")));
        }else{
            lblInfoHint->setText(QString(tr("好友信息")));
        }

        btnBuddy->setIcon(QPixmap(ImagePath::BUDDY_SELECTED));
        btnBuddy->setStyleSheet(SheetStyle::TOOLBUTTON_BLUE_15);
        btnMessage->setIcon(QPixmap(ImagePath::USER_MESSAGE_CANCELED));
        btnMessage->setStyleSheet(SheetStyle::TOOLBUTTON_GREY_15);
        btnCluster->setIcon(QPixmap(ImagePath::USER_CLUSTER_CANCELED));
        btnCluster->setStyleSheet(SheetStyle::TOOLBUTTON_GREY_15);

    }else if(curListWidget == clusterListWidget){
        if(clusterListWidget->count() <= 0){
            lblInfoHint->setText(QString(tr("你还没有加入或创建群组")));
        }else{
            lblInfoHint->setText(QString(tr("群组信息")));
        }

        btnCluster->setIcon(QPixmap(ImagePath::USER_CLUSTER_SELECTED));
        btnCluster->setStyleSheet(SheetStyle::TOOLBUTTON_BLUE_15);
        btnBuddy->setIcon(QPixmap(ImagePath::BUDDY_CANCELED));
        btnBuddy->setStyleSheet(SheetStyle::TOOLBUTTON_GREY_15);
        btnMessage->setIcon(QPixmap(ImagePath::USER_MESSAGE_CANCELED));
        btnMessage->setStyleSheet(SheetStyle::TOOLBUTTON_GREY_15);
    }
}

void ContactPage::displayConcretePage(BasePage *page)
{
    if(page == NULL) return;
    page->show();
    connect(page, SIGNAL(destroyed()), this, SLOT(on_deleteSubPages()));
    connect(page, SIGNAL(readyRead(quint16,char*,quint16)), SLOT(on_recvDataFromOtherPage(quint16,char*,quint16)));
}

void ContactPage::listWidgetLastWidgetStyle(ListWidget *listWidget)
{
    for(int i = 0; i < listWidget->count(); i++){
        QWidget* widget = listWidget->itemAt(i);
        widget->setStyleSheet(SheetStyle::GROUPBUTTON_BOTTOMBORDER);
        if(i == (listWidget->count() - 1)){
            widget->setStyleSheet(SheetStyle::GROUPBUTTON_NOBORDER);
        }
    }
}

void ContactPage::transferDataToAnotherPage(InputPacket &inpack, BasePage *basePage)
{
    if(basePage == NULL) return;
        basePage->fetchData(inpack);
}

void ContactPage::on_btnAddContactsPage_clicked()
{
    //隐藏底部任务栏
    emit readyRead(CommandCode::BOTTOM_BAR_UNDISPLAY, NULL, 0);
    SAFE_DELETE(userManagementPage);
    userManagementPage = new UserManagementPage(this);
    this->displayConcretePage(userManagementPage);
}

void ContactPage::on_toolBtnMessage_clicked()
{
    this->adjustListWidgets(messageListWidget);
    this->labelHintInfoDisplay();
}

void ContactPage::on_toolBtnCluster_clicked()
{
    this->adjustListWidgets(clusterListWidget);
    this->labelHintInfoDisplay();
}

void ContactPage::on_toolBtnBuddy_clicked()
{
    this->adjustListWidgets(buddyListWidget);
    this->labelHintInfoDisplay();
}

void ContactPage::on_deleteSubPages()
{
    QString name = this->sender()->objectName();
    if(name == QString("UserJoinNotifyListPage") || name == QString("UClusterNotifyListPage"))
    {
        UNotifySummary* info = this->findNotifyInfo(curHintMsgButton->getMsgType(), curHintMsgButton->getIndexOne());
        if(info != NULL){
            info->updateUnreadMsgCount(0);
            curHintMsgButton->setUnreadMsgCount(0);//按通知类型分类的未读消息归零

            int unreadCount = notifySummaryList->getTotalUnreadCount();
            btnMessage->setUnreadMsgCount(unreadCount);//未读消息总数设置
        }
    }

    userManagementPage = NULL;
    userJoinNotifyListPage = NULL;
    clusterNotifyListPage = NULL;
    emit readyRead(CommandCode::BOTTOM_BAR_DISPLAY, NULL, 0);//显示底部任务栏
}

void ContactPage::on_btnDeleteNotify_clicked()
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

void ContactPage::on_btnUserJoinNotify_clicked()
{
    HintMsgButton* thisBtn = dynamic_cast<HintMsgButton*>(sender());
    this->adjustUnreadNotifyMessageCount(thisBtn);

    SAFE_DELETE(userJoinNotifyListPage);
    userJoinNotifyListPage = new UserJoinNotifyListPage(this);
    userJoinNotifyListPage->setObjectName(QString("UserJoinNotifyListPage"));
    this->displayConcretePage(userJoinNotifyListPage);
    emit readyRead(CommandCode::BOTTOM_BAR_UNDISPLAY, NULL, 0);//隐藏底部任务栏
}

void ContactPage::on_btnClusterJoinNotify_clicked()
{
    HintMsgButton* thisBtn = dynamic_cast<HintMsgButton*>(sender());
    this->adjustUnreadNotifyMessageCount(thisBtn);

    SAFE_DELETE(clusterNotifyListPage);
    clusterNotifyListPage = new UClusterNotifyListPage(this);
    clusterNotifyListPage->setObjectName(QString("UClusterNotifyListPage"));
    this->displayConcretePage(clusterNotifyListPage);
    emit readyRead(CommandCode::BOTTOM_BAR_UNDISPLAY, NULL, 0);//隐藏底部任务栏
}

void ContactPage::on_btnClusterManagmentPage_clicked()
{

}

void ContactPage::on_recvDataFromOtherPage(quint16 cmdCode, char *data, quint16 dataLen)
{
    if(cmdCode == CommandCode::BOTTOM_BAR_DISPLAY){
        on_deleteSubPages();
        return;
    }
    emit readyRead(cmdCode, data, dataLen);
}

ContactPage::~ContactPage()
{
    SAFE_DELETE(userInfoList);
}
