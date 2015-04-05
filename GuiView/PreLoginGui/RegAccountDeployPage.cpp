#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QValidator>

#include "Keys.h"
#include "AssemblyData.h"
#include "InputPacket.h"
#include "GlobalDefine.hpp"
#include "UserProtocol.hpp"
#include "HintMsgPage.h"
#include "LoadingPage.h"
#include "NavigationBar.h"
#include "ImageConfig.hpp"
#include "SheetStyleConfig.hpp"
#include "RegAccountDeployPage.h"

RegAccountDeployPage::RegAccountDeployPage(QWidget* parent)
    : BasePage(parent)
    , loadingPage(NULL)
{
    initializeWidget();
}

void RegAccountDeployPage::initializeWidget()
{
    //提示信息页面操作
    hintMsgPage = new HintMsgPage(this);
    hintMsgPage->hide();
    connect(hintMsgPage, SIGNAL(submitClicked(bool)), this, SLOT(on_hintMsgPage(bool)));

    //导航栏
    QString strTitle = tr("设 置");
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

    //账号设置
    editAccount = new QLineEdit(this);
    editAccount->setFixedHeight(this->screenHeight()*0.077);
    editAccount->setPlaceholderText(tr("用户名(2~20个字符)"));
    editAccount->setStyleSheet(SheetStyle::QLINEEDIT);
    this->setInputMode(editAccount);
    editAccount->setTextMargins(this->screenWidth()*0.02, 0, this->screenWidth()*0.02, 0);

    editPassword = new QLineEdit(this);
    editPassword->setFixedHeight(this->screenHeight()*0.077);
    editPassword->setPlaceholderText(tr("密码(6~20个字符)"));
    editPassword->setStyleSheet(SheetStyle::QLINEEDIT);
    this->setInputMode(editPassword);
    editPassword->setEchoMode(QLineEdit::Password);
    editPassword->setTextMargins(this->screenWidth()*0.02, 0, this->screenWidth()*0.02, 0);

    btnSubmit = new QPushButton(tr("提  交"), this);
    btnSubmit->setFixedHeight(this->screenHeight()*0.074);
    btnSubmit->setStyleSheet(SheetStyle::QPUSHBUTTON);
    connect(btnSubmit, SIGNAL(clicked()), this, SLOT(on_btnSubmit_clicked()));

    QVBoxLayout* vbLayout = new QVBoxLayout;
    vbLayout->addWidget(editAccount);
    vbLayout->addSpacing(this->screenHeight()*0.015);
    vbLayout->addWidget(editPassword);
    vbLayout->addSpacing(this->screenHeight()*0.03);
    vbLayout->addWidget(btnSubmit);
    vbLayout->setAlignment(Qt::AlignTop);
    vbLayout->addStretch(0);
    vbLayout->setSpacing(0);
    vbLayout->setContentsMargins(this->screenWidth()*0.054,this->screenHeight()*0.026,this->screenWidth()*0.054,0);
    this->setBodyPartLayout(vbLayout);

    //屏幕触摸滚动设置
    this->installScrollViewportArea();
}

void RegAccountDeployPage::fetchData(InputPacket &inpack)
{
    quint16 cmdCode = inpack.getCmdCode();
    switch(cmdCode)
    {
        case CommandCode::REGISTER_THIRD_STEP:
            this->handleRegisterThirdStep(inpack);
            break;
        case CommandCode::NET_ERROR:
            this->handleNetError();
            break;
    }
}

void RegAccountDeployPage::handleNetError()
{
    if(loadingPage == NULL) return;
    QString strText = tr("网络错误,请重试!");
    handleHintInfo(strText);
}

void RegAccountDeployPage::handleRegisterThirdStep(InputPacket &inpack)
{
    quint8 answerCode = inpack.getAnswerCode();
    QString strText;
    if(answerCode == Mi::Success){
        strText = tr("账户设置成功");
        hintMsgPage->setStatus(true);
    }
    else if(answerCode == Mi::Used)
        strText = tr("账号已存在,请重新输入");
    else if(answerCode == Mi::Failure)
        strText = tr("账户设置失败,请重试");

    this->handleHintInfo(strText);
}

void RegAccountDeployPage::handleHintInfo(QString &strText)
{
    hintMsgPage->setText(strText);
    hintMsgPage->show();
    SAFE_DELETE(loadingPage);
}

void RegAccountDeployPage::setInputMode(QLineEdit* line)
{
    QRegExp regx("[a-zA-Z0-9]+$");
    validator = new QRegExpValidator(regx, line);
    line->setValidator(validator);
}

void RegAccountDeployPage::on_btnSubmit_clicked()
{
    QString strAccount = editAccount->text().trimmed();
    if(strAccount.length() < 2 || strAccount.length() > 20){
        QString str = tr("用户名(2~20位)");
        this->handleHintInfo(str);
        return;
    }

    QString strPwd = editPassword->text().trimmed();
    if(strPwd.length() < 6 || strPwd.length() > 20){
        QString str = tr("密码(6~20位)");
        this->handleHintInfo(str);
        return;
    }

    QByteArray bytePwd = strPwd.toLatin1();
    char* chPwd = bytePwd.data();

    MD5 md5Pwd;
    char pwdOut[Keys::MD5KeyLen];
    md5Pwd.calc(chPwd, strPwd.length(), pwdOut);

    AssemblyData assemblyData;
    assemblyData.append(strAccount);
    assemblyData.append(pwdOut, Keys::MD5KeyLen);
    char* data = assemblyData.getAssemblyData();
    quint16 dataLen = assemblyData.getAssemblyDataLen();
    emit readyRead(CommandCode::REGISTER_THIRD_STEP, data, dataLen);

    //加载页面
    if(loadingPage == NULL){
        loadingPage = new LoadingPage(this);
        loadingPage->show();
    }
}

void RegAccountDeployPage::on_hintMsgPage(bool isJump)
{
    if(!isJump){
        hintMsgPage->hide();
        return;
    }

    hintMsgPage->hide();
    emit readyRead(CommandCode::BACK_LOGIN_PAGE, NULL, 0);
}

void RegAccountDeployPage::on_btnPrePage_clicked()
{
    this->hide();
    this->deleteLater();
}

RegAccountDeployPage::~RegAccountDeployPage(){}

