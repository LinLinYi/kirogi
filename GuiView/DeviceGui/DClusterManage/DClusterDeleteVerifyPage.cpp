#include <QVBoxLayout>
#include <QEvent>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>

#include "GlobalDefine.hpp"
#include "InputPacket.h"
#include "AssemblyData.h"
#include "UserProtocol.hpp"
#include "HintMsgPage.h"
#include "LoadingPage.h"
#include "NavigationBar.h"
#include "ImageConfig.hpp"
#include "SheetStyleConfig.hpp"
#include "DClusterDeleteVerifyPage.h"

DClusterDeleteVerifyPage::DClusterDeleteVerifyPage(QWidget* parent, quint64 clusID)
    : BasePage(parent)
    , clusterID(clusID)
    , loadingPage(NULL)
{
    initializeWidget();
}

void DClusterDeleteVerifyPage::initializeWidget()
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

void DClusterDeleteVerifyPage::fetchData(InputPacket& inpack)
{
    quint16 cmdCode = inpack.getCmdCode();
    switch(cmdCode)
    {
        case CommandCode::DCLUSTER_DELETE_VERIFY:
            this->handleDelDevClusterVerify(inpack);
            break;
        case CommandCode::NET_ERROR:
            this->handleNetError();
            break;
    }
}

void DClusterDeleteVerifyPage::handleNetError()
{
    if(loadingPage == NULL) return;
    QString lblText = tr("网络错误,请重试");
    this->handleHintInfo(lblText);
}

void DClusterDeleteVerifyPage::handleHintInfo(QString& text)
{
    hintMsgPage->setText(text);
    hintMsgPage->show();
    SAFE_DELETE(loadingPage);
}

void DClusterDeleteVerifyPage::handleDelDevClusterVerify(InputPacket& inpack)
{
    quint8 ansCode = inpack.getAnswerCode();
    if(ansCode == Mi::Failure){
        QString strText = tr("验证码错误,请重试");
        this->handleHintInfo(strText);
    }
    else if(Mi::Success){
        QString strText = tr("删除群成功");
        hintMsgPage->setStatus(true);
        this->handleHintInfo(strText);
    }
}

void DClusterDeleteVerifyPage::on_btnSubmit_clicked()
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
    assemblyData.append(clusterID);
    assemblyData.append(validateCode);
    char* data = assemblyData.getAssemblyData();
    quint16 dataLen = assemblyData.getAssemblyDataLen();
    emit readyRead(CommandCode::DCLUSTER_DELETE_VERIFY, data, dataLen);

    //页面加载窗口
    if(loadingPage == NULL){
        loadingPage = new LoadingPage(this);
        loadingPage->show();
    }
}

void DClusterDeleteVerifyPage::on_hintMsgPage(bool isJump)
{
    if(!isJump){
        hintMsgPage->hide();
        return;
    }

    this->hide();
    this->deleteLater();
    emit readyRead(CommandCode::BACK_DEVICE_PAGE, NULL, 0);
}

void DClusterDeleteVerifyPage::on_btnPrePage_clicked()
{
    this->hide();
    this->deleteLater();
}

DClusterDeleteVerifyPage::~DClusterDeleteVerifyPage()
{

}


