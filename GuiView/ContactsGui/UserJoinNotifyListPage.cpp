#include <QVBoxLayout>
#include <QHBoxLayout>

#include "CheckButton.h"
#include "ListWidget.h"
#include "UserReviewPage.h"
#include "UserJoinNotifyList.h"
#include "UserProtocol.hpp"
#include "ServerNotifyInfo.h"
#include "NavigationBar.h"
#include "ImageConfig.hpp"
#include "AssemblyData.h"
#include "InputPacket.h"
#include "DispersalData.h"
#include "SheetStyleConfig.hpp"
#include "UserJoinNotifyListPage.h"
#include "UserJoinNotifyDataSource.h"

UserJoinNotifyListPage::UserJoinNotifyListPage(QWidget* parent)
    :BasePage(parent)
    , curFixedContentHeight(0)
    , notifyListWidget(NULL)
    , userReviewPage(NULL)
    , notifyList(NULL)
{
    this->initializeWidget();
}

void UserJoinNotifyListPage::initializeWidget()
{
    //导航栏
    QString strTitle = tr("好友申请");
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

void UserJoinNotifyListPage::loadLocalDataSource()
{
    SAFE_DELETE(notifyList);
    notifyList = new UserJoinNotifyList;
    notifyList->loadDataSource();
    this->deployMsgNotifyList(notifyList->fetchList());
}

void UserJoinNotifyListPage::fetchData(InputPacket &inpack)
{
    quint16 cmdCode = inpack.getCmdCode();
    switch(cmdCode)
    {
        case CommandCode::USER_JOIN_APPROVAL:
            this->recvUserCheckResultFromServer(inpack);
            break;
        case CommandCode::SERVER_NOTIFY:
            this->recvNotifyInfoFromServer(inpack);
            break;
    }
    this->transferDataToAnotherPage(inpack, userReviewPage);
}

void UserJoinNotifyListPage::recvUserCheckResultFromServer(InputPacket &inpack)
{
    quint16 offset = 0;
    quint8 result = 0;
    quint64 userID = 0;
    DispersalData dispersalData;

    char* data = inpack.getPackData();
    offset += dispersalData.outlet(result, data + offset);
    dispersalData.outlet(userID, data + offset);

    QString strText;
    Mi::MsgStatus status;
    UserJoinNotify* info = this->findNotifyFromNotifyList(notifyList, userID);
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

void UserJoinNotifyListPage::recvNotifyInfoFromServer(InputPacket&)
{
    //通知数据已在联系人主页面统一处理，并保存到数据库
    this->loadLocalDataSource();
}

void UserJoinNotifyListPage::handleUserApproval(InputPacket& inpack)
{
    char* data = inpack.getPackData();
    quint16 dataLen = inpack.getPackDataLen();

    if(data ==NULL || dataLen == 0) return;
    UserJoinNotify* info = new UserJoinNotify;
    info->unpack(data, Mi::UserJoinApproval);
    notifyList->append(info);
    this->appendNotifyToListWidget(notifyListWidget, info);
}

void UserJoinNotifyListPage::deployMsgNotifyList(QHash<quint64, UserJoinNotify *> list)
{
    QHashIterator<quint64, UserJoinNotify*> iter(list);
    while(iter.hasNext()){
        iter.next();
        UserJoinNotify* info = dynamic_cast<UserJoinNotify*>(iter.value());
        this->appendNotifyToListWidget(notifyListWidget, info);
    }
}

void UserJoinNotifyListPage::appendNotifyToListWidget(ListWidget* listWidget, UserJoinNotify *info)
{
    QWidget* widget = this->findNotifyButtonFromListWidget(listWidget, info->getUserID());
    CheckButton* btn = NULL;

    if(widget != NULL){//已存在，则更新
        btn = dynamic_cast<CheckButton*>(widget);
        this->setNotifyButton(btn, info);
    }else{//不存在，则加入
        btn = new CheckButton(this);
        this->setNotifyButton(btn, info);
        this->sortWithNotifyStatus(listWidget, btn);

        int height = curFixedContentHeight + listWidget->height();
        this->setBodyRealHeight(height);
    }
}

void UserJoinNotifyListPage::setNotifyButton(CheckButton *btn, UserJoinNotify *info)
{
    quint64 usrID = info->getUserID();
    btn->setIndexOne(usrID);
    btn->setUserID(usrID);
    btn->setUserName(info->getUserName());
    btn->setCheckStatus(info->getMsgStatus());
    btn->setAttribute(Qt::WA_LayoutUsesWidgetRect);
    btn->setStyleSheet(SheetStyle::GROUPBUTTON_BOTTOMBORDER);

    QPixmap pixmap(ImagePath::USER_PORTRAIT);
    pixmap.setDevicePixelRatio(2.0);
    btn->setPixMap(pixmap);
    btn->setFixedHeight(this->screenHeight()*0.1);
    btn->setFixedWidth(this->screenWidth());
    btn->setDescribeInfo(info->getNotifyContent());
    btn->setStyleSheet(SheetStyle::CHECKBUTTON_BOTTOM_BORDER);
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

void UserJoinNotifyListPage::sortWithNotifyStatus(ListWidget* listWidget, CheckButton *hBtn)
{
    if(hBtn->getCheckStatus() == Mi::Pending){
        listWidget->insertWidget(0, hBtn);
    }else
        listWidget->addWidget(hBtn);

    this->listWidgetLastWidgetStyle(listWidget);
}

UserJoinNotify* UserJoinNotifyListPage::findNotifyFromNotifyList(UserJoinNotifyList* infoList, quint64 userID)
{
    QHash<quint64, UserJoinNotify*> list = infoList->fetchList();
    UserJoinNotify* info = list.value(userID);
    return info;
}

QWidget* UserJoinNotifyListPage::findNotifyButtonFromListWidget(ListWidget *listwidget, quint64 userID)
{
    int count = listwidget->count();
    for(int i = 0; i < count; i++){
        QWidget* widget = listwidget->itemAt(i);
        CheckButton* hBtn = dynamic_cast<CheckButton*>(widget);

        if(hBtn->getIndexOne() == userID)
            return widget;
    }
    return NULL;
}

void UserJoinNotifyListPage::displayConcretePage(BasePage *page)
{
    if(page == NULL) return;
    page->show();
    connect(page, SIGNAL(destroyed()), this, SLOT(on_deleteSubPages()));
    connect(page, SIGNAL(readyRead(quint16,char*,quint16)), this, SLOT(on_recvDataFromOtherPage(quint16,char*,quint16)));
}

void UserJoinNotifyListPage::listWidgetLastWidgetStyle(ListWidget *listWidget)
{
    for(int i = 0; i < listWidget->count(); i++){
        QWidget* widget = listWidget->itemAt(i);
        widget->setStyleSheet(SheetStyle::GROUPBUTTON_BOTTOMBORDER);
        if(i == (listWidget->count() - 1)){
            widget->setStyleSheet(SheetStyle::GROUPBUTTON_NOBORDER);
        }
    }
}

void UserJoinNotifyListPage::transferDataToAnotherPage(InputPacket &inpack, BasePage *page)
{
    if(page == NULL) return;
    page->fetchData(inpack);
}

void UserJoinNotifyListPage::on_btnReviewPage_clicked()
{
    CheckButton* btnThis = dynamic_cast<CheckButton*>(sender());
    curCheckButton = btnThis;
    quint64 userID = btnThis->getUserID();
    UserJoinNotify info = *(this->findNotifyFromNotifyList(notifyList, userID));

    SAFE_DELETE(userReviewPage);
    userReviewPage = new UserReviewPage(this, info);
    this->displayConcretePage(userReviewPage);
}

void UserJoinNotifyListPage::on_btnReview_clicked()
{
    CheckButton* btnThis = dynamic_cast<CheckButton*>(sender());
    curCheckButton = btnThis;
    quint64 userID = btnThis->getUserID();

    quint8 ansCode = Mi::Agreed;
    AssemblyData assemblyData;
    assemblyData.append(ansCode);
    assemblyData.append(userID);
    char* data = assemblyData.getAssemblyData();
    quint16 dataLen = assemblyData.getAssemblyDataLen();
    emit readyRead(CommandCode::USER_JOIN_APPROVAL, data, dataLen);
}

void UserJoinNotifyListPage::on_btnPrePage_clicked()
{
    this->hide();
    this->deleteLater();
}

void UserJoinNotifyListPage::on_deleteSubPages()
{
    userReviewPage = NULL;
}

void UserJoinNotifyListPage::on_recvDataFromOtherPage(quint16 cmdCode, char *data, quint16 dataLen)
{
    emit readyRead(cmdCode, data, dataLen);
}

UserJoinNotifyListPage::~UserJoinNotifyListPage()
{
    SAFE_DELETE(notifyList);
}


