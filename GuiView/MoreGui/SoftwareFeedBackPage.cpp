#include <QVBoxLayout>
#include <QTextEdit>

#include "AssemblyData.h"
#include "HintMsgPage.h"
#include "LoadingPage.h"
#include "NavigationBar.h"
#include "UserProtocol.hpp"
#include "ImageConfig.hpp"
#include "InputPacket.h"
#include "NavigationBar.h"
#include "HintMsgPage.h"
#include "LoadingPage.h"
#include "GlobalDefine.hpp"
#include "SheetStyleConfig.hpp"
#include "SoftwareFeedBackPage.h"

SoftwareFeedBackPage::SoftwareFeedBackPage(QWidget* parent, quint64 userID)
    :BasePage(parent)
    , curAccountID(userID)
    , loadingPage(NULL)
{
    this->initializeWidget();
}

void SoftwareFeedBackPage::initializeWidget()
{
    //提示信息页面操作
    hintMsgPage = new HintMsgPage(this);
    hintMsgPage->hide();
    connect(hintMsgPage, SIGNAL(submitClicked(bool)), this, SLOT(on_hintMsgPage(bool)));

    //导航栏
    QString strTitle = tr("反馈");
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

    //反馈
    editFeedback = new QTextEdit(this);
    editFeedback->setFixedHeight(this->screenHeight()*0.30);
    editFeedback->setPlaceholderText(QString("请输入您的反馈意见"));
    editFeedback->setContentsMargins(this->screenWidth()*0.02, this->screenWidth()*0.02, this->screenWidth()*0.02, 0);
    editFeedback->setStyleSheet("border:none; background-color:white; font:16px;");

    QVBoxLayout* vblTotalLayout = new QVBoxLayout;
    vblTotalLayout->addWidget(editFeedback);
    vblTotalLayout->setMargin(0);
    vblTotalLayout->setAlignment(Qt::AlignTop);
    vblTotalLayout->setSpacing(0);
    vblTotalLayout->addStretch(0);
    this->setBodyPartLayout(vblTotalLayout);

    //屏幕触摸滚动设置
    this->installScrollViewportArea();
}

void SoftwareFeedBackPage::fetchData(InputPacket &inpack)
{
    quint16 cmdCode = inpack.getCmdCode();
    switch(cmdCode)
    {
        case CommandCode::SOFTWARE_INFO_FEEDBACK:
            this->handleFeedbackInfo(inpack);
            break;
        case CommandCode::NET_ERROR:
            this->handleNetError();
            break;
    }
}

void SoftwareFeedBackPage::handleFeedbackInfo(InputPacket& inpack)
{
    quint16 ansCode = inpack.getAnswerCode();
    if(ansCode == Mi::Success){
        QString strText = tr("提交成功");
        hintMsgPage->setStatus(true);
        this->handleHintInfo(strText);
    }else if(ansCode == Mi::Failure){
        QString strText = tr("提交失败,请重试");
        this->handleHintInfo(strText);
    }
}

void SoftwareFeedBackPage::handleHintInfo(QString &strText)
{
    hintMsgPage->setText(strText);
    hintMsgPage->show();
    SAFE_DELETE(loadingPage);
}

void SoftwareFeedBackPage::handleNetError()
{
    if(loadingPage == NULL) return;
    QString strText = tr("网络错误,请重试");
    this->handleHintInfo(strText);
}

void SoftwareFeedBackPage::on_btnSubmitPage_clicked()
{
    QString strUserFeedback = editFeedback->toPlainText().trimmed();
    AssemblyData assemblyData;
    assemblyData.append(strUserFeedback);
    char* data = assemblyData.getAssemblyData();
    quint16 dataLen = assemblyData.getAssemblyDataLen();
    emit readyRead(CommandCode::SOFTWARE_INFO_FEEDBACK, data, dataLen);

    if(loadingPage == NULL){
        loadingPage = new LoadingPage(this);
        loadingPage->show();
    }
}

void SoftwareFeedBackPage::on_hintMsgPage(bool isJump)
{
    hintMsgPage->hide();
    if(!isJump)  return;
    this->hide();
    this->deleteLater();
}

void SoftwareFeedBackPage::on_btnPrePage_clicked()
{
    this->hide();
    this->deleteLater();
}

SoftwareFeedBackPage::~SoftwareFeedBackPage()
{

}

