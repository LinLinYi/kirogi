#include <QVBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>

#include "AssemblyData.h"
#include "GlobalDefine.hpp"
#include "InputPacket.h"
#include "ImageConfig.hpp"
#include "SheetStyleConfig.hpp"
#include "UserProtocol.hpp"
#include "NavigationBar.h"
#include "HintMsgPage.h"
#include "LoadingPage.h"
#include "UserVerifyNewMailPage.h"

UserVerifyNewMailPage::UserVerifyNewMailPage(QWidget* parent, QString& newMail)
    : BasePage(parent)
    , loadingPage(NULL)
    , newMailAddr(newMail)
{
    this->initializeWidget();
}

void UserVerifyNewMailPage::initializeWidget()
{
    //提示信息页面操作
    hintMsgPage = new HintMsgPage(this);
    hintMsgPage->hide();
    connect(hintMsgPage, SIGNAL(submitClicked(bool)), this, SLOT(on_hintMsgPage(bool)));

    //导航栏
    QString strTitle = tr("邮箱验证");
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

    btnSubmit = new QPushButton(tr("提 交"), this);
    btnSubmit->setFixedHeight(this->screenHeight()*0.074);
    btnSubmit->setStyleSheet(SheetStyle::QPUSHBUTTON);
    connect(btnSubmit, SIGNAL(clicked()), this, SLOT(on_btnSubmit_clicked()));

    QVBoxLayout* vbLayout = new QVBoxLayout;
    vbLayout->addLayout(vbInfoLayout);
    vbLayout->addSpacing(this->screenHeight()*0.03);
    vbLayout->addWidget(btnSubmit);
    vbLayout->setAlignment(Qt::AlignTop);
    vbLayout->addStretch(0);
    vbLayout->addSpacing(0);
    vbLayout->setContentsMargins(this->screenWidth()*0.054,this->screenHeight()*0.026,this->screenWidth()*0.054,0);
    this->setBodyPartLayout(vbLayout);
}

void UserVerifyNewMailPage::fetchData(InputPacket &inpack)
{
    quint16 cmdCode = inpack.getCmdCode();
    switch(cmdCode)
    {
    case CommandCode::MODIFY_MAIL_VERIFYCODE:
        handleUserNewMailVerify(inpack);
        break;
    case CommandCode::NET_ERROR:
        handleNetError();
        break;
    }
}

void UserVerifyNewMailPage::handleNetError()
{
    if(loadingPage == NULL) return;
    QString lblText = tr("网络连接错误,请重试");
    handleHintInfo(lblText);
}

void UserVerifyNewMailPage::handleUserNewMailVerify(InputPacket &inpack)
{
    quint8 answerCode = inpack.getAnswerCode();
    QString strText;

    if(answerCode == Mi::Success){
       SAFE_DELETE(loadingPage);
       strText = tr("邮箱修改成功");
       hintMsgPage->setStatus(true);
    }else if(answerCode == Mi::Error){
       strText = tr("验证码错误");
    }else{
       strText = tr("邮箱修改失败");
    }
    handleHintInfo(strText);
}

void UserVerifyNewMailPage::transferDataToAnotherPage(InputPacket &inpack, BasePage *basePage)
{
    if(basePage == NULL) return;
    basePage->fetchData(inpack);
}

void UserVerifyNewMailPage::handleHintInfo(QString &strText)
{
    hintMsgPage->setText(strText);
    hintMsgPage->show();
    SAFE_DELETE(loadingPage);
}

void UserVerifyNewMailPage::on_btnSubmit_clicked()
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
    quint32 verifyCode = byteValidationCode.toUInt();

    AssemblyData assemblyData;
    assemblyData.append(verifyCode);
    assemblyData.append(newMailAddr);
    char* data = assemblyData.getAssemblyData();
    quint16 dataLen = assemblyData.getAssemblyDataLen();
    emit readyRead(CommandCode::MODIFY_MAIL_VERIFYCODE, data, dataLen);

    if(loadingPage == NULL){
        loadingPage = new LoadingPage(this);
        loadingPage->show();
    }
}

void UserVerifyNewMailPage::on_hintMsgPage(bool isJump)
{
    hintMsgPage->hide();
    if(!isJump) return;
    emit readyRead(CommandCode::BACK_ACCOUNT_SAFE_PAGE, NULL, 0);
}

void UserVerifyNewMailPage::on_btnPrePage_clicked()
{
    this->hide();
    this->deleteLater();
}

UserVerifyNewMailPage::~UserVerifyNewMailPage()
{

}

