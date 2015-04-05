#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "MD5.h"
#include "AssemblyData.h"
#include "GlobalDefine.hpp"
#include "HintMsgPage.h"
#include "ImageConfig.hpp"
#include "LoadingPage.h"
#include "SheetStyleConfig.hpp"
#include "UserRegisterPage.h"
#include "UserProtocol.hpp"
#include "OutPutPacket.h"
#include "NavigationBar.h"

UserRegisterPage::UserRegisterPage(QWidget* parent)
    : BasePage(parent)
    , loadingPage(NULL)
    , regValidationPage(NULL)
{
    this->initializeWidget();
}

void UserRegisterPage::initializeWidget()
{
    //提示信息页面操作
    hintMsgPage = new HintMsgPage(this);
    hintMsgPage->hide();
    connect(hintMsgPage, SIGNAL(submitClicked(bool)), this, SLOT(on_hintMsgPage(bool)));

    //导航栏
    QString strTitle = tr("注 册");
    QString strLeftBtn = tr("取消");
    QPixmap leftArrowsPixmap(ImagePath::NAVIGATION_LEFT_ARROWS);
    m_navigationBar = new NavigationBar(this);
    m_navigationBar->setTitleText(strTitle);
    m_navigationBar->setLeftText(strLeftBtn);
    m_navigationBar->setLeftPixMap(leftArrowsPixmap);
    connect(m_navigationBar, SIGNAL(leftClicked()), this, SLOT(on_btnCancel_Clicked()));

    QHBoxLayout* pHLTop = new QHBoxLayout();
    pHLTop->addWidget(m_navigationBar);
    pHLTop->setSpacing(0);
    pHLTop->setMargin(0);
    this->setTopbarLayout(pHLTop);

    //用户邮箱
    m_pEditEmail = new QLineEdit(this);
    m_pEditEmail->setStyleSheet(SheetStyle::QLINEEDIT);
    m_pEditEmail->setFixedHeight(this->screenHeight()*0.077);
    m_pEditEmail->setPlaceholderText(tr("输入您的邮箱地址"));
    m_pEditEmail->setMaxLength(50);
    m_pEditEmail->setTextMargins(this->screenWidth()*0.03, 0, 0, 0);

    QVBoxLayout* vbMailLayout = new QVBoxLayout;
    vbMailLayout->addWidget(m_pEditEmail);
    vbMailLayout->setMargin(0);

    m_pBtnRegister = new QPushButton(QObject::tr("注  册"));
    m_pBtnRegister->setFixedHeight(this->screenHeight()*0.074);
    m_pBtnRegister->setStyleSheet(SheetStyle::REGISTER_BUTTON);
    connect(m_pBtnRegister, SIGNAL(clicked()), this, SLOT(on_btnRegister_Clicked()));

    QVBoxLayout* pVlTotal = new QVBoxLayout();
    pVlTotal->addLayout(vbMailLayout);
    pVlTotal->addSpacing(this->screenHeight()*0.03);
    pVlTotal->addWidget(m_pBtnRegister);
    pVlTotal->setSpacing(0);
    pVlTotal->addStretch(0);
    pVlTotal->setContentsMargins(this->screenWidth()*0.054,this->screenHeight()*0.026,this->screenWidth()*0.054,0);
    this->setBodyPartLayout(pVlTotal);
}

void UserRegisterPage::fetchData(InputPacket& inpack)
{
    quint16 cmdCode = inpack.getCmdCode();
    switch(cmdCode)
    {
        case CommandCode::REGISTER_FIRST_STEP:
            this->handleRegisterFirstStep(inpack);
            break;
        case CommandCode::NET_ERROR:
            this->handleNetError();
            break;
    }

    this->transferDataToAnotherPage(inpack, regValidationPage);
}

void UserRegisterPage::handleNetError()
{
    if(loadingPage == NULL) return;
    QString lblText = tr("网络错误,请重试.");
    this->handleHintInfo(lblText);
}

void UserRegisterPage::handleRegisterFirstStep(InputPacket &inpack)
{
    quint8 answerCode = inpack.getAnswerCode();
    if(answerCode == Mi::Success){
        SAFE_DELETE(loadingPage);
        regValidationPage = new RegisterVerifyPage(this);
        this->displayConcretePage(regValidationPage);
    }else if(answerCode == Mi::Used){
        QString lblText = tr("用户邮箱已使用");
        this->handleHintInfo(lblText);
    }else if(answerCode == Mi::Failure){
        QString lblText = tr("注册失败,请重试");
        this->handleHintInfo(lblText);
    }
}

void UserRegisterPage::displayConcretePage(BasePage *page)
{
    if(page == NULL) return;
    page->show();
    connect(page, SIGNAL(destroyed()), this, SLOT(on_deleteSubPages()));
    connect(page, SIGNAL(readyRead(quint16,char*,quint16)), this, SLOT(on_recvDataFromOtherPage(quint16,char*,quint16)));
}

void UserRegisterPage::transferDataToAnotherPage(InputPacket &inpack, BasePage *basePage)
{
    if(basePage == NULL) return;
    basePage->fetchData(inpack);
}

void UserRegisterPage::handleHintInfo(QString& text)
{
    hintMsgPage->setText(text);
    hintMsgPage->show();
    SAFE_DELETE(loadingPage);
}

void UserRegisterPage::on_btnRegister_Clicked()
{   
    QString userMail = m_pEditEmail->text().trimmed();
    QRegExp regx("([0-9A-Za-z\\-_\\.]+)@([0-9a-z-\\-]+\\.[a-z]{2,3}(\\.[a-z]{2})?)");
    if(!regx.exactMatch(m_pEditEmail->text().trimmed())){
        QString lblText = tr("邮箱格式不正确");
        hintMsgPage->setText(lblText);
        hintMsgPage->show();
        return;
    }

    AssemblyData assemblyData;
    assemblyData.append(userMail);
    char* data = assemblyData.getAssemblyData();
    quint16 dataLen = assemblyData.getAssemblyDataLen();
    emit readyRead(CommandCode::REGISTER_FIRST_STEP, data, dataLen);

    //页面加载窗口
    if(loadingPage ==NULL){
        loadingPage = new LoadingPage(this);
        loadingPage->show();
    }
}

void UserRegisterPage::on_hintMsgPage(bool isJump)
{
    if(!isJump)
        hintMsgPage->hide();
}

void UserRegisterPage::on_btnCancel_Clicked()
{
    this->hide();
    this->deleteLater();
}

void UserRegisterPage::on_deleteSubPages()
{
    regValidationPage = NULL;
}

void UserRegisterPage::on_recvDataFromOtherPage(quint16 cmdCode, char *dataField, quint16 dataLen)
{
    emit readyRead(cmdCode, dataField, dataLen);
}

UserRegisterPage::~UserRegisterPage()
{

}
