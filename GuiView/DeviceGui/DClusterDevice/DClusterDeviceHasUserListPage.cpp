#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QAction>

#include "MenuBar.h"
#include "UserProtocol.hpp"
#include "DClusterDevInfo.h"
#include "ImageConfig.hpp"
#include "AssemblyData.h"
#include "InputPacket.h"
#include "NavigationBar.h"
#include "SheetStyleConfig.hpp"
#include "NavigationMenuBar.h"
#include "GroupButton.h"
#include "GListWidget.h"
#include "DClusterDeviceAddUserPage.h"
#include "DClusterDeviceDelUserPage.h"
#include "DClusterDeviceDetailPage.h"
#include "UserAuthorizedInfoList.h"
#include "DClusterDeviceHasUserListPage.h"

DClusterDeviceHasUserListPage::DClusterDeviceHasUserListPage(QWidget* parent, quint64 clusterID, quint64 deviceID)
    : BasePage(parent)
    , groupCount(0)
    , curDeviceID(deviceID)
    , curClusterID(clusterID)
    , curFixedContentHeight(0)
    , authorizedUserInfoList(NULL)
    , clusterDeviceAddUserPage(NULL)
    , clusterDeviceDelUserPage(NULL)
    , clusterDeviceDetailPage(NULL)
{
    this->initializeWidget();
}

void DClusterDeviceHasUserListPage::initializeWidget()
{
    //导航栏
    QString strLeftBtn = tr("上一步");
    QString strTitle = tr("设备所属成员");
    navigationMenuBar = new NavigationMenuBar(this);
    navigationMenuBar->setLeftText(strLeftBtn);
    navigationMenuBar->setTitleText(strTitle);
    connect(navigationMenuBar, SIGNAL(leftClicked()), this, SLOT(on_btnPrepage_clicked()));

    MenuBar* menuBar = navigationMenuBar->getMenuBar();
    addUserAction = menuBar->addAction(tr("添加成员"));
    deleteUserAction = menuBar->addAction(tr("删除成员"));
    connect(addUserAction, SIGNAL(triggered()),this,SLOT(on_addUserAction_clicked()));
    connect(deleteUserAction, SIGNAL(triggered()), this, SLOT(on_deleteUserAction_clicked()));

    QHBoxLayout* pHLTop = new QHBoxLayout();
    pHLTop->addWidget(navigationMenuBar);
    pHLTop->setSpacing(0);
    pHLTop->setMargin(0);
    this->setTopbarLayout(pHLTop);

    //成员信息
    QPixmap pixmapLeft(ImagePath::DEVICE_PORTRAIT);
    QPixmap pixmapRight(ImagePath::RIGHT_HOLLOW_ARROWS);
    btnDeviceDetail = new GroupButton(this);
    btnDeviceDetail->setPixMap(pixmapLeft);
    btnDeviceDetail->setReserve(pixmapRight);
    btnDeviceDetail->setAttribute(Qt::WA_LayoutUsesWidgetRect);
    btnDeviceDetail->setFixedHeight(this->screenHeight()*0.12);
    btnDeviceDetail->setStyleSheet(SheetStyle::GROUPBUTTON_BOTTOMBORDER);
    curFixedContentHeight += btnDeviceDetail->height();
    connect(btnDeviceDetail, SIGNAL(clicked()), this, SLOT(on_btnDeviceInfo_clicked()));

    //设备所属成员列表
    userListWidget = new GListWidget(this);
    this->setGListWidget(userListWidget);
    connect(userListWidget, SIGNAL(flexClicked(int)), this, SLOT(on_btnListWidgetFlex_clicked(int)));

    //主体布局
    QVBoxLayout* vbLayout = new QVBoxLayout;
    vbLayout->addWidget(btnDeviceDetail);
    vbLayout->addSpacing(this->screenHeight()*0.037);
    vbLayout->addWidget(userListWidget);
    vbLayout->setSpacing(0);
    vbLayout->setMargin(0);
    vbLayout->addStretch(0);
    curFixedContentHeight += this->screenHeight()*0.037;
    this->setBodyPartLayout(vbLayout);

    //屏幕触摸滚动设置
    this->installScrollViewportArea();
    this->loadLocalData();
    timerID = this->startTimer(ConstNum::MAX_TIME_INTERVAL);
}

void DClusterDeviceHasUserListPage::loadLocalData()
{
    DClusterDevInfo devInfo;
    devInfo.loadDataSource(curClusterID, curDeviceID);
    btnDeviceDetail->setText(devInfo.getDeviceName());
}

void DClusterDeviceHasUserListPage::fetchData(InputPacket &inpack)
{
    quint16 cmdCode = inpack.getCmdCode();
    switch(cmdCode)
    {
        case CommandCode::FETCH_DEV_AUTHORIZED_USER_LIST:
            this->handleDeviceAuthorizedUserList(inpack);
            break;
        case CommandCode::DCLUSTER_DEVICE_ADD_USER:
            this->handleClusterAddUserInfo(inpack);
            break;
        case CommandCode::DCLUSTER_DEVICE_DELETE_USER:
            this->handleClusterDelUserInfo(inpack);
            break;
    }
    this->transferDataToAnotherPage(inpack, clusterDeviceDetailPage);
    this->transferDataToAnotherPage(inpack, clusterDeviceAddUserPage);
    this->transferDataToAnotherPage(inpack, clusterDeviceDelUserPage);
}

void DClusterDeviceHasUserListPage::timerEvent(QTimerEvent*)
{
    this->sendFetchAuthorizedUserCmd();
    this->killTimer(timerID);
}

void DClusterDeviceHasUserListPage::sendFetchAuthorizedUserCmd()
{
    AssemblyData assemblyData;
    assemblyData.append(curClusterID);
    assemblyData.append(curDeviceID);
    char* data = assemblyData.getAssemblyData();
    quint16 dataLen = assemblyData.getAssemblyDataLen();
    emit readyRead(CommandCode::FETCH_DEV_AUTHORIZED_USER_LIST, data, dataLen);
}

void DClusterDeviceHasUserListPage::handleDeviceAuthorizedUserList(InputPacket &inpack)
{
    //处理网络信息数据
    char* data = inpack.getPackData();
    quint16 dataLen = inpack.getPackDataLen();

    //处理已授权用户列表
    SAFE_DELETE(authorizedUserInfoList);
    authorizedUserInfoList = new UserAuthorizedInfoList;
    authorizedUserInfoList->appendList(data, dataLen);

    QHash<quint64, UserAuthorizedInfo*> userList = authorizedUserInfoList->fetchList();
    QHashIterator<quint64, UserAuthorizedInfo*> iter(userList);
    while(iter.hasNext())
    {
        iter.next();
        UserAuthorizedInfo* userInfo = dynamic_cast<UserAuthorizedInfo*>(iter.value());
        for(int i = 0; i < userListWidget->itemCount(); i++){
            QWidget* widget = userListWidget->itemAt(i);
            GroupButton* userBtn = dynamic_cast<GroupButton*>(widget);
            if(userBtn->getIndexOne() == userInfo->getUserID()) {
                userInfo = NULL;
                break;
            }
        }

        if(userInfo){
            this->setUserWidget(userInfo, userListWidget);
        }
    }
}

void DClusterDeviceHasUserListPage::handleClusterAddUserInfo(InputPacket &inpack)
{
    quint8 ansCode = inpack.getAnswerCode();
    if(ansCode == Mi::Success){
        SAFE_DELETE(authorizedUserInfoList);
        this->sendFetchAuthorizedUserCmd();
    }
}

void DClusterDeviceHasUserListPage::handleClusterDelUserInfo(InputPacket &inpack)
{
    quint8 ansCode = inpack.getAnswerCode();
    if(ansCode == Mi::Success && clusterDeviceDelUserPage != NULL){
        QList<quint64> deledUsrList = clusterDeviceDelUserPage->getDelUserList();
        this->delAuthorizedUserWidget(deledUsrList);
    }
}

void DClusterDeviceHasUserListPage::setGListWidget(GListWidget *listWidget)
{
    QPixmap pixmap(ImagePath::DOWNWARD_SOLID_ARROWS);
    pixmap.setDevicePixelRatio(2.0);

    listWidget->setPixmap(pixmap);
    listWidget->setListVisible(true);
    listWidget->setTitle(tr("所属成员"));
    listWidget->setTitleHeight(this->screenHeight()*0.08);
    listWidget->setContentsMargins(this->screenWidth()*0.03,0,0,0);
    listWidget->setTitleStyleSheet(SheetStyle::GROUPBUTTON_TOPBORDER);
}

void DClusterDeviceHasUserListPage::setUserWidget(UserAuthorizedInfo *userInfo, GListWidget* listWidget)
{
    if(userInfo == NULL || listWidget == NULL) return;
    QPixmap pixmap(ImagePath::USER_PORTRAIT);
    pixmap.setDevicePixelRatio(2.0);

    //用户按钮设置
    GroupButton* usrBtn = new GroupButton(this);
    QString title = userInfo->getUserName();
    usrBtn->setIndexOne(userInfo->getUserID());
    usrBtn->setText(title);
    usrBtn->setPixMap(pixmap);
    usrBtn->setAttribute(Qt::WA_LayoutUsesWidgetRect);
    usrBtn->setFixedHeight(this->screenHeight()*0.1);
    usrBtn->setContentsMargins(0,0,0,0);
    usrBtn->setStyleSheet(SheetStyle::GROUPBUTTON_TOPBORDER);

    //将用户加到相应的组
    listWidget->addWidget(usrBtn);
    if(listWidget->isListVisible()){
        qint16 height = curFixedContentHeight + userListWidget->height();
        this->setBodyRealHeight(height);
    }
}

void DClusterDeviceHasUserListPage::transferDataToAnotherPage( InputPacket &inpack, BasePage *basePage)
{
    if(basePage == NULL) return;
    basePage->fetchData(inpack);
}

void DClusterDeviceHasUserListPage::on_btnDeviceInfo_clicked()
{
    clusterDeviceDetailPage  = new DClusterDeviceDetailPage(this, curClusterID, curDeviceID);
    this->displayConcretePage(clusterDeviceDetailPage);
}

void DClusterDeviceHasUserListPage::on_addUserAction_clicked()
{
    clusterDeviceAddUserPage = new DClusterDeviceAddUserPage(this, curClusterID, curDeviceID);
    this->displayConcretePage(clusterDeviceAddUserPage);
}

void DClusterDeviceHasUserListPage::on_deleteUserAction_clicked()
{
    clusterDeviceDelUserPage = new DClusterDeviceDelUserPage(this, curClusterID, curDeviceID);
    this->displayConcretePage(clusterDeviceDelUserPage);
}

void DClusterDeviceHasUserListPage::on_btnListWidgetFlex_clicked(int listHeight)
{
    QPixmap pixmap;
    GListWidget *listWidget = dynamic_cast<GListWidget *>(sender());

    if(listHeight <= 0){
        pixmap.load(ImagePath::RIGHT_SOLID_ARROWS);
        listWidget->setPixmap(pixmap);
    }
    else{
        pixmap.load(ImagePath::DOWNWARD_SOLID_ARROWS);
        listWidget->setPixmap(pixmap);
    }

    int curHeight = curFixedContentHeight + userListWidget->height();
    if(curHeight <= (int)this->scrollAreaNoBottomBarHeight())
        this->setBodyRealHeight(this->scrollAreaNoBottomBarHeight());
    else
        this->setBodyRealHeight(curHeight);
}

void DClusterDeviceHasUserListPage::on_btnPrepage_clicked()
{
    this->hide();
    this->deleteLater();
}

void DClusterDeviceHasUserListPage::on_deleteSubPages()
{
    clusterDeviceAddUserPage = NULL;
    clusterDeviceDelUserPage = NULL;
    clusterDeviceDetailPage = NULL;
}

void DClusterDeviceHasUserListPage::on_recvDataFromOtherPage(quint16 cmdCode, char *data, quint16 dataLen)
{
    emit readyRead(cmdCode, data, dataLen);
}

void DClusterDeviceHasUserListPage::delAuthorizedUserWidget(QList<quint64> &delList)
{
    if(delList.count() == 0) return;
    QHash<quint64, UserAuthorizedInfo*> usrList = authorizedUserInfoList->fetchList();
    QHashIterator<quint64, UserAuthorizedInfo*> iter(usrList);
    while(iter.hasNext())
    {
        iter.next();
        UserAuthorizedInfo* info = iter.value();
        for(int i = 0; i < delList.count(); i++){
            if(info->getUserID() == delList.at(i)){
                this->delUserWidget(info->getUserID());
                authorizedUserInfoList->remove(info->getUserID());
                break;
            }
        }
    }
}

void DClusterDeviceHasUserListPage::delUserWidget(quint64 usrID)
{
    int count = userListWidget->itemCount();
    for(int i = 0; i < count; i++)
    {
        QWidget* widget = userListWidget->itemAt(i);
        GroupButton* btn = dynamic_cast<GroupButton*>(widget);
        quint64 userID = btn->getIndexOne();

        if(userID == usrID){
            userListWidget->takeAt(i);
            qint16 height = curFixedContentHeight + userListWidget->height();
            this->setBodyRealHeight(height);
            return;
        }
    }
}

void DClusterDeviceHasUserListPage::displayConcretePage(BasePage *page)
{
    if(page == NULL) return;
    page->show();
    connect(page, SIGNAL(destroyed()), this, SLOT(on_deleteSubPages()));
    connect(page, SIGNAL(readyRead(quint16,char*,quint16)), this, SLOT(on_recvDataFromOtherPage(quint16,char*,quint16)));
}

DClusterDeviceHasUserListPage::~DClusterDeviceHasUserListPage()
{
    SAFE_DELETE(authorizedUserInfoList);
}

