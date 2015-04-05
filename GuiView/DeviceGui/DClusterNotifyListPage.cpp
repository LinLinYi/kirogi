﻿#include <QHBoxLayout>

#include "DispersalData.h"
#include "InputPacket.h"
#include "AssemblyData.h"
#include "ListWidget.h"
#include "CheckButton.h"
#include "UserProtocol.hpp"
#include "ImageConfig.hpp"
#include "ServerNotifyInfo.h"
#include "NavigationBar.h"
#include "DClusterReviewPage.h"
#include "DClusterJoinNotifyList.h"
#include "SheetStyleConfig.hpp"
#include "DClusterNotifyListPage.h"

DClusterNotifyListPage::DClusterNotifyListPage(QWidget* parent)
    :BasePage(parent)
    , curFixedContentHeight(0)
    , notifyListWidget(NULL)
    , clusterReviewPage(NULL)
    , notifyList(NULL)
{
    this->initializeWidget();
}

void DClusterNotifyListPage::initializeWidget()
{
    //导航栏
    QString strTitle = tr("群通知");
    QString strLeftBtn = tr("消息");
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

    //群成员布局
    notifyListWidget = new ListWidget(this);
    notifyListWidget->setStyleSheet(SheetStyle::LISTWIDGET);
    notifyListWidget->setContentsMargins(this->screenWidth()*0.03,0,0,0);
    curFixedContentHeight += notifyListWidget->height();

    QVBoxLayout* vblTotalLayout = new QVBoxLayout;
    vblTotalLayout->addWidget(notifyListWidget);
    vblTotalLayout->addStretch(0);
    vblTotalLayout->setMargin(0);
    vblTotalLayout->setSpacing(0);
    vblTotalLayout->setAlignment(Qt::AlignTop);
    this->setBodyPartLayout(vblTotalLayout);

    this->loadLocalDataSource();
    this->installScrollViewportArea();
    this->setBodyRealHeight(curFixedContentHeight);
}

void DClusterNotifyListPage::fetchData(InputPacket &inpack)
{
    quint16 cmdCode = inpack.getCmdCode();
    switch(cmdCode)
    {
        case CommandCode::DCLUSTER_APPLY_APPROVAL:
        case CommandCode::DCLUSTER_INVITE_APPROVAL:
            this->recvClusterCheckResultFromServer(inpack);
            break;
        case CommandCode::SERVER_NOTIFY:
            this->recvNotifyInfoFromServer(inpack);
            break;
    }
    this->transferDataToAnotherPage(inpack, clusterReviewPage);
}

void DClusterNotifyListPage::loadLocalDataSource()
{
    SAFE_DELETE(notifyList);
    notifyList = new DClusterJoinNotifyList;
    notifyList->loadDataSource();
    this->deployMsgNotifyList(notifyList->fetchList());
}

void DClusterNotifyListPage::recvClusterCheckResultFromServer(InputPacket &inpack)
{
    quint64 userID = 0;
    quint16 offset = 0;
    quint8 result = 0;
    quint64 clusterID = 0;
    DispersalData dispersalData;

    char* data = inpack.getPackData();
    offset += dispersalData.outlet(result, data + offset);
    offset += dispersalData.outlet(clusterID, data + offset);
    dispersalData.outlet(userID, data + offset);

    Mi::ClusterJoin type = Mi::JoinNone;
    quint16 cmdCode = inpack.getCmdCode();
    if(cmdCode == CommandCode::DCLUSTER_APPLY_APPROVAL){
        type = Mi::ClusterApplyReq;
    }else if(cmdCode == CommandCode::DCLUSTER_INVITE_APPROVAL)
        type = Mi::ClusterInviteReq;

    QString strText;
    Mi::MsgStatus status;
    DClusterJoinNotify* info = this->findNotifyFromNotifyList(type, userID, clusterID);
    if(result == Mi::Agreed && info != NULL){
        strText = QString(tr("已同意"));
        status = Mi::Agreed;
    }else if(result == Mi::Refused && info != NULL){
        strText = QString(tr("已拒绝"));
        status = Mi::Refused;
    }else
        return;

    curCheckButton->setReviewText(strText);
    curCheckButton->setReviewEnable(false);
    info->updateInfoStatus(status);
}

void DClusterNotifyListPage::recvNotifyInfoFromServer(InputPacket&)
{
    //通知数据已在联系人主页面统一处理，并保存到数据库
    this->loadLocalDataSource();
}

void DClusterNotifyListPage::deployMsgNotifyList(QList<DClusterJoinNotify *> list)
{
    for(int i = 0; i < list.count(); i++){
        DClusterJoinNotify* info = list.at(i);
        this->appendNotifyToListWidget(notifyListWidget, info);
    }
}

void DClusterNotifyListPage::appendNotifyToListWidget(ListWidget* listWidget, DClusterJoinNotify *info)
{
    QWidget* widget = this->findNotifyButtonFromListWidget(listWidget, info);
    CheckButton* btn = NULL;

    if(widget != NULL){//已存在，则更新
        btn = dynamic_cast<CheckButton*>(widget);
        this->setNotifyButton(btn, info);
        SAFE_DELETE(info);
    }else{//不存在，则加入
        btn = new CheckButton(this);
        this->setNotifyButton(btn, info);
        this->sortWithNotifyStatus(listWidget, btn);

        int height = curFixedContentHeight + listWidget->height();
        this->setBodyRealHeight(height);
    }
}

void DClusterNotifyListPage::setNotifyButton(CheckButton *btn, DClusterJoinNotify *info)
{
    btn->setUserID(info->getUserID());
    btn->setUserName(info->getUserName());
    btn->setClusterID(info->getClusterID());
    btn->setClusterName(info->getClusterName());
    btn->setCheckType(info->getNotifyType());
    btn->setCheckStatus(info->getMsgStatus());
    btn->setAttribute(Qt::WA_LayoutUsesWidgetRect);
    btn->setStyleSheet(SheetStyle::GROUPBUTTON_BOTTOMBORDER);

    QPixmap pixmap(ImagePath::USER_PORTRAIT);
    pixmap.setDevicePixelRatio(2.0);
    btn->setPixMap(pixmap);
    btn->setFixedHeight(this->screenHeight()*0.1);
    btn->setFixedWidth(this->screenWidth());
    btn->setDescribeInfo(info->getNotifyContent());
    connect(btn, SIGNAL(clicked()), this, SLOT(on_btnReviewPage_clicked()));
    connect(btn, SIGNAL(reviewClicked()), this, SLOT(on_btnReview_clicked()));

    quint8 isReview = info->getMsgStatus();
    if(isReview == Mi::Agreed){
        btn->setReviewText(tr("已同意"));
        btn->setReviewEnable(false);
    }else if(isReview == Mi::Refused){
        btn->setReviewText(tr("已拒绝"));
        btn->setReviewEnable(false);
    }else if(isReview == Mi::Pending){
        btn->setReviewText(tr("同意"));
        btn->setReviewEnable(true);
    }else
        return;
}

void DClusterNotifyListPage::sortWithNotifyStatus(ListWidget* listWidget, CheckButton *hBtn)
{
    if(hBtn->getCheckStatus() == Mi::Pending){
        listWidget->insertWidget(0, hBtn);
    }else
        listWidget->addWidget(hBtn);

    this->listWidgetLastWidgetStyle(listWidget);
}

void DClusterNotifyListPage::displayConcretePage(BasePage *page)
{
    if(page == NULL) return;
    page->show();
    connect(page, SIGNAL(destroyed()), this, SLOT(on_deleteSubPages()));
    connect(page, SIGNAL(readyRead(quint16,char*,quint16)), this, SLOT(on_recvDataFromOtherPage(quint16,char*,quint16)));
}

void DClusterNotifyListPage::listWidgetLastWidgetStyle(ListWidget *listWidget)
{
    for(int i = 0; i < listWidget->count(); i++){
        QWidget* widget = listWidget->itemAt(i);
        widget->setStyleSheet(SheetStyle::GROUPBUTTON_BOTTOMBORDER);
        if(i == (listWidget->count() - 1)){
            widget->setStyleSheet(SheetStyle::GROUPBUTTON_NOBORDER);
        }
    }
}

void DClusterNotifyListPage::transferDataToAnotherPage(InputPacket &inpack, BasePage *page)
{
    if(page == NULL) return;
    page->fetchData(inpack);
}

QWidget* DClusterNotifyListPage::findNotifyButtonFromListWidget(ListWidget *listwidget, DClusterJoinNotify *info)
{
    int count = listwidget->count();
    for(int i = 0; i < count; i++){
        QWidget* widget = listwidget->itemAt(i);
        CheckButton* hBtn = dynamic_cast<CheckButton*>(widget);

        if(hBtn->getUserID() == info->getUserID() && hBtn->getClusterID() == info->getClusterID())
            return widget;
    }
    return NULL;
}

DClusterJoinNotify* DClusterNotifyListPage::findNotifyFromNotifyList(Mi::ClusterJoin type, quint64 userID, quint64 clusterID)
{
    QList<DClusterJoinNotify*> infoList = notifyList->fetchList();
    for(int i = 0; i < infoList.count(); i++){
        DClusterJoinNotify* info = infoList.at(i);
        if(type == info->getNotifyType() && userID == info->getUserID() && clusterID == info->getClusterID())
            return info;
    }

    return NULL;
}

void DClusterNotifyListPage::on_btnReviewPage_clicked()
{
    CheckButton* btnThis = dynamic_cast<CheckButton*>(sender());
    curCheckButton = btnThis;
    quint64 userID = btnThis->getUserID();
    quint64 clusID = btnThis->getClusterID();
    Mi::ClusterJoin type = btnThis->getCheckType();
    DClusterJoinNotify info = *(this->findNotifyFromNotifyList(type, userID, clusID));

    clusterReviewPage = new DClusterReviewPage(this, info);
    this->displayConcretePage(clusterReviewPage);
}

void DClusterNotifyListPage::on_btnReview_clicked()
{
    CheckButton* btnThis = dynamic_cast<CheckButton*>(sender());
    curCheckButton = btnThis;
    quint64 clusterID = btnThis->getClusterID();
    quint64 userID = btnThis->getUserID();

    quint8 ansCode = Mi::Agreed;
    AssemblyData assemblyData;
    assemblyData.append(ansCode);
    assemblyData.append(clusterID);
    assemblyData.append(userID);
    char* data = assemblyData.getAssemblyData();
    quint16 dataLen = assemblyData.getAssemblyDataLen();

    if(btnThis->getCheckType() == Mi::ClusterInviteReq)
        emit readyRead(CommandCode::DCLUSTER_INVITE_APPROVAL, data, dataLen);
    else if(btnThis->getCheckType() == Mi::ClusterApplyReq)
        emit readyRead(CommandCode::DCLUSTER_APPLY_APPROVAL, data, dataLen);
}

void DClusterNotifyListPage::on_btnPrePage_clicked()
{
    this->hide();
    this->deleteLater();
}

void DClusterNotifyListPage::on_deleteSubPages()
{
    clusterReviewPage = NULL;
}

void DClusterNotifyListPage::on_recvDataFromOtherPage(quint16 cmdCode, char *data, quint16 dataLen)
{
    emit readyRead(cmdCode, data, dataLen);
}

DClusterNotifyListPage::~DClusterNotifyListPage()
{
    SAFE_DELETE(notifyList);
}