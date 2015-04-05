#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>

#include "GroupButton.h"
#include "UserProtocol.hpp"
#include "NavigationBar.h"
#include "ImageConfig.hpp"
#include "HintMsgPage.h"
#include "LoadingPage.h"
#include "InputPacket.h"
#include "AssemblyData.h"
#include "SheetStyleConfig.hpp"
#include "UserAuthorizedInfoList.h"
#include "DClusterDeviceDelUserPage.h"

DClusterDeviceDelUserPage::DClusterDeviceDelUserPage(QWidget* parent, quint64 clusterID, quint64 deviceID)
    : BasePage(parent)
    , curClusterID(clusterID)
    , curDeviceID(deviceID)
    , curFixedContentHeight(0)
    , userDelCount(0)
    , loadingPage(NULL)
    ,  userList(NULL)
{
    initializeWidget();
}

void DClusterDeviceDelUserPage::initializeWidget()
{
    //提示信息页面操作
    hintMsgPage = new HintMsgPage(this);
    hintMsgPage->hide();
    connect(hintMsgPage, SIGNAL(submitClicked(bool)), this, SLOT(on_hintMsgPage(bool)));

    //导航栏
    QString strTitle = tr("删除成员");
    QString strLeftBtn = tr("返回");
    QString strRightBtn = tr("删除");
    QPixmap leftArrowsPixmap(ImagePath::NAVIGATION_LEFT_ARROWS);
    navigationBar = new NavigationBar(this);
    navigationBar->setTitleText(strTitle);
    navigationBar->setLeftText(strLeftBtn);
    navigationBar->setRightText(strRightBtn);
    navigationBar->setLeftPixMap(leftArrowsPixmap);
    connect(navigationBar, SIGNAL(leftClicked()), this, SLOT(on_btnPrePage_clicked()));
    connect(navigationBar, SIGNAL(rightClicked()), this, SLOT(on_btnDelUser_clicked()));

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

void DClusterDeviceDelUserPage::fetchData(InputPacket &inpack)
{
    quint16 cmdCode = inpack.getCmdCode();
    DPRINT("-->DevClusterDeviceDelUserPage receive data, the cmdCode is:0x%x", cmdCode);

    switch(cmdCode)
    {
        case CommandCode::NET_ERROR:
            this->handleNetError();
            break;
        case CommandCode::DCLUSTER_DEVICE_DELETE_USER:
            this->handleDelUserInfo(inpack);
            break;
        case CommandCode::FETCH_DEV_AUTHORIZED_USER_LIST:
            this->handleAuthorizedUserList(inpack);
            break;
    }
}

void DClusterDeviceDelUserPage::timerEvent(QTimerEvent*)
{
    AssemblyData assemblyData;
    assemblyData.append(curClusterID);
    assemblyData.append(curDeviceID);
    char* data = assemblyData.getAssemblyData();
    quint16 dataLen = assemblyData.getAssemblyDataLen();
    emit readyRead(CommandCode::FETCH_DEV_AUTHORIZED_USER_LIST, data, dataLen);
    this->killTimer(timerID);
}

void DClusterDeviceDelUserPage::handleNetError()
{
    if(loadingPage == NULL) return;
    for(int i = 0; i < userIdList.count(); i++)
        userIdList.removeAt(i);

    QString strText = tr("网络错误,请重试");
    this->handleHintInfo(strText);
}

void DClusterDeviceDelUserPage::handleHintInfo(QString& text)
{
    hintMsgPage->setText(text);
    hintMsgPage->show();
    SAFE_DELETE(loadingPage);
}

void DClusterDeviceDelUserPage::handleDelUserInfo(InputPacket& inpack)
{
    quint8 ansCode = inpack.getAnswerCode();
    if(ansCode == Mi::Success){
        QString strText = tr("成员删除成功");
        hintMsgPage->setStatus(true);
        this->handleHintInfo(strText);
    }
    else if(ansCode == Mi::Failure){
        QString strText = tr("成员删除失败");
        this->handleHintInfo(strText);
    }
}

void DClusterDeviceDelUserPage::handleAuthorizedUserList(InputPacket &inpack)
{
    if(userList == NULL)
        userList = new UserAuthorizedInfoList;
    
    char* data = inpack.getPackData();
    quint16 dataLen = inpack.getPackDataLen();
    userList->appendList(data, dataLen);
    
    QHash<quint64, UserAuthorizedInfo*> usrList = userList->fetchList();
    QHashIterator<quint64, UserAuthorizedInfo*> iter(usrList);
    
    while(iter.hasNext())
    {
        iter.next();
        UserAuthorizedInfo* usrInfo = dynamic_cast<UserAuthorizedInfo*>(iter.value());
        this->setUserButton(usrInfo);
        
        if(curFixedContentHeight <= this->bodyPartHeight())
            this->setBodyRealHeight(this->bodyPartHeight());
        else
            this->setBodyRealHeight(curFixedContentHeight);
    }
}

void DClusterDeviceDelUserPage::setUserButton(UserAuthorizedInfo *usrInfo)
{
    if(usrInfo == NULL)
        return;

    //设备按钮设置
    GroupButton* btn = new GroupButton(this);
    btn->setIndexOne(usrInfo->getUserID());
    QPixmap pixmap(ImagePath::DEVICE_PORTRAIT);
    QPixmap pixmapReserve(ImagePath::SELECT_CANCEL);
    pixmap.setDevicePixelRatio(2.0);
    pixmapReserve.setDevicePixelRatio(2.0);

    btn->setText(usrInfo->getUserName());
    btn->setPixMap(pixmap);
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

void DClusterDeviceDelUserPage::on_btnDelUser_clicked()
{
    if(userDelCount == 0){
        QString strText = tr("请选择删除的成员");
        handleHintInfo(strText);
        return;
    }

    QHashIterator<quint64, GroupButton*> iter(usersBtnList);
    AssemblyData assemblyData;
    assemblyData.append(curClusterID);
    assemblyData.append(curDeviceID);
    assemblyData.append(userDelCount);

    while(iter.hasNext()){
        iter.next();
        GroupButton* groupButton = iter.value();
        if(groupButton->getSelectFlag()){
            quint64 userID = groupButton->getIndexOne();
            assemblyData.append(userID);
            userIdList.append(userID);

            quint8 authCode = Mi::SeniorManager;
            assemblyData.append(authCode);
        }
    }

    char* data = assemblyData.getAssemblyData();
    quint16 dataLen = assemblyData.getAssemblyDataLen();
    emit readyRead(CommandCode::DCLUSTER_DEVICE_DELETE_USER, data, dataLen);

    //页面提示信息
    if(loadingPage == NULL){
        loadingPage = new LoadingPage(this);
        loadingPage->show();
    }
}

void DClusterDeviceDelUserPage::on_btnUserButton_clicked()
{
    GroupButton* groupButton = dynamic_cast<GroupButton*>(sender());
    QPixmap pixmapBtnDelete(ImagePath::SELECT_SUBMIT);
    pixmapBtnDelete.setDevicePixelRatio(2.0);

    QPixmap pixmapBtnCancel(ImagePath::SELECT_CANCEL);
    pixmapBtnCancel.setDevicePixelRatio(2.0);

    if(groupButton->getSelectFlag() == false){
        groupButton->setReserve(pixmapBtnDelete);
        groupButton->setSelectFlag(true);
        userDelCount++;
    }
    else{
        groupButton->setReserve(pixmapBtnCancel);
        groupButton->setSelectFlag(false);
        userDelCount--;
    }

    if(userDelCount > 0){
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

void DClusterDeviceDelUserPage::on_btnCancel_clicked()
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
            userDelCount--;
        }
    }

    QString strLeftBtn = tr("返回");
    QPixmap leftArrowsPixmap(ImagePath::NAVIGATION_LEFT_ARROWS);
    navigationBar->setLeftText(strLeftBtn);
    navigationBar->setLeftPixMap(leftArrowsPixmap);
    disconnect(navigationBar, SIGNAL(leftClicked()), this, SLOT(on_btnCancel_clicked()));
    connect(navigationBar, SIGNAL(leftClicked()), this, SLOT(on_btnPrePage_clicked()));
}

void DClusterDeviceDelUserPage::on_hintMsgPage(bool isJump)
{
    if(!isJump)
        hintMsgPage->hide();
    else{
        this->hide();
        this->deleteLater();
    }
}

void DClusterDeviceDelUserPage::on_btnPrePage_clicked()
{
    this->hide();
    this->deleteLater();
}

DClusterDeviceDelUserPage::~DClusterDeviceDelUserPage()
{
    SAFE_DELETE(userList);
}
