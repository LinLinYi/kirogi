#include <QHBoxLayout>
#include <QVBoxLayout>

#include "DClusterInviteUserPage.h"
#include "UserManageMentPage.h"
#include "UserProtocol.hpp"
#include "NavigationBar.h"
#include "ImageConfig.hpp"
#include "UserInfo.h"
#include "InputPacket.h"
#include "HintMsgPage.h"
#include "AssemblyData.h"
#include "GroupButton.h"
#include "LoadingPage.h"
#include "DClusterUserInfo.h"
#include "SheetStyleConfig.hpp"

DClusterInviteUserPage::DClusterInviteUserPage(QWidget* parent, quint64 clusterID)
    :BasePage(parent)
    , curClusterID(clusterID)
    , isExistUserInfo(false)
    , curUserInfo(NULL)
    , loadingPage(NULL)
{
    this->initializeWidget();
}

void DClusterInviteUserPage::initializeWidget()
{
    //提示信息页面操作
    hintMsgPage = new HintMsgPage(this);
    hintMsgPage->hide();
    connect(hintMsgPage, SIGNAL(submitClicked(bool)), this, SLOT(on_hintMsgPage(bool)));

    //导航栏
    QString strTitle = tr("成员资料");
    QString strLeftBtn = tr("返回");
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

    //个人标题信息
    QPixmap pixmap(ImagePath::USER_PORTRAIT);
    btnTitleInfo = new GroupButton(this);
    btnTitleInfo->setPixMap(pixmap);
    btnTitleInfo->setFixedHeight(this->screenHeight()*0.12);
    btnTitleInfo->setFixedWidth(this->screenWidth());
    btnTitleInfo->setAttribute(Qt::WA_LayoutUsesWidgetRect);

    btnSignatrue = new GroupButton(this);
    btnSignatrue->setAttribute(Qt::WA_LayoutUsesWidgetRect);
    btnSignatrue->setStyleSheet(SheetStyle::GROUPBUTTON_NOBORDER);
    btnSignatrue->setText(tr("设备描述"));
    btnSignatrue->setFixedHeight(this->screenHeight()*0.08);
    btnSignatrue->setContentsMargins(this->screenWidth()*0.05,0,this->screenWidth()*0.05, 0);

    //邀请成员按钮
    btnInviteMember = new QPushButton(this);
    btnInviteMember->setFixedHeight(this->bottomBarHeight());
    btnInviteMember->setStyleSheet("background-color:white; border-radius:3px; font:15px; color:#0090ff;");
    connect(btnInviteMember, SIGNAL(clicked()), this, SLOT(on_btnInviteMember_clicked()));
    
    QVBoxLayout* vbInviteMember = new QVBoxLayout;
    vbInviteMember->addWidget(btnInviteMember);
    vbInviteMember->setSpacing(0);
    vbInviteMember->setMargin(0);
    this->setBottombarLayout(vbInviteMember);

    QVBoxLayout* vbTotalLayout = new QVBoxLayout;
    vbTotalLayout->addWidget(btnTitleInfo);
    vbTotalLayout->addWidget(btnSignatrue);
    vbTotalLayout->setMargin(0);
    vbTotalLayout->setSpacing(0);
    vbTotalLayout->addStretch(0);
    this->setBodyPartLayout(vbTotalLayout);

    //屏幕触摸滚动设置
    this->setBodyScreenHeight(this->scrollAreaHasBottomBarHeight());
    this->installScrollViewportArea();
}

void DClusterInviteUserPage::fetchData(InputPacket &inpack)
{
    quint16 cmdCode = inpack.getCmdCode();
    switch(cmdCode)
    {
        case CommandCode::NET_ERROR:
            this->handleNetError();
            break;
        case CommandCode::DCLUSTER_SEARCH_USER:
            this->handleSearchUserinfo(inpack);
            break;
        case CommandCode::DCLUSTER_INVITE_REQ:
            this->handleInviteUserInfo(inpack);
            break;
    }
}

void DClusterInviteUserPage::handleNetError()
{
    if(loadingPage == NULL) return;
    QString lblText = tr("网络错误, 请重试");
    this->handleHintInfo(lblText);
}

void DClusterInviteUserPage::handleInviteUserInfo(InputPacket &inpack)
{
    quint8 ansCode = inpack.getAnswerCode();
    if(ansCode == Mi::Success){
        QString strText = tr("邀请发送成功");
        hintMsgPage->setStatus(true);
        this->handleHintInfo(strText);
    }
    else if(Mi::Failure){
        QString strText = tr("邀请发送失败, 请重试");
        this->handleHintInfo(strText);
    }
}

void DClusterInviteUserPage::handleSearchUserinfo(InputPacket &inpack)
{
    char* data = inpack.getPackData();
    SAFE_DELETE(curUserInfo);
    curUserInfo = new UserInfo;
    curUserInfo->unpack(data);
    curUserID = curUserInfo->getUserID();

    isExistUserInfo = DClusterUserInfo::isExistUserInfo(curClusterID, curUserID);
    if(isExistUserInfo){
        btnInviteMember->setText(QString(tr("返回主页")));
    }else
        btnInviteMember->setText(tr("邀请加入"));

    QString userName = curUserInfo->getUserName();
    QString userRemarks = curUserInfo->getRemarks();

    if(userRemarks.length() > 0){
        userName = QString(tr("账号：")) + userName;
        btnTitleInfo->setText(userRemarks);
        btnTitleInfo->setRemarks(userName);
    }else{
        btnTitleInfo->setRemarks(userName);
    }

    btnSignatrue->setText(QString(tr("签名")));
    btnSignatrue->setDescribe(curUserInfo->getDescribe());
}

void DClusterInviteUserPage::on_btnInviteMember_clicked()
{
    if(isExistUserInfo){
        emit readyRead(CommandCode::BACK_DEVICE_PAGE, NULL, 0);
    }else{
        AssemblyData assemblyData;
        assemblyData.append(curClusterID);
        assemblyData.append(curUserID);

        char* data = assemblyData.getAssemblyData();
        quint16 dataLen = assemblyData.getAssemblyDataLen();
        emit readyRead(CommandCode::DCLUSTER_INVITE_REQ, data, dataLen);

        if(loadingPage == NULL){
            loadingPage = new LoadingPage(this);
            loadingPage->show();
        }
    }
}

void DClusterInviteUserPage::on_hintMsgPage(bool isJump)
{
    hintMsgPage->hide();
    if(isJump){
        emit readyRead(CommandCode::BACK_CLUSTER_MANAGE_PAGE, NULL, 0);
    }
}

void DClusterInviteUserPage::handleHintInfo(QString& text)
{
    hintMsgPage->setText(text);
    hintMsgPage->show();
    SAFE_DELETE(loadingPage);
}

void DClusterInviteUserPage::on_btnPrePage_clicked()
{
    this->hide();
    this->deleteLater();
}

DClusterInviteUserPage::~DClusterInviteUserPage()
{
    SAFE_DELETE(curUserInfo);
}
