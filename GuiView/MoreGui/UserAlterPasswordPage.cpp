#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QValidator>

#include "MD5.h"
#include "ListWidget.h"
#include "UserInfo.h"
#include "UserProtocol.hpp"
#include "HintMsgPage.h"
#include "ImageConfig.hpp"
#include "NavigationBar.h"
#include "InputPacket.h"
#include "LoadingPage.h"
#include "SheetStyleConfig.hpp"
#include "UserAlterPasswordPage.h"

UserAlterPasswordPage::UserAlterPasswordPage(QWidget* parent, quint64 accountID)
    :BasePage(parent)
    , curAccountID(accountID)
    , loadingPage(NULL)
{
    userInfo = new UserInfo;
    userInfo->loadDataSource(curAccountID);
    this->initializeWidget();
}

void UserAlterPasswordPage::initializeWidget()
{
    //提示信息页面操作
    hintMsgPage = new HintMsgPage(this);
    hintMsgPage->hide();
    connect(hintMsgPage, SIGNAL(submitClicked(bool)), this, SLOT(on_hintMsgPage(bool)));

    //导航栏
    QString strTitle = tr("设置密码");
    QString strLeftBtn = tr("取消");
    QString strRightBtn = tr("完成");
    navigationBar = new NavigationBar(this);
    navigationBar->setTitleText(strTitle);
    navigationBar->setLeftText(strLeftBtn);
    navigationBar->setRightText(strRightBtn);
    connect(navigationBar, SIGNAL(leftClicked()), this, SLOT(on_btnPrePage_clicked()));
    connect(navigationBar, SIGNAL(rightClicked()), this, SLOT(on_btnSubmit_clicked()));

    QHBoxLayout* pHLTop = new QHBoxLayout();
    pHLTop->addWidget(navigationBar);
    pHLTop->setSpacing(0);
    pHLTop->setMargin(0);
    this->setTopbarLayout(pHLTop);

    //信息提示标签
    QLabel* lblHintInfo = new QLabel(this);
    lblHintInfo->adjustSize();
    lblHintInfo->setWordWrap(true);
    lblHintInfo->setText(tr("设置账号密码后, 你可以用账号+修改后的密码登录"));
    lblHintInfo->setMargin(this->screenWidth()*0.05);
    lblHintInfo->setStyleSheet("color:#8c8c8c; font: 14px;");

    //账号标签
    QString strStyleMacNum = "border-style:solid; border-width: 0px;\
    border-top-width:1px;border-color:rgb(208, 208, 208);\
    background-color:white;font:bold 15px;color:#8c8c8c;";
    
    QLabel* lblMacNum =  new QLabel(this);
    lblMacNum->setText(QObject::tr("账号"));
    lblMacNum->setAlignment(Qt::AlignCenter);
    lblMacNum->setFixedHeight(this->screenHeight()*0.077);
    lblMacNum->setMinimumWidth(this->screenHeight()*0.07);
    lblMacNum->setStyleSheet(strStyleMacNum);

    //账号输入
    QString strStyleEditMacNum = "border-style:solid; border-width: 0px;\
    border-top-width:1px;border-color:rgb(208, 208, 208);\
    background-color:white;font:15px;color:#8c8c8c;";
    
    editMacNum = new QLineEdit(this);
    editMacNum->setMaxLength(20);
    editMacNum->setFixedHeight(this->screenHeight()*0.077);
    this->setInputMode(editMacNum);
    editMacNum->setEnabled(false);
    editMacNum->setPlaceholderText(userInfo->getUserName());
    editMacNum->setStyleSheet(strStyleEditMacNum);

    //密码标签
    QString strStyleMacPwd = "border-style:solid; border-width: 0px;\
    border-top-width:1px;border-color:rgb(208, 208, 208);\
    background-color:white;font:bold 15px;color:black;";
    
    QLabel* lblMacPwd = new QLabel(this);
    lblMacPwd->setText(QObject::tr("新密码"));
    lblMacPwd->setAlignment(Qt::AlignCenter);
    lblMacPwd->setFixedHeight(this->screenHeight()*0.077);
    lblMacPwd->setMinimumWidth(this->screenHeight()*0.07);
    lblMacPwd->setStyleSheet(strStyleMacPwd);

    //输入密码
    QString strStyleEditMacPwd = "border-style:solid; border-width: 0px;\
    border-top-width:1px;border-color:rgb(208, 208, 208);\
    background-color:white;font:15px;color:black;";
    
    editMacPwd = new QLineEdit(this);
    editMacPwd->setMaxLength(20);
    editMacPwd->setFixedHeight(this->screenHeight()*0.077);
    this->setInputMode(editMacPwd);
    editMacPwd->setEchoMode(QLineEdit::Password);
    editMacPwd->setPlaceholderText(QObject::tr("请输入新密码"));
    editMacPwd->setStyleSheet(strStyleEditMacPwd);

    //确认密码标签
    QString strStyleAffirmPwd = "border-style:solid; border-width: 0px;\
    border-top-width:1px;border-bottom-width:1px;border-color:rgb(208, 208, 208);\
    background-color:white;font:bold 15px;color:black;";
    
    QLabel* lblAffirmPwd = new QLabel(this);
    lblAffirmPwd->setText(QObject::tr("确认密码"));
    lblAffirmPwd->setAlignment(Qt::AlignCenter);
    lblAffirmPwd->setFixedHeight(this->screenHeight()*0.077);
    lblAffirmPwd->setMinimumWidth(this->screenHeight()*0.07);
    lblAffirmPwd->setStyleSheet(strStyleAffirmPwd);

    //确认密码
    QString strStyleEditAffirmPwd = "border-style:solid; border-width: 0px;\
    border-top-width:1px;border-bottom-width:1px;border-color:rgb(208, 208, 208);\
    background-color:white;font:15px;color:black;";
    
    editAffirmPwd = new QLineEdit(this);
    editAffirmPwd->setMaxLength(20);
    editAffirmPwd->setFixedHeight(this->screenHeight()*0.077);
    this->setInputMode(editAffirmPwd);
    editAffirmPwd->setEchoMode(QLineEdit::Password);
    editAffirmPwd->setPlaceholderText(tr("请确认新密码"));
    editAffirmPwd->setStyleSheet(strStyleEditAffirmPwd);

    QGridLayout* glModifyPwd = new QGridLayout();
    glModifyPwd->addWidget(lblMacNum, 0, 0, 1, 1);
    glModifyPwd->addWidget(editMacNum, 0, 1, 1, 3);
    glModifyPwd->addWidget(lblMacPwd, 1, 0, 1, 1);
    glModifyPwd->addWidget(editMacPwd, 1, 1, 1, 3);
    glModifyPwd->addWidget(lblAffirmPwd, 2, 0, 1, 1);
    glModifyPwd->addWidget(editAffirmPwd, 2, 1, 1, 3);
    glModifyPwd->setSpacing(0);
    glModifyPwd->setMargin(0);
    glModifyPwd->setAlignment(Qt::AlignTop);

    QVBoxLayout* vblTotalLayout = new QVBoxLayout;
    vblTotalLayout->addWidget(lblHintInfo);
    vblTotalLayout->addLayout(glModifyPwd);
    vblTotalLayout->setSpacing(0);
    vblTotalLayout->setMargin(0);
    vblTotalLayout->addStretch(0);
    this->setBodyPartLayout(vblTotalLayout);

    //屏幕触摸滚动设置
    this->installScrollViewportArea();
}

void UserAlterPasswordPage::fetchData(InputPacket &inpack)
{
    quint16 cmdCode = inpack.getCmdCode();
    switch(cmdCode)
    {
        case CommandCode::MODIFY_ACCOUNT_PWD:
            this->handleModifyInfo(inpack);
            break;
        case CommandCode::NET_ERROR:
            this->handleNetError();
            break;
    }
}

void UserAlterPasswordPage::handleNetError()
{
    if(loadingPage == NULL) return;
    QString strText = tr("网络连接错误,请重试");
    this->handleHintInfo(strText);
}

void UserAlterPasswordPage::handleHintInfo(QString &strText)
{
    hintMsgPage->setText(strText);
    hintMsgPage->show();
    SAFE_DELETE(loadingPage);
}

void UserAlterPasswordPage::handleModifyInfo(InputPacket &inpack)
{
    quint8 answerCode = inpack.getAnswerCode();
    if(answerCode == Mi::Success){
        QString lblText = tr("修改密码成功");
        hintMsgPage->setStatus(true);
        this->handleHintInfo(lblText);
    }
    else if(answerCode == Mi::Failure){
        QString lblText = tr("修改密码失败");
        this->handleHintInfo(lblText);
    }
}

void UserAlterPasswordPage::setInputMode(QLineEdit* line)
{
    QRegExp regx("[a-zA-Z0-9]+$");
    validator = new QRegExpValidator(regx, line);
    line->setValidator(validator);
}

void UserAlterPasswordPage::on_btnSubmit_clicked()
{
    QString strPwd = editMacPwd->text().trimmed();
    if(strPwd.length() < 6 || strPwd.length() > 20){
        QString strText = tr("密码位数为6~20位");
        this->handleHintInfo(strText);
        return;
    }

    QString strSurePwd = editAffirmPwd->text().trimmed();
    if(strPwd != strSurePwd){
        QString strText = tr("两次密码不一致,请重新输入");
        this->handleHintInfo(strText);
        return;
    }

    QByteArray bytePwd = strPwd.toLatin1();
    char* chPwd = bytePwd.data();
    char msgPackBuf[ConstNum::MAX_PACKET_SIZE] = {0};

    MD5 md5Pwd;
    md5Pwd.calc(chPwd, strPwd.length(), msgPackBuf);
    emit readyRead(CommandCode::MODIFY_ACCOUNT_PWD, msgPackBuf, 16);

    if(loadingPage == NULL){
        loadingPage = new LoadingPage(this);
        loadingPage->show();
    }
}

void UserAlterPasswordPage::on_hintMsgPage(bool isJump)
{
    hintMsgPage->hide();
    if(!isJump) return;
    emit readyRead(CommandCode::BACK_ACCOUNT_SAFE_PAGE, NULL, 0);
}

void UserAlterPasswordPage::on_btnPrePage_clicked()
{
    emit readyRead(CommandCode::BACK_ACCOUNT_SAFE_PAGE, NULL, 0);
}

UserAlterPasswordPage::~UserAlterPasswordPage()
{
    SAFE_DELETE(userInfo);
    SAFE_DELETE(validator);
}
