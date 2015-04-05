#include <QVBoxLayout>
#include <QEvent>
#include <QLineEdit>

#include "AssemblyData.h"
#include "GlobalDefine.hpp"
#include "InputPacket.h"
#include "HintMsgPage.h"
#include "LoadingPage.h"
#include "NavigationBar.h"
#include "UserProtocol.hpp"
#include "ImageConfig.hpp"
#include "SheetStyleConfig.hpp"
#include "RegisterVerifyPage.h"

RegisterVerifyPage::RegisterVerifyPage(QWidget* parent)
    : BasePage(parent)
    , loadingPage(NULL)
    , regAccountDeployPage(NULL)
{
    initializeWidget();
}

void RegisterVerifyPage::initializeWidget()
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

    //验证码相关部分
    lblMsgHint = new QLabel(this);
    lblMsgHint->setText(tr("已发送验证码到您的邮箱"));
    lblMsgHint->setFixedHeight(this->screenHeight()*0.05);
    lblMsgHint->setAlignment(Qt::AlignLeft);
    lblMsgHint->setContentsMargins(this->screenWidth()*0.02, this->screenWidth()*0.01, this->screenWidth()*0.02, this->screenWidth()*0.01);
    lblMsgHint->setStyleSheet("color:rgb(122,123,128); font:16px;");

    editAuthCode = new QLineEdit(this);
    editAuthCode->setPlaceholderText(tr("输入6位验证码"));
    editAuthCode->setFixedHeight(this->screenHeight()*0.08);
    editAuthCode->setTextMargins(this->screenWidth()*0.02, 0, this->screenWidth()*0.02, 0);
    editAuthCode->setStyleSheet(SheetStyle::REGISTER_VALIDATION_LINEEDIT);

    QVBoxLayout* vbInfoLayout = new QVBoxLayout;
    vbInfoLayout->addWidget(lblMsgHint);
    vbInfoLayout->addWidget(editAuthCode);
    vbInfoLayout->setAlignment(Qt::AlignCenter);
    vbInfoLayout->setSpacing(0);
    vbInfoLayout->setMargin(0);

    btnSubmit = new QPushButton(tr("提 交"), this);
    btnSubmit->setFixedHeight(this->screenHeight()*0.074);
    btnSubmit->setStyleSheet(SheetStyle::REGISTER_BUTTON);
    connect(btnSubmit, SIGNAL(clicked()), this, SLOT(on_btnSubmit_clicked()));

    QVBoxLayout* vbTotalLayout = new QVBoxLayout;
    vbTotalLayout->addLayout(vbInfoLayout);
    vbTotalLayout->addSpacing(this->screenHeight()*0.03);
    vbTotalLayout->addWidget(btnSubmit);
    vbTotalLayout->setContentsMargins(this->screenWidth()*0.054,this->screenHeight()*0.026,this->screenWidth()*0.054,0);
    vbTotalLayout->addStretch(0);
    vbTotalLayout->setSpacing(0);
    this->setBodyPartLayout(vbTotalLayout);

    //屏幕触摸滚动设置
    this->installScrollViewportArea();
}

void RegisterVerifyPage::fetchData(InputPacket &inpack)
{
    quint16 cmdCode = inpack.getCmdCode();
    switch(cmdCode)
    {
        case CommandCode::REGISTER_SECOND_STEP:
            this->handleRegisterSecondStep(inpack);
            break;
        case CommandCode::NET_ERROR:
            this->handleNetError();
            break;
    }
    transferDataToAnotherPage(inpack, regAccountDeployPage);
}

void RegisterVerifyPage::handleNetError()
{
    if(loadingPage == NULL)  return;
    QString strText = tr("网络错误,请重试");
    this->handleHintInfo(strText);
}

void RegisterVerifyPage::handleRegisterSecondStep(InputPacket &inpack)
{
    quint8 answerCode = inpack.getAnswerCode();
    if(answerCode == Mi::Success){
        SAFE_DELETE(loadingPage);
        regAccountDeployPage = new RegAccountDeployPage(this);
        this->displayConcretePage(regAccountDeployPage);
    }else if(answerCode == Mi::Error){
        QString strText = tr("验证码错误,请重新输入");
        this->handleHintInfo(strText);
    }else if(answerCode == Mi::Failure){
        QString strText = tr("验证失败,请重试");
        this->handleHintInfo(strText);
    }
}

void RegisterVerifyPage::displayConcretePage(BasePage *page)
{
    if(page == NULL) return;
    page->show();
    connect(page, SIGNAL(destroyed()), this, SLOT(on_deleteSubPages()));
    connect(page, SIGNAL(readyRead(quint16,char*,quint16)), this, SLOT(on_recvDataFromOtherPage(quint16,char*,quint16)));
}

void RegisterVerifyPage::transferDataToAnotherPage(InputPacket &inpack, BasePage *basePage)
{
    if(basePage == NULL) return;
    basePage->fetchData(inpack);
}

void RegisterVerifyPage::handleHintInfo(QString& text)
{
    hintMsgPage->setText(text);
    hintMsgPage->show();
    SAFE_DELETE(loadingPage);
}

void RegisterVerifyPage::on_btnPrePage_clicked()
{
    this->hide();
    this->deleteLater();
}

void RegisterVerifyPage::on_btnSubmit_clicked()
{
    if(editAuthCode->text().length() != 6){
        QString strText = tr("请输入6位验证码");
        this->handleHintInfo(strText);
        return;
    }

    QString strValidationCode = editAuthCode->text().trimmed();
    QByteArray byteValidationCode = strValidationCode.toLatin1();
    quint32 validateCode = byteValidationCode.toUInt();

    AssemblyData assemblyData;
    assemblyData.append(validateCode);
    char* data = assemblyData.getAssemblyData();
    quint16 dataLen = assemblyData.getAssemblyDataLen();
    emit readyRead(CommandCode::REGISTER_SECOND_STEP, data, dataLen);

    //页面加载窗口
    if(loadingPage == NULL){
        loadingPage = new LoadingPage(this);
        loadingPage->show();
    }
}

void RegisterVerifyPage::on_hintMsgPage(bool isJump)
{
    if(!isJump)
        hintMsgPage->hide();
}

void RegisterVerifyPage::on_deleteSubPages()
{
    regAccountDeployPage = NULL;
}

void RegisterVerifyPage::on_recvDataFromOtherPage(quint16 cmdCode, char *data, quint16 dataLen)
{
    emit readyRead(cmdCode, data, dataLen);
}

RegisterVerifyPage::~RegisterVerifyPage()
{

}

