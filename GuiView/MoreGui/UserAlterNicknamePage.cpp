#include <QVBoxLayout>
#include <QLineEdit>

#include "AssemblyData.h"
#include "InputPacket.h"
#include "HintMsgPage.h"
#include "UserProtocol.hpp"
#include "ImageConfig.hpp"
#include "NavigationBar.h"
#include "LoadingPage.h"
#include "HintMsgPage.h"
#include "SheetStyleConfig.hpp"
#include "UserAlterNicknamePage.h"

UserAlterNicknamePage::UserAlterNicknamePage(QWidget* parent, quint64 accountID)
    :BasePage(parent)
    , curAccountID(accountID)
    , loadingPage(NULL)
{
    this->initializeWidget();
    this->loadLocalData(curAccountID);
}

void UserAlterNicknamePage::initializeWidget()
{
    //提示信息页面操作
    hintMsgPage = new HintMsgPage(this);
    hintMsgPage->hide();
    connect(hintMsgPage, SIGNAL(submitClicked(bool)), this, SLOT(on_hintMsgPage(bool)));

    //导航栏
    QString strTitle = tr("昵称");
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

    //昵称
    editNickname = new QLineEdit(this);
    editNickname->setTextMargins(this->screenWidth()*0.02, 0, 0, 0);
    editNickname->setFixedHeight(this->screenHeight()*0.07);
    editNickname->setStyleSheet("border-style:solid; border-radius:0px; border-width: 0px;\
                               border-color:rgb(208, 208, 208); background-color:white; font:15px;");

    QVBoxLayout* vblTotalLayout = new QVBoxLayout;
    vblTotalLayout->addSpacing(this->screenHeight()*0.026);
    vblTotalLayout->addWidget(editNickname);
    vblTotalLayout->setMargin(0);
    vblTotalLayout->setAlignment(Qt::AlignTop);
    vblTotalLayout->setSpacing(0);
    vblTotalLayout->addStretch(0);
    this->setBodyPartLayout(vblTotalLayout);

    //屏幕触摸滚动设置
    this->installScrollViewportArea();
}

void UserAlterNicknamePage::fetchData(InputPacket &inpack)
{
    quint16 cmdCode = inpack.getCmdCode();
    switch(cmdCode)
    {
        case CommandCode::MODIFY_ACCOUNT_NICKNAME:
            this->handleNickNameInfo(inpack);
            break;
        case CommandCode::NET_ERROR:
            this->handleNetError();
            break;
    }
}

void UserAlterNicknamePage::loadLocalData(quint64 userID)
{
    curUserInfo.loadDataSource(userID);
    editNickname->setText(curUserInfo.getRemarks());
}

void UserAlterNicknamePage::handleNickNameInfo(InputPacket& inpack)
{
    quint16 ansCode = inpack.getAnswerCode();
    if(ansCode == Mi::Success){
        QString nickName = editNickname->text().trimmed();
        curUserInfo.updateRemarks(nickName);//更新数据库

        QString strText = tr("修改成功");
        hintMsgPage->setStatus(true);
        this->handleHintInfo(strText);
    }else if(ansCode == Mi::Failure){
        QString strText = tr("修改失败,请重试");
        this->handleHintInfo(strText);
    }else
        SAFE_DELETE(loadingPage);
}

void UserAlterNicknamePage::handleHintInfo(QString &strText)
{
    hintMsgPage->setText(strText);
    hintMsgPage->show();
    SAFE_DELETE(loadingPage);
}

void UserAlterNicknamePage::handleNetError()
{
    if(loadingPage == NULL) return;
    QString strText = tr("网络错误,请重试");
    this->handleHintInfo(strText);
}

void UserAlterNicknamePage::on_btnSubmitPage_clicked()
{
    QString strNickName = editNickname->text().trimmed();
    AssemblyData assemblyData;
    assemblyData.append(strNickName);
    char* data = assemblyData.getAssemblyData();
    quint16 dataLen = assemblyData.getAssemblyDataLen();
    emit readyRead(CommandCode::MODIFY_ACCOUNT_NICKNAME, data, dataLen);

    if(loadingPage == NULL){
        loadingPage = new LoadingPage(this);
        loadingPage->show();
    }
}

void UserAlterNicknamePage::on_hintMsgPage(bool isJump)
{
    hintMsgPage->hide();
    if(!isJump) return;
    this->hide();
    this->deleteLater();
}

void UserAlterNicknamePage::on_btnPrePage_clicked()
{
    this->hide();
    this->deleteLater();
}

UserAlterNicknamePage::~UserAlterNicknamePage()
{

}

