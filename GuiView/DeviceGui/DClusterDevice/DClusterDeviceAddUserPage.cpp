#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>

#include "UserProtocol.hpp"
#include "GroupButton.h"
#include "NavigationBar.h"
#include "LoadingPage.h"
#include "HintMsgPage.h"
#include "ImageConfig.hpp"
#include "NavigationBar.h"
#include "InputPacket.h"
#include "AssemblyData.h"
#include "SheetStyleConfig.hpp"
#include "DClusterUserInfoList.h"
#include "UserAuthorizedInfoList.h"
#include "DClusterDeviceAddUserPage.h"

DClusterDeviceAddUserPage::DClusterDeviceAddUserPage(QWidget* parent, quint64 clusterID, quint64 deviceID)
    : BasePage(parent)
    , curClusterID(clusterID)
    , curDeviceID(deviceID)
    , curFixedContentHeight(0)
    , userAddCount(0)
    , loadingPage(NULL)
    , userInfoList(NULL)
{
    this->initializeWidget();
}

void DClusterDeviceAddUserPage::initializeWidget()
{
    //提示信息页面操作
    hintMsgPage = new HintMsgPage(this);
    hintMsgPage->hide();
    connect(hintMsgPage, SIGNAL(submitClicked(bool)), this, SLOT(on_hintMsgPage(bool)));

    //导航栏
    QString strTitle = tr("添加成员");
    QString strLeftBtn = tr("返回");
    QString strRightBtn = tr("确认");
    QPixmap leftArrowsPixmap(ImagePath::NAVIGATION_LEFT_ARROWS);
    navigationBar = new NavigationBar(this);
    navigationBar->setTitleText(strTitle);
    navigationBar->setLeftText(strLeftBtn);
    navigationBar->setRightText(strRightBtn);
    navigationBar->setLeftPixMap(leftArrowsPixmap);
    connect(navigationBar, SIGNAL(leftClicked()), this, SLOT(on_btnPrePage_clicked()));
    connect(navigationBar, SIGNAL(rightClicked()), this, SLOT(on_addUser_clicked()));

    QHBoxLayout* pHLTop = new QHBoxLayout();
    pHLTop->addWidget(navigationBar);
    pHLTop->setSpacing(0);
    pHLTop->setMargin(0);
    this->setTopbarLayout(pHLTop);

    //群成员布局
    userListLayout = new QVBoxLayout();
    userListLayout->setMargin(0);
    userListLayout->setSpacing(0);
    userListLayout->setContentsMargins(0, 0, 0, 0);

    QVBoxLayout* vblTotalLayout = new QVBoxLayout();
    vblTotalLayout->addLayout(userListLayout);
    vblTotalLayout->addStretch(0);
    vblTotalLayout->setMargin(0);
    vblTotalLayout->setSpacing(0);
    vblTotalLayout->setAlignment(Qt::AlignTop);
    this->setBodyPartLayout(vblTotalLayout);

    //屏幕触摸滚动设置
    this->installScrollViewportArea();
    timerID = this->startTimer(ConstNum::MAX_TIME_INTERVAL);
}

void DClusterDeviceAddUserPage::fetchData(InputPacket &inpack)
{
    quint16 cmdCode = inpack.getCmdCode();
    DPRINT("-->DevClusterDeviceAddUserPage receive data, the cmdCode is:0x%x", cmdCode);

    switch(cmdCode)
    {
        case CommandCode::NET_ERROR:
            this->handleNetError();
            break;
        case CommandCode::FETCH_DCLUSTER_USER_LIST:
            this->handleDevClusterUserList(inpack);
            break;
        case CommandCode::DCLUSTER_DEVICE_ADD_USER:
            this->recvUserInfoFromServer(inpack);
            break;
        case CommandCode::FETCH_DEV_AUTHORIZED_USER_LIST:
            this->handleAuthorizedUserList(inpack);
            break;
    }
}

void DClusterDeviceAddUserPage::timerEvent(QTimerEvent*)
{
    AssemblyData assemblyData;
    assemblyData.append(curClusterID);
    char* data = assemblyData.getAssemblyData();
    quint16 dataLen = assemblyData.getAssemblyDataLen();
    emit readyRead(CommandCode::FETCH_DCLUSTER_USER_LIST, data, dataLen);
    this->killTimer(timerID);
}

void DClusterDeviceAddUserPage::handleNetError()
{
    if(loadingPage == NULL)  return;
    for(int i = 0; i < usrIdList.count(); i++)
        usrIdList.removeAt(i);

    QString lblText = tr("网络错误,请重试");
    handleHintInfo(lblText);
}

void DClusterDeviceAddUserPage::handleHintInfo(QString& text)
{
    hintMsgPage->setText(text);
    hintMsgPage->show();
    SAFE_DELETE(loadingPage);
}

void DClusterDeviceAddUserPage::handleDevClusterUserList(InputPacket &inpack)
{
    char* data = inpack.getPackData();
    quint16 dataLen = inpack.getPackDataLen();

    SAFE_DELETE(userInfoList);
    userInfoList = new DClusterUserInfoList;
    userInfoList->appendList(data, dataLen);

    //获取已授权成员列表
    AssemblyData assemblyData;
    assemblyData.append(curClusterID);
    assemblyData.append(curDeviceID);
    char* packbuf = assemblyData.getAssemblyData();
    quint16 packbufLen = assemblyData.getAssemblyDataLen();
    emit readyRead(CommandCode::FETCH_DEV_AUTHORIZED_USER_LIST, packbuf, packbufLen);
}


void DClusterDeviceAddUserPage::handleAuthorizedUserList(InputPacket &inpack)
{
    //已授权成员列表
    char* data = inpack.getPackData();
    quint16 dataLen = inpack.getPackDataLen();
    UserAuthorizedInfoList authorizedList;
    authorizedList.appendList(data, dataLen);
    QHash<quint64, UserAuthorizedInfo*> authList = authorizedList.fetchList();
    QHashIterator<quint64, UserAuthorizedInfo*> iterAuth(authList);

    //群成员列表
    QHash<quint64, DClusterUserInfo*> usrList = userInfoList->fetchList();
    QHashIterator<quint64, DClusterUserInfo*> iterCluster(usrList);

    //未授权成员列表
    while(iterCluster.hasNext())
    {
        iterCluster.next();
        bool flag = true;

        quint64 clusUsrID = (iterCluster.value())->getUserID();
        if(authList.count() > 0){
            while(iterAuth.hasNext()){
                iterAuth.next();
                UserAuthorizedInfo* authUserInfo = dynamic_cast<UserAuthorizedInfo*>(iterAuth.value());
                quint64 authUsrID = authUserInfo->getUserID();
                if(authUsrID == clusUsrID){
                    flag = false;
                    userInfoList->remove(authUsrID);
                    break;
                }
                else if(!iterAuth.hasNext())
                    flag = true;
            }
            iterAuth.toFront();
        }

        if(flag){
            DClusterUserInfo* usrInfo = dynamic_cast<DClusterUserInfo*>(iterCluster.value());
            this->setUserButton(usrInfo);

            if(curFixedContentHeight <= this->bodyPartHeight())
                this->setBodyRealHeight(this->bodyPartHeight());
            else
                this->setBodyRealHeight(curFixedContentHeight);
        }
    }
}

void DClusterDeviceAddUserPage::recvUserInfoFromServer(InputPacket& inpack)
{
    quint8 ansCode = inpack.getAnswerCode();
    if(ansCode == Mi::Success){
        QString strText = tr("成员添加成功");
        hintMsgPage->setStatus(true);
        this->handleHintInfo(strText);
    }
    else if(ansCode == Mi::Failure){
        QString strText = tr("成员添加失败,请重试");
        this->handleHintInfo(strText);
    }
}

void DClusterDeviceAddUserPage::setUserButton(DClusterUserInfo *usrInfo)
{
    if(usrInfo == NULL) return;
    if(usersBtnList.contains(usrInfo->getUserID())) return;

    //成员按钮设置
    GroupButton* btn = new GroupButton(this);
    btn->setIndexOne(usrInfo->getUserID());
    QPixmap pixmap(ImagePath::DEVICE_PORTRAIT);
    QPixmap pixmapReserve(ImagePath::SELECT_CANCEL);
    pixmap.setDevicePixelRatio(2.0);
    pixmapReserve.setDevicePixelRatio(2.0);

    btn->setPixMap(pixmap);
    btn->setText(usrInfo->getUserName());
    btn->setFixedHeight(this->screenHeight()*0.1);
    btn->setFixedWidth(this->screenWidth());
    btn->setStyleSheet(SheetStyle::GROUPBUTTON_BOTTOMBORDER);
    btn->setReserve(pixmapReserve);
    btn->setAttribute(Qt::WA_LayoutUsesWidgetRect);

    usersBtnList.insert(usrInfo->getUserID(), btn);
    connect(btn, SIGNAL(clicked()), this, SLOT(on_btnUserButton_clicked()));

    userListLayout->addWidget(btn);
    curFixedContentHeight += btn->height();
    this->repaint();
}

void DClusterDeviceAddUserPage::on_addUser_clicked()
{
    if(userAddCount == 0){
        QString strText = tr("请选择添加的成员");
        this->handleHintInfo(strText);
        return;
    }

    QHashIterator<quint64, GroupButton*> iter(usersBtnList);
    AssemblyData assemblyData;
    assemblyData.append(curClusterID);
    assemblyData.append(curDeviceID);
    assemblyData.append(userAddCount);

    while(iter.hasNext()){
        iter.next();
        GroupButton* groupButton = iter.value();
        if(groupButton->getSelectFlag()){
            quint64 userID = groupButton->getIndexOne();
            assemblyData.append(userID);
            usrIdList.append(userID);

            quint8 authRole = Mi::SeniorManager;
            assemblyData.append(authRole);
        }
    }

    char* data = assemblyData.getAssemblyData();
    quint16 dataLen = assemblyData.getAssemblyDataLen();
    emit readyRead(CommandCode::DCLUSTER_DEVICE_ADD_USER, data, dataLen);

    //页面提示信息
    if(loadingPage == NULL){
        loadingPage = new LoadingPage(this);
        loadingPage->show();
    }
}

void DClusterDeviceAddUserPage::on_btnUserButton_clicked()
{
    GroupButton* groupButton = dynamic_cast<GroupButton*>(sender());
    QPixmap pixmapBtnDelete(ImagePath::SELECT_SUBMIT);
    pixmapBtnDelete.setDevicePixelRatio(2.0);

    QPixmap pixmapBtnCancel(ImagePath::SELECT_CANCEL);
    pixmapBtnCancel.setDevicePixelRatio(2.0);

    if(groupButton->getSelectFlag() == false){
        groupButton->setReserve(pixmapBtnDelete);
        groupButton->setSelectFlag(true);
        userAddCount++;
    }
    else{
        groupButton->setReserve(pixmapBtnCancel);
        groupButton->setSelectFlag(false);
        userAddCount--;
    }

    if(userAddCount > 0){
        QString strLeftBtn = tr("取消");
        QPixmap leftArrowsPixmap("");
        navigationBar->setLeftText(strLeftBtn);
        navigationBar->setLeftPixMap(leftArrowsPixmap);
        disconnect(navigationBar, SIGNAL(leftClicked()), this, SLOT(on_btnPrePage_clicked()));
        connect(navigationBar, SIGNAL(leftClicked()), this, SLOT(on_btnCancel_clicked()));
    }
    else{
        QString strLeftBtn = tr("返回");
        QPixmap leftArrowsPixmap(ImagePath::NAVIGATION_LEFT_ARROWS);
        navigationBar->setLeftText(strLeftBtn);
        navigationBar->setLeftPixMap(leftArrowsPixmap);
        disconnect(navigationBar, SIGNAL(leftClicked()), this, SLOT(on_btnCancel_clicked()));
        connect(navigationBar, SIGNAL(leftClicked()), this, SLOT(on_btnPrePage_clicked()));
    }
}

void DClusterDeviceAddUserPage::on_btnCancel_clicked()
{
    QHashIterator<quint64, GroupButton*> iter(usersBtnList);
    QPixmap pixmapBtnCancel(ImagePath::SELECT_CANCEL);
    pixmapBtnCancel.setDevicePixelRatio(2.0);

    while(iter.hasNext()){
        iter.next();
        GroupButton* groupButton = iter.value();
        if(groupButton->getSelectFlag()){
            groupButton->setReserve(pixmapBtnCancel);
            groupButton->setSelectFlag(false);
            userAddCount--;
        }
    }

    QString strLeftBtn = tr("返回");
    QPixmap leftArrowsPixmap(ImagePath::NAVIGATION_LEFT_ARROWS);
    navigationBar->setLeftText(strLeftBtn);
    navigationBar->setLeftPixMap(leftArrowsPixmap);
    disconnect(navigationBar, SIGNAL(leftClicked()), this, SLOT(on_btnCancel_clicked()));
    connect(navigationBar, SIGNAL(leftClicked()), this, SLOT(on_btnPrePage_clicked()));
}

void DClusterDeviceAddUserPage::on_hintMsgPage(bool isJump)
{
    if(!isJump)
        hintMsgPage->hide();
    else{
        this->hide();
        this->deleteLater();
    }
}

void DClusterDeviceAddUserPage::on_btnPrePage_clicked()
{
    this->hide();
    this->deleteLater();
}

DClusterDeviceAddUserPage::~DClusterDeviceAddUserPage()
{
    SAFE_DELETE(userInfoList);
}


