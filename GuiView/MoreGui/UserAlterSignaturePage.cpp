#include <QVBoxLayout>
#include <QTextEdit>

#include "HintMsgPage.h"
#include "LoadingPage.h"
#include "NavigationBar.h"
#include "UserProtocol.hpp"
#include "ImageConfig.hpp"
#include "InputPacket.h"
#include "AssemblyData.h"
#include "SheetStyleConfig.hpp"
#include "UserAlterSignaturePage.h"

UserAlterSignaturePage::UserAlterSignaturePage(QWidget* parent, quint64 userID)
    :BasePage(parent)
    , curAccountID(userID)
    , loadingPage(NULL)
{
    this->initializeWidget();
}

void UserAlterSignaturePage::initializeWidget()
{
    //提示信息页面操作
    hintMsgPage = new HintMsgPage(this);
    hintMsgPage->hide();
    connect(hintMsgPage, SIGNAL(submitClicked(bool)), this, SLOT(on_hintMsgPage(bool)));

    //导航栏
    QString strTitle = tr("签名");
    QString strLeftBtn = tr("取消");
    QString strRightBtn = tr("提交");
    QPixmap leftArrowsPixmap(ImagePath::NAVIGATION_LEFT_ARROWS);
    navigationBar = new NavigationBar(this);
    navigationBar->setTitleText(strTitle);
    navigationBar->setLeftText(strLeftBtn);
    navigationBar->setRightText(strRightBtn);
    navigationBar->setLeftPixMap(leftArrowsPixmap);
    connect(navigationBar, SIGNAL(leftClicked()), this, SLOT(on_btnPrePage_clicked()));
    connect(navigationBar, SIGNAL(rightClicked()), this, SLOT(on_btnSubmitPage_clicked()));

    QHBoxLayout* pHLTop = new QHBoxLayout();
    pHLTop->addWidget(navigationBar);
    pHLTop->setSpacing(0);
    pHLTop->setMargin(0);
    this->setTopbarLayout(pHLTop);

    //签名
    editSignature = new QTextEdit(this);
    editSignature->setFixedHeight(this->screenHeight()*0.30);
    editSignature->setContentsMargins(this->screenWidth()*0.02, this->screenWidth()*0.02, this->screenWidth()*0.02, 0);
    editSignature->setStyleSheet("border-style:solid; border-radius:0px; border-width: 0px; \
                                border-color:rgb(208, 208, 208); background-color:white; font:15px;");

    QVBoxLayout* vblTotalLayout = new QVBoxLayout;
    vblTotalLayout->addSpacing(this->screenHeight()*0.026);
    vblTotalLayout->addWidget(editSignature);
    vblTotalLayout->setMargin(0);
    vblTotalLayout->setAlignment(Qt::AlignTop);
    vblTotalLayout->setSpacing(0);
    vblTotalLayout->addStretch(0);
    this->setBodyPartLayout(vblTotalLayout);

    //屏幕触摸滚动设置
    this->installScrollViewportArea();
    this->loadLocalData();
}

void UserAlterSignaturePage::fetchData(InputPacket &inpack)
{
    quint16 cmdCode = inpack.getCmdCode();
    switch(cmdCode)
    {
        case CommandCode::MODIFY_ACCOUNT_SIGNATURE:
            this->handleSignatureInfo(inpack);
            break;
        case CommandCode::NET_ERROR:
            this->handleNetError();
            break;
    }
}

void UserAlterSignaturePage::loadLocalData()
{
    curUserInfo.loadDataSource(curAccountID);
    editSignature->setText(curUserInfo.getDescribe());
}

void UserAlterSignaturePage::handleSignatureInfo(InputPacket& inpack)
{
    quint16 ansCode = inpack.getAnswerCode();
    if(ansCode == Mi::Success){
        QString strSignatrue = editSignature->toPlainText().trimmed();
        curUserInfo.updateDescribe(strSignatrue);

        QString strText = tr("修改成功");
        hintMsgPage->setStatus(true);
        this->handleHintInfo(strText);
    }else if(ansCode == Mi::Failure){
        QString strText = tr("修改失败,请重试");
        this->handleHintInfo(strText);
    }else
        SAFE_DELETE(loadingPage);
}

void UserAlterSignaturePage::handleHintInfo(QString &strText)
{
    hintMsgPage->setText(strText);
    hintMsgPage->show();
    SAFE_DELETE(loadingPage);
}

void UserAlterSignaturePage::handleNetError()
{
    if(loadingPage == NULL) return;
    QString strText = tr("网络错误,请重试");
    this->handleHintInfo(strText);
}

void UserAlterSignaturePage::on_btnSubmitPage_clicked()
{
    QString strUserSignature = editSignature->toPlainText().trimmed();
    AssemblyData assemblyData;
    assemblyData.append(strUserSignature);
    char* data = assemblyData.getAssemblyData();
    quint16 dataLen = assemblyData.getAssemblyDataLen();
    emit readyRead(CommandCode::MODIFY_ACCOUNT_SIGNATURE, data, dataLen);

    if(loadingPage == NULL){
        loadingPage = new LoadingPage(this);
        loadingPage->show();
    }
}

void UserAlterSignaturePage::on_hintMsgPage(bool isJump)
{
    hintMsgPage->hide();
    if(!isJump) return;
    this->hide();
    this->deleteLater();
}

void UserAlterSignaturePage::on_btnPrePage_clicked()
{
    this->hide();
    this->deleteLater();
}

UserAlterSignaturePage::~UserAlterSignaturePage()
{

}

