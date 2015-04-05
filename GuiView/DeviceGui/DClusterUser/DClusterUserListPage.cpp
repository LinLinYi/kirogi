#include <QAction>
#include <QVBoxLayout>
#include <QPushButton>

#include "GroupButton.h"
#include "DClusterInfo.h"
#include "ListWidget.h"
#include "MenuBar.h"
#include "UserProtocol.hpp"
#include "ImageConfig.hpp"
#include "HintMsgPage.h"
#include "InputPacket.h"
#include "AssemblyData.h"
#include "DClusterUserListPage.h"
#include "NavigationMenuBar.h"
#include "SheetStyleConfig.hpp"
#include "DClusterUserListPage.h"
#include "OnlineStatusInfoList.h"
#include "DClusterUserInfoList.h"
#include "DClusterSearchUserPage.h"
#include "DClusterDelUserPage.h"
#include "DClusterUserHasDevicesListPage.h"

DClusterUserListPage::DClusterUserListPage(QWidget* parent, quint64 clusterID)
    :BasePage(parent)
    , curClusterID(clusterID)
    , deleteUserCount(0)
    , curFixedContentHeight (0)
    , clusterInfo(NULL)
    , userInfoList(NULL)
    , loadingPage(NULL)
    , clusterdelUserPage(NULL)
    , clusterSearchUserPage(NULL)
    , clusterUserHasDevicesListPage(NULL)
{
    this->loadLocalData();
    this->initializeWidget();
}

void DClusterUserListPage::loadLocalData()
{
    userInfoList = new DClusterUserInfoList(curClusterID);
    userInfoList->loadDataSource();
}

void DClusterUserListPage::initializeWidget()
{
    //提示信息页面操作
    hintMsgPage = new HintMsgPage(this);
    hintMsgPage->hide();
    connect(hintMsgPage, SIGNAL(submitClicked(bool)), this, SLOT(on_hintMsgPage(bool)));

    //导航菜单栏
    QString strMenuLeftBtn = tr("返回");
    QString strMenuTitle = tr("成员列表");
    QPixmap pixmapRight(ImagePath::NAVIGATION_LEFT_ARROWS);
    pixmapRight.setDevicePixelRatio(2.0);
    navigationMenuBar = new NavigationMenuBar(this);
    navigationMenuBar->setLeftText(strMenuLeftBtn);
    navigationMenuBar->setTitleText(strMenuTitle);
    navigationMenuBar->setLeftPixMap(pixmapRight);
    connect(navigationMenuBar, SIGNAL(leftClicked()), this, SLOT(on_btnPrepage_clicked()));

    MenuBar* menuBar = navigationMenuBar->getMenuBar();
    addUserAction = menuBar->addAction(tr("邀请成员"));
    deleteUserAction = menuBar->addAction(tr("删除成员"));
    connect(addUserAction, SIGNAL(triggered()),this,SLOT(on_addUserAction_clicked()));
    connect(deleteUserAction, SIGNAL(triggered()), this, SLOT(on_delUserAction_clicked()));

    vbTopLayout = new QVBoxLayout();
    vbTopLayout->addWidget(navigationMenuBar);
    vbTopLayout->setSpacing(0);
    vbTopLayout->setMargin(0);
    this->setTopbarLayout(vbTopLayout);

    //搜索按钮
    QPixmap lsearchPixmap(ImagePath::BLACK_SEARCH_BAR);
    lsearchPixmap.setDevicePixelRatio(2.0);

    btnSearch = new QPushButton(this);
    btnSearch->setIcon(lsearchPixmap);
    btnSearch->setText(tr("搜索"));
    btnSearch->setFixedHeight(this->screenHeight()*0.05);
    btnSearch->setStyleSheet(SheetStyle::SEARCH_BUTTON);
    connect(btnSearch, SIGNAL(clicked()), this, SLOT(on_btnSearch_clicked()));

    QVBoxLayout* vbSearchLayout = new QVBoxLayout;
    vbSearchLayout->addWidget(btnSearch);
    vbSearchLayout->setContentsMargins(this->screenHeight()*0.016, this->screenHeight()*0.014,
                                       this->screenHeight()*0.016, this->screenHeight()*0.014);
    curFixedContentHeight += this->screenHeight()*0.05 + this->screenHeight()*0.014 + this->screenHeight()*0.014;

    //群成员布局
    userListWidget = new ListWidget(this);
    userListWidget->setStyleSheet(SheetStyle::LISTWIDGET);
    userListWidget->setContentsMargins(this->screenWidth()*0.03,0,0,0);
    curFixedContentHeight += userListWidget->height();

    QVBoxLayout* vblTotalLayout = new QVBoxLayout;
    vblTotalLayout->addLayout(vbSearchLayout);
    vblTotalLayout->addWidget(userListWidget);
    vblTotalLayout->addStretch(0);
    vblTotalLayout->setMargin(0);
    vblTotalLayout->setSpacing(0);
    vblTotalLayout->setAlignment(Qt::AlignTop);
    this->setBodyPartLayout(vblTotalLayout);

    //初始化页面数据
    this->deployUserList(userInfoList->fetchList());

    //屏幕触摸滚动设置
    this->installScrollViewportArea();
    this->setBodyRealHeight(curFixedContentHeight);
    timerID = this->startTimer(ConstNum::MAX_TIME_INTERVAL);
}

void DClusterUserListPage::fetchData(InputPacket &inpack)
{
    quint16 cmdCode = inpack.getCmdCode();
    DPRINT("-->DevClusterUserListPage receive data, the cmdCode is:0x%x", cmdCode);

    switch(cmdCode)
    {
        case CommandCode::NET_ERROR:
            this->handleNetError();
            break;
        case CommandCode::FETCH_DCLUSTER_USER_LIST:
            this->handleClusterUserListInfo(inpack);
            break;
        case CommandCode::DCLUSTER_DELETE_USER:
            this->handleClusterDelUserListInfo(inpack);
            break;
    }

    this->transferDataToOhterPage(inpack, clusterUserHasDevicesListPage);
    this->transferDataToOhterPage(inpack, clusterSearchUserPage);
    this->transferDataToOhterPage(inpack, clusterdelUserPage);
}

void DClusterUserListPage::timerEvent(QTimerEvent*)
{
    AssemblyData assemblyData;
    assemblyData.append(curClusterID);
    char* data = assemblyData.getAssemblyData();
    quint16 dataLen = assemblyData.getAssemblyDataLen();
    emit readyRead(CommandCode::FETCH_DCLUSTER_USER_LIST, data, dataLen);
    this->killTimer(timerID);
}

void DClusterUserListPage::handleNetError()
{
    if(loadingPage == NULL) return;
    QString lblText = tr("网络错误,请重试");
    handleHintInfo(lblText);
}

void DClusterUserListPage::handleHintInfo(QString& text)
{
    hintMsgPage->setText(text);
    hintMsgPage->show();
    SAFE_DELETE(loadingPage);
}

void DClusterUserListPage::handleClusterDelUserListInfo(InputPacket& inpack)
{
    quint8 ansCode = inpack.getAnswerCode();
    if(clusterdelUserPage == NULL || ansCode == Mi::Failure) return;
    QList<quint64> delUserIDList = clusterdelUserPage->getDelUserList();

    for(int uIndex = 0; uIndex < userListWidget->count(); uIndex++){
        QWidget* widget = userListWidget->itemAt(uIndex);
        GroupButton* btn = dynamic_cast<GroupButton*>(widget);
        for(int delIndex = 0; delIndex < delUserIDList.count(); delIndex++){
            if(btn->getIndexOne() == delUserIDList.at(delIndex)){
                quint64 userID = btn->getIndexOne();
                curFixedContentHeight -= btn->height();
                userListWidget->takeAt(btn);
                userInfoList->takeAt(curClusterID, userID);
                this->setBodyRealHeight(curFixedContentHeight);
            }
        }
    }
}

void DClusterUserListPage::handleClusterUserListInfo(InputPacket& inpack)
{
    SAFE_DELETE(userInfoList);
    userInfoList = new DClusterUserInfoList;

    char* data = inpack.getPackData();
    quint16 dataLen = inpack.getPackDataLen();
    userInfoList->appendList(data, dataLen);
    QHash<quint64, DClusterUserInfo*> usrList = userInfoList->fetchList();
    this->deployUserList(usrList);
}

void DClusterUserListPage::displayConcretePage(BasePage *page)
{
    if(page == NULL) return;
    page->show();
    connect(page, SIGNAL(destroyed()), this, SLOT(on_deleteSubPages()));
    connect(page, SIGNAL(readyRead(quint16,char*,quint16)), this, SLOT(on_recvDataFromOtherPage(quint16,char*,quint16)));
}

void DClusterUserListPage::deployUserList(QHash<quint64, DClusterUserInfo *> list)
{
    QHashIterator<quint64, DClusterUserInfo*> iter(list);
    while(iter.hasNext()){
        iter.next();
        DClusterUserInfo* info = dynamic_cast<DClusterUserInfo*>(iter.value());
        this->appendUserToListWidget(info);
    }

    if(curFixedContentHeight <= this->bodyPartHeight())
        this->setBodyRealHeight(this->bodyPartHeight());
    else
        this->setBodyRealHeight(curFixedContentHeight);
}

void DClusterUserListPage::appendUserToListWidget(DClusterUserInfo *info)
{
    GroupButton* btn = NULL;
    for(int i = 0; i < userListWidget->count(); i++){
        QWidget* widget = userListWidget->itemAt(i);
        btn = dynamic_cast<GroupButton*>(widget);

        if(btn->getIndexOne() == info->getUserID()){//存在，则更新
            this->setUserButton(btn, info);
            return;
        }
    }

    //不存在，则加入
    btn = new GroupButton(this);
    this->setUserButton(btn, info);
    this->sortWithUserRole(userListWidget, btn);
    this->listWidgetLastWidgetStyle(userListWidget);
    btn->setDescribe(AuthContent::GetRole(info->getUserRole()));
    curFixedContentHeight += btn->height();
}

void DClusterUserListPage::setUserButton(GroupButton* btn, DClusterUserInfo* info)
{
    if(btn == NULL || info == NULL) return;

    //设备按钮设置
    QPixmap pixmap(ImagePath::USER_PORTRAIT);
    QPixmap pixmapReserve(ImagePath::RIGHT_HOLLOW_ARROWS);
    pixmap.setDevicePixelRatio(2.0);
    pixmapReserve.setDevicePixelRatio(2.0);

    btn->setIndexOne(info->getUserID());
    btn->setText(info->getUserName());
    btn->setPixMap(pixmap);
    btn->setFixedHeight(this->screenHeight()*0.1);
    btn->setContentsMargins(0, 0, this->screenWidth()*0.03, 0);
    btn->setReserve(pixmapReserve);
    btn->setAttribute(Qt::WA_LayoutUsesWidgetRect);
    btn->setStyleSheet(SheetStyle::GROUPBUTTON_BOTTOMBORDER);
    this->listWidgetLastWidgetStyle(userListWidget);
    connect(btn, SIGNAL(clicked()), this, SLOT(on_btnDevClusterUserHasDevicesListPage_clicked()));
}

void DClusterUserListPage::sortWithUserRole(ListWidget* listWidget, GroupButton* gBtn)
{
    int count = listWidget->count();
    quint64 newClusID = gBtn->getIndexOne();
    DClusterUserInfo* newInfo = userInfoList->fetchList().value(newClusID);
    for(int i = 0; i < count; i++)
    {
        QWidget* widget = listWidget->itemAt(i);
        GroupButton* imBtn = dynamic_cast<GroupButton*>(widget);
        quint64 oldClusID = imBtn->getIndexOne();
        DClusterUserInfo* oldInfo = userInfoList->fetchList().value(oldClusID);

        if(oldInfo == NULL || newInfo == NULL) continue;
        if(newInfo->getUserRole() <= oldInfo->getUserRole()){//角色小于等于时，插入当前位置
            listWidget->insertWidget(i, gBtn);
            return;
        }
    }
    listWidget->insertWidget(count, gBtn);//没有找到，直接插入
}

void DClusterUserListPage::listWidgetLastWidgetStyle(ListWidget *listWidget)
{
    for(int i = 0; i < listWidget->count(); i++){
        QWidget* widget = listWidget->itemAt(i);
        widget->setStyleSheet(SheetStyle::GROUPBUTTON_BOTTOMBORDER);
        if(i == (listWidget->count() - 1)){
            widget->setStyleSheet(SheetStyle::GROUPBUTTON_NOBORDER);
        }
    }
}

void DClusterUserListPage::transferDataToOhterPage(InputPacket &inpack, BasePage *basePage)
{
    if(basePage == NULL) return;
    basePage->fetchData(inpack);
}

void DClusterUserListPage::on_btnSearch_clicked()
{

}

void DClusterUserListPage::on_addUserAction_clicked()
{
    SAFE_DELETE(clusterSearchUserPage);
    clusterSearchUserPage = new DClusterSearchUserPage(this, curClusterID);
    this->displayConcretePage(clusterSearchUserPage);
}

void DClusterUserListPage::on_delUserAction_clicked()
{
    SAFE_DELETE(clusterdelUserPage);
    clusterdelUserPage = new DClusterDelUserPage(this, curClusterID);
    this->displayConcretePage(clusterdelUserPage);
}

void DClusterUserListPage::on_btnDevClusterUserHasDevicesListPage_clicked()
{
    GroupButton* btn = dynamic_cast<GroupButton*>(sender());
    quint64 userID = btn->getIndexOne();
    SAFE_DELETE(clusterUserHasDevicesListPage);
    clusterUserHasDevicesListPage = new DClusterUserHasDevicesListPage(this, curClusterID, userID);
    this->displayConcretePage(clusterUserHasDevicesListPage);
}

void DClusterUserListPage::on_deleteSubPages()
{
    clusterdelUserPage = NULL;
    clusterSearchUserPage = NULL;
    clusterUserHasDevicesListPage = NULL;
}

void DClusterUserListPage::on_recvDataFromOtherPage(quint16 cmdCode, char *data, quint16 dataLen)
{
    if(cmdCode == CommandCode::BACK_CLUSTER_MANAGE_PAGE){
        this->hide();
        this->deleteLater();
        return;
    }
    emit readyRead(cmdCode, data, dataLen);
}

void DClusterUserListPage::on_hintMsgPage(bool isJump)
{
    if(!isJump)
        hintMsgPage->hide();
}

void DClusterUserListPage::on_btnPrepage_clicked()
{
    this->hide();
    this->deleteLater();
}

DClusterUserListPage::~DClusterUserListPage()
{
    SAFE_DELETE(userInfoList);
}

