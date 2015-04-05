#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

#include "GroupButton.h"
#include "UserProtocol.hpp"
#include "NavigationBar.h"
#include "ImageConfig.hpp"
#include "SheetStyleConfig.hpp"
#include "UClusterReviewPage.h"
#include "InputPacket.h"
#include "AssemblyData.h"

UClusterReviewPage::UClusterReviewPage(QWidget *parent, UClusterJoinNotify& info)
    : BasePage(parent)
    , curNotifyInfo(info)
{
    this->initializeWidget();
}

void UClusterReviewPage::initializeWidget()
{
    //导航栏
    QString strTitle = tr("加入群");
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
    btnSender->setFixedHeight(this->screenHeight()*0.1);
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
    curCheckType = curNotifyInfo.getNotifyType();
    if(curCheckStatus != Mi::Pending){
        this->hasCheckedDisplay(curCheckStatus, curCheckType);
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

void UClusterReviewPage::fetchData(InputPacket &inpack)
{
    quint16 cmdCode = inpack.getCmdCode();
    switch(cmdCode){
        case CommandCode::UCLUSTER_INVITE_APPROVAL:
        case CommandCode::UCLUSTER_APPLY_APPROVAL:
            Mi::ClusterJoin type = Mi::JoinNone;
            if(cmdCode == CommandCode::UCLUSTER_INVITE_APPROVAL)
                type = Mi::ClusterInviteReq;
            else if(cmdCode == CommandCode::UCLUSTER_APPLY_APPROVAL)
                type = Mi::ClusterApplyReq;
            else
                return;

            this->hasCheckedDisplay(curCheckStatus, type);
            break;
    }
}

void UClusterReviewPage::on_btnPrePage_clicked()
{
    this->hide();
    this->deleteLater();
}

void UClusterReviewPage::hasCheckedDisplay(Mi::MsgStatus status, Mi::ClusterJoin type)
{
    QString strText;
    if(status == Mi::Agreed && type == Mi::ClusterApplyReq)
        strText = QString(tr("已同意该申请"));
    else if(status == Mi::Refused && type == Mi::ClusterApplyReq)
        strText = QString(tr("已拒绝该申请"));
    else if(status == Mi::Agreed && type == Mi::ClusterInviteReq)
        strText = QString(tr("已同意该邀请"));
    else if(status == Mi::Refused && type == Mi::ClusterInviteReq)
        strText = QString(tr("已拒绝该邀请"));
    else
        return;

    btnAgree->hide();
    btnRefuse->hide();
    lblCheckHint->show();
    lblCheckHint->setText(strText);
}

void UClusterReviewPage::on_btnSender_clicked()
{
    //////////////////////////////////////////////
}

void UClusterReviewPage::on_btnAgree_clicked()
{
    quint8 ansCode = Mi::Agreed;
    curCheckStatus = Mi::Agreed;
    this->sendCheckedStatus(ansCode);
}

void UClusterReviewPage::on_btnRefuse_clicked()
{
    quint8 ansCode = Mi::Refused;
    curCheckStatus = Mi::Refused;
    this->sendCheckedStatus(ansCode);
}

void UClusterReviewPage::sendCheckedStatus(quint8 ansCode)
{
    AssemblyData assemblyData;
    assemblyData.append(ansCode);
    assemblyData.append(curNotifyInfo.getClusterID());
    assemblyData.append(curNotifyInfo.getUserID());
    char* data = assemblyData.getAssemblyData();
    quint16 dataLen = assemblyData.getAssemblyDataLen();

    Mi::ClusterJoin type = curNotifyInfo.getNotifyType();
    if(type == Mi::ClusterApplyReq)
        emit readyRead(CommandCode::UCLUSTER_APPLY_APPROVAL, data, dataLen);
    else if(type == Mi::ClusterInviteReq)
        emit readyRead(CommandCode::UCLUSTER_INVITE_APPROVAL, data, dataLen);
}

UClusterReviewPage::~UClusterReviewPage()
{

}
