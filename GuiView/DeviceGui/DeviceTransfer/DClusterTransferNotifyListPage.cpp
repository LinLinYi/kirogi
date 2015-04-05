#include <QVBoxLayout>
#include <QHBoxLayout>

#include "ListWidget.h"
#include "CheckButton.h"
#include "DispersalData.h"
#include "UserProtocol.hpp"
#include "LoadingPage.h"
#include "NavigationBar.h"
#include "HintMsgPage.h"
#include "InputPacket.h"
#include "GlobalDefine.hpp"
#include "ImageConfig.hpp"
#include "AssemblyData.h"
#include "DeviceTransNotifyList.h"
#include "SheetStyleConfig.hpp"
#include "DClusterTransferReviewPage.h"
#include "DeviceTransferInfoList.h"
#include "DClusterTransferNotifyListPage.h"

DClusterTransferNotifyListPage::DClusterTransferNotifyListPage(QWidget* parent)
    :BasePage(parent)
    , curFixedContentHeight(0)
    , notifyListWidget(NULL)
    , transferReviewPage(NULL)
    , notifyList(NULL)
{
    this->initializeWidget();
}

void DClusterTransferNotifyListPage::initializeWidget()
{
    //导航栏
    QString strTitle = tr("设备移交通知");
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

    //通知信息显示布局
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

void DClusterTransferNotifyListPage::fetchData(InputPacket &inpack)
{
    quint16 cmdCode = inpack.getCmdCode();
    switch(cmdCode)
    {
        case CommandCode::DCLUSTER_DEV_TRANSFER_APPROVAL:
            this->recvClusterCheckResultFromServer(inpack);
            break;
        case CommandCode::SERVER_NOTIFY:
            this->recvNotifyInfoFromServer(inpack);
            break;
    }
    this->transferDataToAnotherPage(inpack, transferReviewPage);
}

void DClusterTransferNotifyListPage::loadLocalDataSource()
{
    SAFE_DELETE(notifyList);
    notifyList = new DeviceTransNotifyList;
    notifyList->loadDataSource();
    this->deployMsgNotifyList(notifyList->fetchList());
}

void DClusterTransferNotifyListPage::recvClusterCheckResultFromServer(InputPacket &inpack)
{
    quint8 result = 0;
    quint16 offset = 0;
    quint64 srcUserID = 0;
    QString srcUserName;
    quint64 srcClusterID = 0;
    QString srcClusterName;
    quint64 destClusterID = 0;
    QString destClusterName;
    DispersalData dispersalData;

    char* data = inpack.getPackData();
    offset += dispersalData.outlet(result, data + offset);
    offset += dispersalData.outlet(srcUserID, data + offset);
    offset += dispersalData.outlet(srcUserName, data + offset);
    offset += dispersalData.outlet(srcClusterID, data + offset);
    offset += dispersalData.outlet(srcClusterName, data + offset);
    offset += dispersalData.outlet(destClusterID, data + offset);
    offset += dispersalData.outlet(destClusterName, data + offset);

    QString strText;
    Mi::MsgStatus status;
    DeviceTransNotify* info = this->findNotifyFromNotifyList(srcClusterID, destClusterID);

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
    DeviceTransferInfoList::deleteLocalTransferDevices(srcClusterID, destClusterID);
}

void DClusterTransferNotifyListPage::recvNotifyInfoFromServer(InputPacket&)
{
    //通知数据已在联系人主页面统一处理，并保存到数据库
    this->loadLocalDataSource();
}

void DClusterTransferNotifyListPage::deployMsgNotifyList(QList<DeviceTransNotify *> list)
{
    for(int i = 0; i < list.count(); i++){
        DeviceTransNotify* info = list.at(i);
        this->appendNotifyToListWidget(notifyListWidget, info);
    }
}

void DClusterTransferNotifyListPage::appendNotifyToListWidget(ListWidget* listWidget, DeviceTransNotify *info)
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

void DClusterTransferNotifyListPage::setNotifyButton(CheckButton *btn, DeviceTransNotify *info)
{
    btn->setUserID(info->getSrcUserID());
    btn->setUserName(info->getSrcUserName());
    btn->setClusterID(info->getSrcClusterID());
    btn->setClusterName(info->getSrcClusterName());
    btn->setCheckStatus(info->getReviewStatus());
    btn->setReserveID(info->getDestClusterID());
    btn->setReserveName(info->getDestClusterName());
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

    quint8 isReview = info->getReviewStatus();
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

void DClusterTransferNotifyListPage::sortWithNotifyStatus(ListWidget* listWidget, CheckButton *hBtn)
{
    if(hBtn->getCheckStatus() == Mi::Pending){
        listWidget->insertWidget(0, hBtn);
    }else
        listWidget->addWidget(hBtn);

    this->listWidgetLastWidgetStyle(listWidget);
}

void DClusterTransferNotifyListPage::displayConcretePage(BasePage *page)
{
    if(page == NULL) return;
    page->show();
    connect(page, SIGNAL(destroyed()), this, SLOT(on_deleteSubPages()));
    connect(page, SIGNAL(readyRead(quint16,char*,quint16)), this, SLOT(on_recvDataFromOtherPage(quint16,char*,quint16)));
}

void DClusterTransferNotifyListPage::listWidgetLastWidgetStyle(ListWidget *listWidget)
{
    for(int i = 0; i < listWidget->count(); i++){
        QWidget* widget = listWidget->itemAt(i);
        widget->setStyleSheet(SheetStyle::GROUPBUTTON_BOTTOMBORDER);
        if(i == (listWidget->count() - 1)){
            widget->setStyleSheet(SheetStyle::GROUPBUTTON_NOBORDER);
        }
    }
}

void DClusterTransferNotifyListPage::transferDataToAnotherPage(InputPacket &inpack, BasePage *page)
{
    if(page == NULL) return;
    page->fetchData(inpack);
}

QWidget* DClusterTransferNotifyListPage::findNotifyButtonFromListWidget(ListWidget* listwidget, DeviceTransNotify* info)
{
    int count = listwidget->count();
    for(int i = 0; i < count; i++){
        QWidget* widget = listwidget->itemAt(i);
        CheckButton* hBtn = dynamic_cast<CheckButton*>(widget);

        if(hBtn->getClusterID() == info->getSrcClusterID() && hBtn->getReserveID() == info->getDestClusterID())
            return widget;
    }
    return NULL;
}

DeviceTransNotify* DClusterTransferNotifyListPage::findNotifyFromNotifyList(quint64 srcClusterID, quint64 destClusterID)
{
    QList<DeviceTransNotify*> infoList = notifyList->fetchList();
    for(int i = 0; i < infoList.count(); i++){
        DeviceTransNotify* info = infoList.at(i);
        if(srcClusterID == info->getSrcClusterID() && destClusterID == info->getDestClusterID())
            return info;
    }

    return NULL;
}

void DClusterTransferNotifyListPage::on_btnReviewPage_clicked()
{
//    CheckButton* btnThis = dynamic_cast<CheckButton*>(sender());
//    curCheckButton = btnThis;
//    quint64 srcClusterID = btnThis->getClusterID();
//    quint64 destClusterID = btnThis->getReserveID();

//    transferReviewPage = new DClusterTransferReviewPage(this);
//    this->displayConcretePage(transferReviewPage);
}

void DClusterTransferNotifyListPage::on_btnReview_clicked()
{
    CheckButton* btnThis = dynamic_cast<CheckButton*>(sender());
    curCheckButton = btnThis;
    quint64 srcClusterID = btnThis->getClusterID();
    quint64 destClusterID = btnThis->getReserveID();
    DeviceTransNotify* notifyInfo = this->findNotifyFromNotifyList(srcClusterID, destClusterID);
    if(notifyInfo == NULL) return;

    quint8 ansCode = Mi::Agreed;
    AssemblyData assemblyData;
    assemblyData.append(ansCode);
    assemblyData.append(notifyInfo->getSrcUserID());
    assemblyData.append(notifyInfo->getSrcUserName());
    assemblyData.append(notifyInfo->getSrcClusterID());
    assemblyData.append(notifyInfo->getSrcClusterName());
    assemblyData.append(notifyInfo->getDestClusterID());
    assemblyData.append(notifyInfo->getDestClusterName());

    DeviceTransferInfoList infoList;
    infoList.loadDataSource(srcClusterID, destClusterID);

    QHash<quint64, DeviceTransferInfo*> hashList = infoList.fetchList();
    QHashIterator<quint64, DeviceTransferInfo*> iter(hashList);

    quint16 devCount = hashList.count();
    assemblyData.append(devCount);

    while (iter.hasNext()) {
        iter.next();
        DeviceTransferInfo* info = iter.value();
        quint64 devID = info->getDeviceID();
        assemblyData.append(devID);
    }

    char* data = assemblyData.getAssemblyData();
    quint16 dataLen = assemblyData.getAssemblyDataLen();
    emit readyRead(CommandCode::DCLUSTER_DEV_TRANSFER_APPROVAL, data, dataLen);
}

void DClusterTransferNotifyListPage::on_btnPrePage_clicked()
{
    this->hide();
    this->deleteLater();
}

void DClusterTransferNotifyListPage::on_deleteSubPages()
{
    this->loadLocalDataSource();
    transferReviewPage = NULL;
}

void DClusterTransferNotifyListPage::on_recvDataFromOtherPage(quint16 cmdCode, char *data, quint16 dataLen)
{
    emit readyRead(cmdCode, data, dataLen);
}

DClusterTransferNotifyListPage::~DClusterTransferNotifyListPage()
{
    SAFE_DELETE(notifyList);
}

