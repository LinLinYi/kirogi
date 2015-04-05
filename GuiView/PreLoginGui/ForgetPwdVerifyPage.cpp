#include <QHBoxLayout>
#include <QLabel>

#include "AssemblyData.h"
#include "GlobalDefine.hpp"
#include "HintMsgPage.h"
#include "NavigationBar.h"
#include "SheetStyleConfig.hpp"
#include "UserProtocol.hpp"
#include "ImageConfig.hpp"
#include "LoadingPage.h"
#include "ForgetPwdVerifyPage.h"

ForgetPwdVerifyPage::ForgetPwdVerifyPage(QWidget* parent)
    : BasePage(parent)
    , loadingPage(NULL)
    , forgetPwdResetPage(NULL)
{
    this->initializeWidget();
}

void ForgetPwdVerifyPage::initializeWidget()
{
    //提示信息页面操作
    hintMsgPage = new HintMsgPage(this);
    hintMsgPage->hide();
    connect(hintMsgPage, SIGNAL(submitClicked(bool)), this, SLOT(on_hintMsgPage(bool)));

    //导航栏
    QString strTitle = tr("验 证");
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
    QLabel* lblMsgHint = new QLabel(this);
    lblMsgHint->setText(tr("已发送验证码到您的邮箱"));
    lblMsgHint->setFixedHeight(this->screenHeight()*0.05);
    lblMsgHint->setAlignment(Qt::AlignLeft);
    lblMsgHint->setContentsMargins(this->screenWidth()*0.02, 0, this->screenWidth()*0.02, 0);
    lblMsgHint->setStyleSheet("color:rgb(122,123,128); font:16px;");

    editValidate = new QLineEdit(this);
    editValidate->setFixedHeight(this->screenHeight()*0.08);
    editValidate->setPlaceholderText(tr("输入6位验证码"));
    editValidate->setStyleSheet(SheetStyle::FORGETPWD_VALIDATION_LINEEDIT);
    editValidate->setTextMargins(this->screenWidth()*0.02, 0, this->screenWidth()*0.02, 0);

    QVBoxLayout* vbInfoLayout = new QVBoxLayout;
    vbInfoLayout->addWidget(lblMsgHint);
    vbInfoLayout->addWidget(editValidate);
    vbInfoLayout->setAlignment(Qt::AlignCenter);
    vbInfoLayout->setSpacing(0);
    vbInfoLayout->setMargin(0);

    btnNextStep = new QPushButton(tr("下一步"), this);
    btnNextStep->setFixedHeight(this->screenHeight()*0.074);
    btnNextStep->setStyleSheet(SheetStyle::QPUSHBUTTON);
    connect(btnNextStep, SIGNAL(clicked()), this, SLOT(on_btnNextStep_clicked()));

    QVBoxLayout* vbLayout = new QVBoxLayout;
    vbLayout->addLayout(vbInfoLayout);
    vbLayout->addSpacing(this->screenHeight()*0.03);
    vbLayout->addWidget(btnNextStep);
    vbLayout->setAlignment(Qt::AlignTop);
    vbLayout->addStretch(0);
    vbLayout->addSpacing(0);
    vbLayout->setContentsMargins(this->screenWidth()*0.054,this->screenHeight()*0.026,this->screenWidth()*0.054,0);
    this->setBodyPartLayout(vbLayout);
}

void ForgetPwdVerifyPage::fetchData(InputPacket &inpack)
{
    quint16 cmdCode = inpack.getCmdCode();
    switch(cmdCode)
    {
    case CommandCode::FORGET_PASSWORD_VERIFY:
        this->handleForgetPwdVerify(inpack);
        break;
    case CommandCode::NET_ERROR:
        this->handleNetError();
        break;
    }
    transferDataToAnotherPage(inpack, forgetPwdResetPage);
}

void ForgetPwdVerifyPage::handleNetError()
{
    if(loadingPage == NULL) return;
    QString lblText = tr("网络连接错误,请重试");
    this->handleHintInfo(lblText);
}

void ForgetPwdVerifyPage::handleForgetPwdVerify(InputPacket &inpack)
{
    quint8 answerCode = inpack.getAnswerCode();
    if(answerCode == Mi::Success){
       SAFE_DELETE(loadingPage);
        forgetPwdResetPage = new ForgetPwdResetPage(this);
        this->displayConcretePage(forgetPwdResetPage);
    }else if(answerCode == Mi::Error){
       QString strText = tr("验证码错误");
       this->handleHintInfo(strText);
    }
}

void ForgetPwdVerifyPage::displayConcretePage(BasePage *page)
{
    if(page == NULL) return;
    page->show();
    connect(page, SIGNAL(destroyed()), this, SLOT(on_deleteSubpage()));
    connect(page, SIGNAL(readyRead(quint16,char*,quint16)), this, SLOT(on_recvDataFromOtherPage(quint16,char*,quint16)));
}

void ForgetPwdVerifyPage::transferDataToAnotherPage(InputPacket &inpack, BasePage *basePage)
{
    if(basePage == NULL) return;
    basePage->fetchData(inpack);
}

void ForgetPwdVerifyPage::handleHintInfo(QString &strText)
{
    hintMsgPage->setText(strText);
    hintMsgPage->show();
    SAFE_DELETE(loadingPage);
}

void ForgetPwdVerifyPage::on_btnNextStep_clicked()
{
    if(editValidate->text().length() != 6){
        QString strText = tr("请输入6位验证码");
        hintMsgPage->setText(strText);
        hintMsgPage->show();

        editValidate->setFocus();
        return;
    }

    QString strValidationCode = editValidate->text().trimmed();
    QByteArray byteValidationCode = strValidationCode.toLatin1();
    verifyCode = byteValidationCode.toUInt();

    AssemblyData assemblyData;
    assemblyData.append(verifyCode);
    char* data = assemblyData.getAssemblyData();
    quint16 dataLen = assemblyData.getAssemblyDataLen();
    emit readyRead(CommandCode::FORGET_PASSWORD_VERIFY, data, dataLen);

    if(loadingPage == NULL){
        loadingPage = new LoadingPage(this);
        loadingPage->show();
    }
}

void ForgetPwdVerifyPage::on_hintMsgPage(bool isJump)
{
    if(!isJump)
        hintMsgPage->hide();
}

void ForgetPwdVerifyPage::on_btnPrePage_clicked()
{
    this->hide();
    this->deleteLater();
}

void ForgetPwdVerifyPage::on_deleteSubpage()
{
    forgetPwdResetPage = NULL;
}

void ForgetPwdVerifyPage::on_recvDataFromOtherPage(quint16 cmdCode, char *dataField, quint16 dataLen)
{
    emit readyRead(cmdCode, dataField, dataLen);
}


ForgetPwdVerifyPage::~ForgetPwdVerifyPage()
{

}
