#include <QVBoxLayout>

#include "InputPacket.h"
#include "AssemblyData.h"
#include "UserProtocol.hpp"
#include "NavigationBar.h"
#include "ImageConfig.hpp"
#include "ListWidget.h"
#include "GroupButton.h"
#include "NavigationBar.h"
#include "DClusterInfo.h"
#include "SheetStyleConfig.hpp"
#include "OnlineStatusInfoList.h"
#include "DClusterDevInfoList.h"
#include "DClusterDeviceHasUserListPage.h"
#include "DClusterDeviceListPage.h"

DClusterDeviceListPage::DClusterDeviceListPage(QWidget* parent, quint64 clusterID)
    :BasePage(parent)
    , curClusterID(clusterID)
    , deviceInfoList(NULL)
    , clusterDeviceHasUserListPage(NULL)
{
    curFixedContentHeight = 0;
    this->initializeWidget();
}

void DClusterDeviceListPage::initializeWidget()
{
    //导航栏
    QString strLeftBtn = tr("上一步");
    QString strTitle = tr("设备列表");
    navigationBar = new NavigationBar(this);
    navigationBar->setLeftText(strLeftBtn);
    navigationBar->setTitleText(strTitle);
    connect(navigationBar, SIGNAL(leftClicked()), this, SLOT(on_btnPrepage_clicked()));

    QHBoxLayout* pHLTop = new QHBoxLayout();
    pHLTop->addWidget(navigationBar);
    pHLTop->setSpacing(0);
    pHLTop->setMargin(0);
    this->setTopbarLayout(pHLTop);

    //搜索按钮
    QPixmap leftArrowsPixmap(ImagePath::BLACK_SEARCH_BAR);
    leftArrowsPixmap.setDevicePixelRatio(2.0);
    btnSearch = new QPushButton(this);
    btnSearch->setIcon(leftArrowsPixmap);
    btnSearch->setText(tr("搜索"));
    btnSearch->setFixedHeight(this->screenHeight()*0.05);
    btnSearch->setStyleSheet(SheetStyle::SEARCH_BUTTON);
    connect(btnSearch, SIGNAL(clicked()), this, SLOT(on_btnSearch_clicked()));

    QVBoxLayout* vbSearchLayout = new QVBoxLayout;
    vbSearchLayout->addWidget(btnSearch);
    vbSearchLayout->setContentsMargins(this->screenHeight()*0.016, this->screenHeight()*0.014,
                                       this->screenHeight()*0.016, this->screenHeight()*0.014);
    curFixedContentHeight += this->screenHeight()*0.05 + this->screenHeight()*0.014 + this->screenHeight()*0.014;

    //群设备布局
    devListWidget = new ListWidget(this);
    devListWidget->setStyleSheet(SheetStyle::LISTWIDGET);
    devListWidget->setContentsMargins(this->screenWidth()*0.03,0,0,0);
    curFixedContentHeight += devListWidget->height();

    QVBoxLayout* vblTotalLayout = new QVBoxLayout();
    vblTotalLayout->addLayout(vbSearchLayout);
    vblTotalLayout->addWidget(devListWidget);
    vblTotalLayout->addStretch(0);
    vblTotalLayout->setMargin(0);
    vblTotalLayout->setSpacing(0);
    vblTotalLayout->setAlignment(Qt::AlignTop);
    this->setBodyPartLayout(vblTotalLayout);

    //初始化页面数据
    deviceInfoList = new DClusterDevInfoList(curClusterID);
    deviceInfoList->loadDataSource();
    this->deployDeviceList(deviceInfoList->fetchList());

    //屏幕触摸滚动设置
    this->installScrollViewportArea();
    this->setBodyRealHeight(curFixedContentHeight);
    timerID = this->startTimer(ConstNum::MAX_TIME_INTERVAL);
}

void DClusterDeviceListPage::fetchData(InputPacket &inpack)
{
    quint16 cmdCode = inpack.getCmdCode();
    DPRINT("-->DevClusterDeviceListPage receive data, the cmdCode is:0x%x", cmdCode);

    switch(cmdCode)
    {
        case CommandCode::FETCH_DCLUSTER_DEVICE_LIST:
            this->handleDevClusterDeviceListInfo(inpack);
            break;
        case CommandCode::FETCH_DCLUSTER_DEV_ONLINE_LIST:
            this->handleDevClusterDeviceOnlineList(inpack);
            break;
    }
    this->transferDataToAnotherPage(inpack, clusterDeviceHasUserListPage);
}

void DClusterDeviceListPage::timerEvent(QTimerEvent*)
{
    AssemblyData assemblyData;
    assemblyData.append(curClusterID);
    char* data = assemblyData.getAssemblyData();
    quint16 dataLen = assemblyData.getAssemblyDataLen();
    emit readyRead(CommandCode::FETCH_DCLUSTER_DEVICE_LIST, data, dataLen);
    this->killTimer(timerID);
}

void DClusterDeviceListPage::handleDevClusterDeviceListInfo(InputPacket& inpack)
{
    SAFE_DELETE(deviceInfoList);
    deviceInfoList = new DClusterDevInfoList;

    char* data = inpack.getPackData();
    quint16 dataLen = inpack.getPackDataLen();
    deviceInfoList->appendList(data, dataLen);
    QHash<quint64, DClusterDevInfo*> devList = deviceInfoList->fetchList();
    this->deployDeviceList(devList);
    this->listWidgetLastWidgetStyle(devListWidget);

    AssemblyData assemblyData;
    assemblyData.append(curClusterID);
    char* buf = assemblyData.getAssemblyData();
    quint16 len = assemblyData.getAssemblyDataLen();
    emit readyRead(CommandCode::FETCH_DCLUSTER_DEV_ONLINE_LIST, buf, len);
}

void DClusterDeviceListPage::handleDevClusterDeviceOnlineList(InputPacket& inpack)
{
    char* data = inpack.getPackData();
    quint16 dataLen = inpack.getPackDataLen();

    OnlineStatusInfoList onlineInfoList;
    onlineInfoList.appendList(data, dataLen, true);

    QHash<quint64, OnlineStatusInfo*> onlineList = onlineInfoList.fetchList();
    QHash<quint64, OnlineStatusInfo*>::iterator onLineIter;

    for(onLineIter = onlineList.begin(); onLineIter != onlineList.end(); ++onLineIter){
        this->changeDeviceOnlineStatus(onLineIter.value());
    }
}

void DClusterDeviceListPage::changeDeviceOnlineStatus(OnlineStatusInfo *onlineInfo)
{
    QString status = NetLine::GetNetStatus(onlineInfo->getStatus());
    for(int i = 0; i < devListWidget->count(); i++){
        QWidget* firstWdg = devListWidget->itemAt(i);
        GroupButton* btn = dynamic_cast<GroupButton*>(firstWdg);
        quint64 devID = btn->getIndexOne();

        if(onlineInfo->getID() == devID){
            btn->setDescribe(status);

            if(onlineInfo->getStatus() == Mi::OnLine)
            {
                devListWidget->removeWidget(firstWdg);
                devListWidget->insertWidget(0, firstWdg);
                this->listWidgetLastWidgetStyle(devListWidget);
            }
        }
    }
}

void DClusterDeviceListPage::deployDeviceList(QHash<quint64, DClusterDevInfo *> list)
{
    QHashIterator<quint64, DClusterDevInfo*> iter(list);
    while(iter.hasNext())
    {
        iter.next();
        DClusterDevInfo* devInfo = dynamic_cast<DClusterDevInfo*>(iter.value());
        this->appendDeviceToListWidget(devInfo);
    }

    qint16 height = curFixedContentHeight + devListWidget->height();
    this->setBodyRealHeight(height);
}

void DClusterDeviceListPage::appendDeviceToListWidget(DClusterDevInfo *info)
{
    GroupButton* btn = NULL;
    for(int i = 0; i < devListWidget->count(); i++){
        QWidget* widget = devListWidget->itemAt(i);
        btn = dynamic_cast<GroupButton*>(widget);

        if(btn->getIndexOne() == info->getDeviceID()){//存在，则更新
            this->setDeviceButton(btn, info);
            return;
        }
    }

    //不存在，则加入
    btn = new GroupButton(this);
    this->setDeviceButton(btn, info);
    devListWidget->addWidget(btn);

    qint16 height = curFixedContentHeight + devListWidget->height();
    this->setBodyRealHeight(height);
}

void DClusterDeviceListPage::setDeviceButton(GroupButton* btn, DClusterDevInfo* info)
{
    if(btn == NULL || info == NULL) return;

    //设备按钮设置
    QPixmap pixmap(ImagePath::DEVICE_PORTRAIT);
    pixmap.setDevicePixelRatio(2.0);
    btn->setIndexOne(info->getDeviceID());
    btn->setText(info->getDeviceName());
    btn->setPixMap(pixmap);
    btn->setDescribe(QString(tr("[离线]")));
    btn->setFixedHeight(this->screenHeight()*0.1);
    btn->setFixedWidth(this->screenWidth());
    btn->setAttribute(Qt::WA_LayoutUsesWidgetRect);
    btn->setContentsMargins(0,0,0,0);
    btn->setStyleSheet(SheetStyle::GROUPBUTTON_BOTTOMBORDER);
    connect(btn, SIGNAL(clicked()), this, SLOT(on_btnDevClusterDeviceInfo_clicked()));
}

void DClusterDeviceListPage::displayConcretePage(BasePage *page)
{
    if(page == NULL) return;
    page->show();
    connect(page, SIGNAL(destroyed()), this, SLOT(on_deleteSubPages()));
    connect(page, SIGNAL(readyRead(quint16,char*,quint16)), this, SLOT(on_recvDataFromOtherPage(quint16,char*,quint16)));
}

void DClusterDeviceListPage::listWidgetLastWidgetStyle(ListWidget *listWidget)
{
    for(int i = 0; i < listWidget->count(); i++){
        QWidget* widget = listWidget->itemAt(i);
        widget->setStyleSheet(SheetStyle::GROUPBUTTON_BOTTOMBORDER);
        if(i == (listWidget->count() - 1)){
            widget->setStyleSheet(SheetStyle::GROUPBUTTON_NOBORDER);
        }
    }
}

void DClusterDeviceListPage::transferDataToAnotherPage(InputPacket &inpack, BasePage *basePage)
{
    if(basePage == NULL) return;
    basePage->fetchData(inpack);
}

void DClusterDeviceListPage::on_btnSearch_clicked()
{

}

void DClusterDeviceListPage::on_btnDevClusterDeviceInfo_clicked()
{
    GroupButton* btn = dynamic_cast<GroupButton*>(sender());
    quint64 devID = btn->getIndexOne();

    SAFE_DELETE(clusterDeviceHasUserListPage);
    clusterDeviceHasUserListPage = new DClusterDeviceHasUserListPage(this, curClusterID, devID);
    this->displayConcretePage(clusterDeviceHasUserListPage);
}

void DClusterDeviceListPage::on_recvDataFromOtherPage(quint16 cmdCode, char *data, quint16 dataLen)
{
    if(cmdCode == CommandCode::BACK_CLUSTER_MANAGE_PAGE){
        this->hide();
        this->deleteLater();
        return;
    }
    emit readyRead(cmdCode, data, dataLen);
}

void DClusterDeviceListPage::on_btnPrepage_clicked()
{
    this->hide();
    this->deleteLater();
}

void DClusterDeviceListPage::on_deleteSubPages()
{
    clusterDeviceHasUserListPage = NULL;
}

DClusterDeviceListPage::~DClusterDeviceListPage()
{
    SAFE_DELETE(deviceInfoList);
}
