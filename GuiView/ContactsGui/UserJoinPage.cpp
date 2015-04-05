#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>

#include "UserInfo.h"
#include "InputPacket.h"
#include "HintMsgPage.h"
#include "LoadingPage.h"
#include "GroupButton.h"
#include "UserJoinPage.h"
#include "UserProtocol.hpp"
#include "NavigationBar.h"
#include "ImageConfig.hpp"
#include "AssemblyData.h"
#include "SheetStyleConfig.hpp"

UserJoinPage::UserJoinPage(QWidget* parent)
    : BasePage(parent)
    , curUserID(0)
    , curUserInfo(NULL)
    , btnTitleInfo(NULL)
    , btnSignatrue(NULL)
    , btnAddBuddy(NULL)
    , loadingPage(NULL)
{
    this->initializeWidget();
}

void UserJoinPage::initializeWidget()
{
    //提示信息页面操作;
    hintMsgPage = new HintMsgPage(this);
    hintMsgPage->hide();
    connect(hintMsgPage, SIGNAL(submitClicked(bool)), this, SLOT(on_hintMsgPage(bool)));

    //导航栏
    QString strTitle = tr("个人资料");
    QString strLeftBtn = tr("添加");
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
    btnSignatrue->setText(tr("签名"));
    btnSignatrue->setFixedHeight(this->screenHeight()*0.08);

    QVBoxLayout* vbBodyLayout = new QVBoxLayout;
    vbBodyLayout->addWidget(btnTitleInfo);
    vbBodyLayout->addWidget(btnSignatrue);
    vbBodyLayout->setMargin(0);
    vbBodyLayout->setSpacing(0);
    vbBodyLayout->addStretch(0);
    this->setBodyPartLayout(vbBodyLayout);

    //添加好友按钮
    btnAddBuddy = new QPushButton(this);
    btnAddBuddy->setText("加为好友");
    btnAddBuddy->setFixedHeight(this->bottomBarHeight());
    btnAddBuddy->setStyleSheet("background-color:white; border-radius:3px; font:15px; color:#0090ff;");
    connect(btnAddBuddy, SIGNAL(clicked()), this, SLOT(on_btnAddBuddy_clicked()));
    
    QVBoxLayout* vbAddBuddy = new QVBoxLayout;
    vbAddBuddy->addWidget(btnAddBuddy);
    vbAddBuddy->setMargin(0);
    this->setBottombarLayout(vbAddBuddy);

    //屏幕触摸滚动设置
    this->setBodyScreenHeight(this->scrollAreaHasBottomBarHeight());
    this->installScrollViewportArea();
}

void UserJoinPage::fetchData(InputPacket& inpack)
{
    quint16 cmdCode = inpack.getCmdCode();
    switch(cmdCode)
    {
        case CommandCode::NET_ERROR:
            this->handleNetError();
            break;
        case CommandCode::USER_JOIN_REQ:
            this->recvUserJoinApprovalResult(inpack);
            break;
        case CommandCode::SEARCH_USER:
            this->handleSearchedUserInfo(inpack);
            break;
    }
}

void UserJoinPage::handleNetError()
{
    if(loadingPage == NULL) return;
    QString lblText = tr("网络错误,请重试");
    this->handleHintInfo(lblText);
}

void UserJoinPage::handleHintInfo(QString& text)
{
    hintMsgPage->setText(text);
    hintMsgPage->show();
    SAFE_DELETE(loadingPage);
}

void UserJoinPage::recvUserJoinApprovalResult(InputPacket &inpack)
{
    quint8 ansCode = inpack.getAnswerCode();
    if(ansCode == Mi::Failure){
        QString strText = tr("发送请求失败,请稍后再试");
        this->handleHintInfo(strText);
    }
    else if(ansCode == Mi::Success){
        QString strText = tr("请求发送成功");
        hintMsgPage->setStatus(true);
        this->handleHintInfo(strText);
    }
}

void UserJoinPage::handleSearchedUserInfo(InputPacket &inpack)
{
    char* data = inpack.getPackData();
    if(data == NULL) return;
    
    SAFE_DELETE(curUserInfo);
    curUserInfo = new UserInfo;
    curUserInfo->unpack(data);
    this->setPageData(curUserInfo);
}

void UserJoinPage::setPageData(UserInfo *info)
{
    QString userName = info->getUserName();
    QString userRemarks = info->getRemarks();
    QString userSignatrue = info->getDescribe();
    curUserID = info->getUserID();

    if(userRemarks.length() > 0){
        btnTitleInfo->setText(userRemarks);
        btnTitleInfo->setRemarks(QString(tr("账号：") + userName));
    }else{
        btnTitleInfo->setText(userName);
    }

    btnSignatrue->setText(QString(tr("签名")));
    btnSignatrue->setDescribe(userSignatrue);
}

void UserJoinPage::on_btnPrePage_clicked()
{
    this->hide();
    this->deleteLater();
}

void UserJoinPage::on_btnAddBuddy_clicked()
{
    AssemblyData assemblyData;
    assemblyData.append(curUserID);
    char* data = assemblyData.getAssemblyData();
    quint16 dataLen = assemblyData.getAssemblyDataLen();
    emit readyRead(CommandCode::USER_JOIN_REQ, data, dataLen);

    if(loadingPage == NULL){
        loadingPage = new LoadingPage(this);
        loadingPage->show();
    }
}

void UserJoinPage::on_hintMsgPage(bool isJump)
{
    if(!isJump){
        hintMsgPage->hide();
        return;
    }
    emit readyRead(CommandCode::BACK_CONTACT_PAGE, NULL, 0);
}

UserJoinPage::~UserJoinPage()
{
    SAFE_DELETE(curUserInfo);
}
