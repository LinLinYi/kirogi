#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QValidator>

#include "MD5.h"
#include "LoadingPage.h"
#include "GlobalDefine.hpp"
#include "InputPacket.h"
#include "HintMsgPage.h"
#include "NavigationBar.h"
#include "UserProtocol.hpp"
#include "ImageConfig.hpp"
#include "SheetStyleConfig.hpp"
#include "ForgetPwdResetPage.h"

ForgetPwdResetPage::ForgetPwdResetPage(QWidget* parent)
    : BasePage(parent)
    , loadingPage(NULL)
{
    this->initializeWidget();
}

void ForgetPwdResetPage::initializeWidget()
{
    //提示信息页面操作
    hintMsgPage = new HintMsgPage(this);
    hintMsgPage->hide();
    connect(hintMsgPage, SIGNAL(submitClicked(bool)), this, SLOT(on_hintMsgPage(bool)));

    //导航栏
    QString strTitle = tr("重设密码");
    QString strLeftBtn = tr("取消");
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

    //验证部分
    editPassword = new QLineEdit(this);
    editPassword->setFixedHeight(this->screenHeight()*0.077);
    editPassword->setPlaceholderText(tr("密码(6~20个字符)"));
    editPassword->setStyleSheet(SheetStyle::QLINEEDIT);
    this->setInputMode(editPassword);
    editPassword->setEchoMode(QLineEdit::Password);
    editPassword->setTextMargins(this->screenWidth()*0.02, 0, this->screenWidth()*0.02, 0);

    editSubmitPassword = new QLineEdit(this);
    editSubmitPassword->setFixedHeight(this->screenHeight()*0.077);
    editSubmitPassword->setPlaceholderText(tr("确认密码(6~20个字符)"));
    editSubmitPassword->setStyleSheet(SheetStyle::QLINEEDIT);
    this->setInputMode(editSubmitPassword);
    editSubmitPassword->setEchoMode(QLineEdit::Password);
    editSubmitPassword->setTextMargins(this->screenWidth()*0.02, 0, this->screenWidth()*0.02, 0);

    btnSubmit = new QPushButton(tr("提 交"), this);
    btnSubmit->setFixedHeight(this->screenHeight()*0.074);
    btnSubmit->setStyleSheet(SheetStyle::QPUSHBUTTON);
    connect(btnSubmit, SIGNAL(clicked()), this, SLOT(on_btnSubmit_clicked()));

    QVBoxLayout* vbLayout = new QVBoxLayout;
    vbLayout->addWidget(editPassword);
    vbLayout->addSpacing(this->screenHeight()*0.01);
    vbLayout->addWidget(editSubmitPassword);
    vbLayout->addSpacing(this->screenHeight()*0.03);
    vbLayout->addWidget(btnSubmit);
    vbLayout->setAlignment(Qt::AlignTop);
    vbLayout->addStretch(0);
    vbLayout->setSpacing(0);
    vbLayout->setContentsMargins(this->screenWidth()*0.054,this->screenHeight()*0.026,this->screenWidth()*0.054,0);
    this->setBodyPartLayout(vbLayout);
}

void ForgetPwdResetPage::fetchData(InputPacket &inpack)
{
    quint16 cmdCode = inpack.getCmdCode();
    switch(cmdCode)
    {
        case CommandCode::FORGET_PASSWORD_RESET:
            handleForgetPwdReset(inpack);
            break;
        case CommandCode::NET_ERROR:
            handleNetError();
            break;
    }
}

void ForgetPwdResetPage::handleNetError()
{
    if(loadingPage == NULL)
        return;

    QString lblText = tr("网络连接错误,请重试");
    handleHintInfo(lblText);
}

void ForgetPwdResetPage::handleForgetPwdReset(InputPacket &inpack)
{
    quint8 answerCode = inpack.getAnswerCode();
    if(answerCode == Mi::Success){
        QString strText = tr("密码修改成功");
        hintMsgPage->setStatus(true);
        this->handleHintInfo(strText);
    }
    else if(answerCode == Mi::Failure){
        QString strText = tr("修改密码失败,请重试");
        this->handleHintInfo(strText);
    }
}

void ForgetPwdResetPage::handleHintInfo(QString &strText)
{
    hintMsgPage->setText(strText);
    hintMsgPage->show();
    SAFE_DELETE(loadingPage);
}

void ForgetPwdResetPage::setInputMode(QLineEdit* line)
{
    QRegExp regx("[a-zA-Z0-9]+$");
    validator = new QRegExpValidator(regx, line);
    line->setValidator(validator);
}

void ForgetPwdResetPage::on_btnPrePage_clicked()
{
    this->hide();
    this->deleteLater();
}

void ForgetPwdResetPage::on_btnSubmit_clicked()
{
    QString strPwd = editPassword->text().trimmed();
    if(strPwd.length() < 6 || strPwd.length() > 20){
        QString strText = tr("密码位数为6~20位");
        handleHintInfo(strText);
        return;
    }

    QString strSurePwd = editSubmitPassword->text().trimmed();
    if(strPwd != strSurePwd){
        QString strText = tr("两次密码不一致,请重新输入");
        handleHintInfo(strText);
        return;
    }

    QByteArray bytePwd = strPwd.toLatin1();
    char* chPwd = bytePwd.data();
    char msgPackBuf[ConstNum::MAX_PACKET_SIZE] = {0};

    MD5 md5Pwd;
    md5Pwd.calc(chPwd, strPwd.length(), msgPackBuf);
    emit readyRead(CommandCode::FORGET_PASSWORD_RESET, msgPackBuf, 16);

    if(loadingPage == NULL){
        loadingPage = new LoadingPage(this);
        loadingPage->show();
    }
}

void ForgetPwdResetPage::on_hintMsgPage(bool isJump)
{
    hintMsgPage->hide();
    if(!isJump) return;

    emit readyRead(CommandCode::BACK_LOGIN_PAGE, NULL, 0);
}

ForgetPwdResetPage::~ForgetPwdResetPage()
{

}
