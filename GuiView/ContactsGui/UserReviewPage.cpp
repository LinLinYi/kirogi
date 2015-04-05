#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

#include "AssemblyData.h"
#include "InputPacket.h"
#include "GroupButton.h"
#include "ImageConfig.hpp"
#include "NavigationBar.h"
#include "UserProtocol.hpp"
#include "UserReviewPage.h"
#include "ImageConfig.hpp"
#include "SheetStyleConfig.hpp"

UserReviewPage::UserReviewPage(QWidget *parent, UserJoinNotify& info)
    : BasePage(parent)
    , curNotifyInfo(info)
{
    this->initializeWidget();
}

void UserReviewPage::initializeWidget()
{
    //导航栏
    QString strTitle = tr("加入好友");
    QString strLeftBtn = tr("返回");
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

    //页面操作
    QPixmap headPixmap(ImagePath::USER_PORTRAIT);
    QPixmap directionpixmap(ImagePath::RIGHT_HOLLOW_ARROWS);
    directionpixmap.setDevicePixelRatio(2.0);
    btnSender = new GroupButton(this);
    btnSender->setPixMap(headPixmap);
    btnSender->setReserve(directionpixmap);
    btnSender->setText(curNotifyInfo.getUserName());
    btnSender->setFixedWidth(this->screenWidth());
    btnSender->setFixedHeight(this->screenHeight()*0.12);
    btnSender->setStyleSheet(SheetStyle::GROUPBUTTON_BOTTOMBORDER);
    connect(btnSender, SIGNAL(clicked()), this, SLOT(on_btnSender_clicked()));

    btnAgree = new QPushButton(this);
    btnAgree->setText(tr("同意"));
    btnAgree->setFixedHeight(this->screenHeight()*0.074);
    btnAgree->setFixedWidth(this->screenWidth()*0.43);
    btnAgree->setStyleSheet("color:white; border-radius:3px; background-color:#0090ff;");
    connect(btnAgree, SIGNAL(clicked()), this, SLOT(on_btnAgree_clicked()));

    btnRefuse = new QPushButton(this);
    btnRefuse->setText(tr("拒绝"));
    btnRefuse->setFixedHeight(this->screenHeight()*0.074);
    btnRefuse->setFixedWidth(this->screenWidth()*0.43);
    btnRefuse->setStyleSheet("color:#8c8c8c; border-radius:3px; background-color:white;");
    connect(btnRefuse, SIGNAL(clicked()), this, SLOT(on_btnRefuse_clicked()));

    lblCheckHint = new QLabel(this);
    lblCheckHint->setFixedWidth(this->screenWidth());
    lblCheckHint->setAlignment(Qt::AlignCenter);
    lblCheckHint->setStyleSheet("color:#8c8c8c;");

    curCheckStatus = curNotifyInfo.getMsgStatus();
    if(curCheckStatus != Mi::Pending){
        this->hasCheckedDisplay(curCheckStatus);
    }

    QHBoxLayout* hbLayout = new QHBoxLayout;
    hbLayout->addWidget(btnRefuse);
    hbLayout->addWidget(btnAgree);
    hbLayout->setSpacing(this->screenWidth()*0.047);
    hbLayout->setContentsMargins(this->screenWidth()*0.047, this->screenHeight()*0.037, 0, 0);

    QVBoxLayout* vbLayout = new QVBoxLayout;
    vbLayout->addWidget(btnSender);
    vbLayout->addLayout(hbLayout);
    vbLayout->addWidget(lblCheckHint);
    vbLayout->setMargin(0);
    vbLayout->setSpacing(0);
    vbLayout->addStretch(0);
    this->setBodyPartLayout(vbLayout);

    //屏幕触摸滚动设置
    this->installScrollViewportArea();
}

void UserReviewPage::fetchData(InputPacket &inpack)
{
    quint16 cmdCode = inpack.getCmdCode();
    switch(cmdCode)
    {
        case CommandCode::USER_JOIN_APPROVAL:
            this->hasCheckedDisplay(curCheckStatus);
            break;
    }
}

void UserReviewPage::on_btnPrePage_clicked()
{
    this->hide();
    this->deleteLater();
}

void UserReviewPage::hasCheckedDisplay(Mi::MsgStatus status)
{
    QString strText;
    if(status == Mi::Agreed)
        strText = QString(tr("已同意该申请"));
    else if(status == Mi::Refused)
        strText = QString(tr("已拒绝该申请"));
    else
        return;

    btnAgree->hide();
    btnRefuse->hide();
    lblCheckHint->show();
    lblCheckHint->setText(strText);
}

void UserReviewPage::on_btnSender_clicked()
{
    //////////////////////////////////////////////
}

void UserReviewPage::on_btnAgree_clicked()
{
    quint8 ansCode = Mi::Agreed;
    curCheckStatus = Mi::Agreed;
    this->sendCheckedStatus(ansCode);
}

void UserReviewPage::on_btnRefuse_clicked()
{
    quint8 ansCode = Mi::Refused;
    curCheckStatus = Mi::Refused;
    this->sendCheckedStatus(ansCode);
}

void UserReviewPage::sendCheckedStatus(quint8 ansCode)
{
    AssemblyData assemblyData;
    assemblyData.append(ansCode);
    assemblyData.append(curNotifyInfo.getUserID());
    char* data = assemblyData.getAssemblyData();
    quint16 dataLen = assemblyData.getAssemblyDataLen();
    emit readyRead(CommandCode::USER_JOIN_APPROVAL, data, dataLen);
}

UserReviewPage::~UserReviewPage()
{

}


