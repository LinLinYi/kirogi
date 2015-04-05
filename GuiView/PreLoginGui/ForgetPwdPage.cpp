#include <QHBoxLayout>

#include "AssemblyData.h"
#include "GlobalDefine.hpp"
#include "HintMsgPage.h"
#include "NavigationBar.h"
#include "UserProtocol.hpp"
#include "ForgetPwdPage.h"
#include "LoadingPage.h"
#include "ImageConfig.hpp"
#include "HintMsgPage.h"
#include "SheetStyleConfig.hpp"

ForgetPwdPage::ForgetPwdPage(QWidget* parent) :
    BasePage(parent)
  , loadingPage(NULL)
  , forgetPwdValidatePage(NULL)
{
    this->initializeWidget();
}

void ForgetPwdPage::initializeWidget()
{
    //提示信息页面操作
    hintMsgPage = new HintMsgPage(this);
    hintMsgPage->hide();
    connect(hintMsgPage, SIGNAL(submitClicked(bool)), this, SLOT(on_hintMsgPage(bool)));

    //导航栏
    QString strTitle = tr("找回密码");
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

    //主体部分
    editMail = new QLineEdit(this);
    editMail->setFixedHeight(this->screenHeight()*0.077);
    editMail->setPlaceholderText(tr("输入您绑定的邮箱地址"));
    editMail->setStyleSheet(SheetStyle::QLINEEDIT);
    editMail->setTextMargins(this->screenWidth()*0.02, 0, this->screenWidth()*0.02, 0);

    btnApply = new QPushButton(tr("申 请"), this);
    btnApply->setFixedHeight(this->screenHeight()*0.074);
    btnApply->setStyleSheet(SheetStyle::QPUSHBUTTON);
    connect(btnApply, SIGNAL(clicked()), this, SLOT(on_btnApply_clicked()));

    QVBoxLayout* vbLayout = new QVBoxLayout;
    vbLayout->addWidget(editMail);
    vbLayout->addSpacing(this->screenHeight()*0.03);
    vbLayout->addWidget(btnApply);
    vbLayout->setAlignment(Qt::AlignTop);
    vbLayout->addStretch(0);
    vbLayout->setSpacing(0);
    vbLayout->setContentsMargins(this->screenWidth()*0.054,this->screenHeight()*0.026,this->screenWidth()*0.054,0);

    this->setBodyPartLayout(vbLayout);
}

void ForgetPwdPage::fetchData(InputPacket &inpack)
{
    quint16 cmdCode = inpack.getCmdCode();
    switch(cmdCode)
    {
        case CommandCode::FORGET_PASSWORD_APPLY:
            handleForgetPwdApply(inpack);
            break;
        case CommandCode::NET_ERROR:
            handleNetError();
            break;
    }

    transferDataToAnotherPage(inpack, forgetPwdValidatePage);
}

void ForgetPwdPage::handleHintInfo(QString &strText)
{
    hintMsgPage->setText(strText);
    hintMsgPage->show();
    SAFE_DELETE(loadingPage);
}

void ForgetPwdPage::handleNetError()
{
    if(loadingPage == NULL) return;
    QString lblText = tr("网络连接错误,请重试");
    this->handleHintInfo(lblText);
}

void ForgetPwdPage::handleForgetPwdApply(InputPacket &inpack)
{
    quint8 answerCode = inpack.getAnswerCode();
    if(answerCode == Mi::Success){
        SAFE_DELETE(loadingPage);
        forgetPwdValidatePage = new ForgetPwdVerifyPage(this);
        this->displayConcretePage(forgetPwdValidatePage);
    }
    else if(answerCode == Mi::Failure){
        QString strText = tr("申请修改密码失败,请重试");
        this->handleHintInfo(strText);
    }
}

void ForgetPwdPage::displayConcretePage(BasePage *page)
{
    if(page == NULL) return;
    page->show();
    connect(page, SIGNAL(destroyed()), this, SLOT(on_deleteSubPages()));
    connect(page, SIGNAL(readyRead(quint16,char*,quint16)), this, SLOT(on_recvDataFromOtherPage(quint16,char*,quint16)));
}

void ForgetPwdPage::transferDataToAnotherPage(InputPacket &inpack, BasePage *basePage)
{
    if(basePage == NULL) return;
    basePage->fetchData(inpack);
}

void ForgetPwdPage::on_btnApply_clicked()
{
    QRegExp regx("([0-9A-Za-z\\-_\\.]+)@([0-9a-z-\\-]+\\.[a-z]{2,3}(\\.[a-z]{2})?)");
    if(!regx.exactMatch(editMail->text().trimmed())){
        QString str = tr("邮箱格式不正确");
        hintMsgPage->setText(str);
        hintMsgPage->show();
        return;
    }

    QString strMail = editMail->text().trimmed();
    AssemblyData assemblyData;
    assemblyData.append(strMail);
    char* data = assemblyData.getAssemblyData();
    quint16 dataLen = assemblyData.getAssemblyDataLen();
    emit readyRead(CommandCode::FORGET_PASSWORD_APPLY, data, dataLen);

    if(loadingPage == NULL){
        loadingPage = new LoadingPage(this);
        loadingPage->show();
    }
}

void ForgetPwdPage::on_hintMsgPage(bool isJump)
{
    if(!isJump)
        hintMsgPage->hide();
}

void ForgetPwdPage::on_deleteSubPages()
{
     forgetPwdValidatePage = NULL;
}

void ForgetPwdPage::on_recvDataFromOtherPage(quint16 cmdCode, char *dataField, quint16 dataLen)
{
    emit readyRead(cmdCode, dataField, dataLen);
}

void ForgetPwdPage::on_btnPrePage_clicked()
{
    this->hide();
    this->deleteLater();
}

ForgetPwdPage::~ForgetPwdPage()
{

}
