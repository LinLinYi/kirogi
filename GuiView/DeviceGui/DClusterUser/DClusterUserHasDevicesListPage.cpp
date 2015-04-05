#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QAction>

#include "MenuBar.h"
#include "GroupButton.h"
#include "GListWidget.h"
#include "UserProtocol.hpp"
#include "DClusterUserInfo.h"
#include "ImageConfig.hpp"
#include "AssemblyData.h"
#include "InputPacket.h"
#include "SheetStyleConfig.hpp"
#include "HintMsgPage.h"
#include "NavigationMenuBar.h"
#include "DClusterUserDetailPage.h"
#include "DClusterDevInfoList.h"
#include "DClusterUserAddDevicePage.h"
#include "DClusterUserDelDevicePage.h"
#include "DClusterAuthDevInfoList.h"
#include "DClusterUserHasDevicesListPage.h"

DClusterUserHasDevicesListPage::DClusterUserHasDevicesListPage(QWidget* parent, quint64 clusterID, quint64 userID)
    : BasePage(parent)
    , groupCount(0)
    , curUserID(userID)
    , curClusterID(clusterID)
    , curFixedContentHeight(0)
    , authorizedDeviceInfoList(NULL)
    , clusterUserAddDevicePage(NULL)
    , clusterUserDelDevicePage(NULL)
    , clusterUserDetailPage(NULL)
{
    this->initializeWidget();
}

void DClusterUserHasDevicesListPage::initializeWidget()
{
    //导航栏
    QString strLeftBtn = tr("上一步");
    QString strTitle = tr("成员所属设备");
    navigationMenuBar = new NavigationMenuBar(this);
    navigationMenuBar->setLeftText(strLeftBtn);
    navigationMenuBar->setTitleText(strTitle);
    connect(navigationMenuBar, SIGNAL(leftClicked()), this, SLOT(on_btnPrepage_clicked()));

    MenuBar* menuBar = navigationMenuBar->getMenuBar();
    addDeviceAction = menuBar->addAction(tr("添加设备"));
    deleteDeviceAction = menuBar->addAction(tr("删除设备"));
    connect(addDeviceAction, SIGNAL(triggered()),this,SLOT(on_addDeviceAction_clicked()));
    connect(deleteDeviceAction, SIGNAL(triggered()), this, SLOT(on_deleteDeviceAction_clicked()));

    QHBoxLayout* pHLTop = new QHBoxLayout();
    pHLTop->addWidget(navigationMenuBar);
    pHLTop->setSpacing(0);
    pHLTop->setMargin(0);
    this->setTopbarLayout(pHLTop);

    //成员信息
    QPixmap pixmapLeft(ImagePath::USER_PORTRAIT);
    QPixmap pixmapRight(ImagePath::RIGHT_HOLLOW_ARROWS);
    btnUserDetail = new GroupButton(this);
    btnUserDetail->setPixMap(pixmapLeft);
    btnUserDetail->setReserve(pixmapRight);
    btnUserDetail->setAttribute(Qt::WA_LayoutUsesWidgetRect);
    btnUserDetail->setFixedHeight(this->screenHeight()*0.12);
    btnUserDetail->setStyleSheet(SheetStyle::GROUPBUTTON_BOTTOMBORDER);
    connect(btnUserDetail, SIGNAL(clicked()), this, SLOT(on_btnUserDetail_clicked()));

    //成员所属设备列表
    devListWidget = new GListWidget(this);
    this->setGListWidget(devListWidget);
    connect(devListWidget, SIGNAL(flexClicked(int)), this, SLOT(on_btnListWidgetFlex_clicked(int)));

    //主体布局
    QVBoxLayout* vbLayout = new QVBoxLayout;
    vbLayout->addWidget(btnUserDetail);
    vbLayout->addSpacing(this->screenHeight()*0.037);
    vbLayout->addWidget(devListWidget);
    vbLayout->setSpacing(0);
    vbLayout->setMargin(0);
    vbLayout->addStretch(0);
    this->setBodyPartLayout(vbLayout);
    curFixedContentHeight += this->screenHeight()*0.037;

    //屏幕触摸滚动设置
    this->installScrollViewportArea();
    this->loadLocalData();
    timerID = this->startTimer(ConstNum::MAX_TIME_INTERVAL);
}

void DClusterUserHasDevicesListPage::loadLocalData()
{
    DClusterUserInfo userInfo;
    userInfo.loadDataSource(curClusterID, curUserID);
    btnUserDetail->setText(userInfo.getUserName());
}

void DClusterUserHasDevicesListPage::fetchData(InputPacket &inpack)
{
    quint16 cmdCode = inpack.getCmdCode();
    switch(cmdCode)
    {
        case CommandCode::FETCH_USER_AUTHORIZED_DEV_LIST:
            this->handleUserAuthorizedDeviceList(inpack);
            break;
        case CommandCode::DCLUSTER_USER_ADD_DEVICE:
            this->recvDeviceInfoFromServer(inpack);
            break;
        case CommandCode::DCLUSTER_USER_DELETE_DEVICE:
            this->handleDelDeviceInfo(inpack);
            break;
    }
    this->transferDataToAnotherPage(inpack, clusterUserAddDevicePage);
    this->transferDataToAnotherPage(inpack, clusterUserDelDevicePage);
    this->transferDataToAnotherPage(inpack, clusterUserDetailPage);
}

void DClusterUserHasDevicesListPage::timerEvent(QTimerEvent*)
{
    this->fetchAuthorizedDeviceCmd();
    this->killTimer(timerID);
}

void DClusterUserHasDevicesListPage::fetchAuthorizedDeviceCmd()
{
    AssemblyData assemblyData;
    assemblyData.append(curClusterID);
    assemblyData.append(curUserID);
    char* data = assemblyData.getAssemblyData();
    quint16 dataLen = assemblyData.getAssemblyDataLen();
    emit readyRead(CommandCode::FETCH_USER_AUTHORIZED_DEV_LIST, data, dataLen);
}

void DClusterUserHasDevicesListPage::handleUserAuthorizedDeviceList(InputPacket &inpack)
{
    //处理网络信息数据
    char* data = inpack.getPackData();
    quint16 dataLen = inpack.getPackDataLen();
    if(clusterUserAddDevicePage != NULL || clusterUserDelDevicePage != NULL) return;

    //处理已授权设备列表
    if(authorizedDeviceInfoList == NULL) {
        authorizedDeviceInfoList = new DClusterAuthDevInfoList;
    }
    authorizedDeviceInfoList->appendList(data, dataLen);

    //显示设备列表
    QHash<quint64, DClusterAuthDevInfo*> userList = authorizedDeviceInfoList->fetchList();
    QHashIterator<quint64, DClusterAuthDevInfo*> iter(userList);

    while(iter.hasNext())
    {
        iter.next();
        DClusterAuthDevInfo* deviceInfo = dynamic_cast<DClusterAuthDevInfo*>(iter.value());
        for(int i = 0; i < (int)devListWidget->itemCount(); i++){
            QWidget* widget = devListWidget->itemAt(i);
            GroupButton* devBtn = dynamic_cast<GroupButton*>(widget);
            if(devBtn->getIndexOne() == deviceInfo->getDeviceID()) {//已授权设备已存在，不添加
                deviceInfo = NULL;
                break;
            }
        }

        if(deviceInfo){//已授权设备不存在，添加
            this->setDeviceWidget(deviceInfo, devListWidget);
        }
    }
}

void DClusterUserHasDevicesListPage::recvDeviceInfoFromServer(InputPacket &inpack)
{
    quint8 ansCode = inpack.getAnswerCode();
    if(ansCode == Mi::Success){
        this->fetchAuthorizedDeviceCmd();
    }
}

void DClusterUserHasDevicesListPage::handleDelDeviceInfo(InputPacket &inpack)
{
    quint8 ansCode = inpack.getAnswerCode();
    if(ansCode == Mi::Success){
        QList<quint64> deledDevList = clusterUserDelDevicePage->getDelDevList();
        this->delAuthorizedDeviceWidget(deledDevList);
    }
}

void DClusterUserHasDevicesListPage::setGListWidget(GListWidget *listWidget)
{
    QPixmap pixmap(ImagePath::DOWNWARD_SOLID_ARROWS);
    pixmap.setDevicePixelRatio(2.0);

    listWidget->setPixmap(pixmap);
    listWidget->setListVisible(true);
    listWidget->setTitle(tr("所属设备"));
    listWidget->setTitleHeight(this->screenHeight()*0.08);
    listWidget->setContentsMargins(this->screenWidth()*0.03,0,0,0);
    listWidget->setTitleStyleSheet(SheetStyle::GROUPBUTTON_TOPBORDER);
}

void DClusterUserHasDevicesListPage::setDeviceWidget(DClusterAuthDevInfo *devInfo, GListWidget* listWidget)
{
    if(devInfo == NULL || listWidget == NULL) return;
    QPixmap pixmap(ImagePath::DEVICE_PORTRAIT);
    pixmap.setDevicePixelRatio(2.0);

    //用户按钮设置
    GroupButton* devBtn = new GroupButton(this);
    QString title = devInfo->getDeviceName();
    devBtn->setIndexOne(devInfo->getDeviceID());
    devBtn->setText(title);
    devBtn->setPixMap(pixmap);
    devBtn->setFixedHeight(this->screenHeight()*0.1);
    devBtn->setContentsMargins(0,0,0,0);
    devBtn->setAttribute(Qt::WA_LayoutUsesWidgetRect);
    devBtn->setStyleSheet(SheetStyle::GROUPBUTTON_TOPBORDER);

    listWidget->addWidget(devBtn);//将用户加到相应的组
    if(listWidget->isListVisible()){
        qint16 height = curFixedContentHeight + devListWidget->height();
        this->setBodyRealHeight(height);
    }
}

void DClusterUserHasDevicesListPage::transferDataToAnotherPage( InputPacket &inpack, BasePage *basePage)
{
    if(basePage == NULL) return;
    basePage->fetchData(inpack);
}

void DClusterUserHasDevicesListPage::on_btnUserDetail_clicked()
{
    clusterUserDetailPage  = new DClusterUserDetailPage(this, curClusterID, curUserID);
    this->displayConcretePage(clusterUserDetailPage);
}

void DClusterUserHasDevicesListPage::on_addDeviceAction_clicked()
{
    clusterUserAddDevicePage = new DClusterUserAddDevicePage(this, curClusterID, curUserID);
    this->displayConcretePage(clusterUserAddDevicePage);
}

void DClusterUserHasDevicesListPage::on_deleteDeviceAction_clicked()
{
    clusterUserDelDevicePage = new DClusterUserDelDevicePage(this, curClusterID, curUserID);
    this->displayConcretePage(clusterUserDelDevicePage);
}

void DClusterUserHasDevicesListPage::on_btnListWidgetFlex_clicked(int listHeight)
{
    QPixmap pixmap;
    GListWidget *listWidget = dynamic_cast<GListWidget *>(sender());

    if(listHeight <= 0){
        pixmap.load(ImagePath::RIGHT_SOLID_ARROWS);
        listWidget->setPixmap(pixmap);
    }
    else{
        pixmap.load(ImagePath::DOWNWARD_SOLID_ARROWS);
        listWidget->setPixmap(pixmap);
    }

    int curHeight = curFixedContentHeight + devListWidget->height();
    if(curHeight <= this->scrollAreaNoBottomBarHeight())
        this->setBodyRealHeight(this->scrollAreaNoBottomBarHeight());
    else
        this->setBodyRealHeight(curHeight);
}

void DClusterUserHasDevicesListPage::on_deleteSubPages()
{
    clusterUserDetailPage = NULL;
    clusterUserAddDevicePage = NULL;
    clusterUserDelDevicePage = NULL;
    this->fetchAuthorizedDeviceCmd();
}

void DClusterUserHasDevicesListPage::on_recvDataFromOtherPage(quint16 cmdCode, char *data, quint16 dataLen)
{
    emit readyRead(cmdCode, data, dataLen);
}

void DClusterUserHasDevicesListPage::on_btnPrepage_clicked()
{
    this->hide();
    this->deleteLater();
}

void DClusterUserHasDevicesListPage::delAuthorizedDeviceWidget(QList<quint64> &delList)
{
    if(delList.count() == 0) return;
    QHash<quint64, DClusterAuthDevInfo*> devList = authorizedDeviceInfoList->fetchList();
    QHashIterator<quint64, DClusterAuthDevInfo*> iter(devList);
    while(iter.hasNext())
    {
        iter.next();
        DClusterAuthDevInfo* info = iter.value();
        for(int i = 0; i < delList.count(); i++)
        {
            if(info->getDeviceID() == delList.at(i))
            {
                delDeviceWidget(info->getDeviceID());
                authorizedDeviceInfoList->remove(info->getDeviceID());
                break;
            }
        }
    }
}

void DClusterUserHasDevicesListPage::delDeviceWidget(quint64 devID)
{
    int count = devListWidget->itemCount();
    for(int i = 0; i < count; i++)
    {
        QWidget* widget = devListWidget->itemAt(i);
        GroupButton* btn = dynamic_cast<GroupButton*>(widget);
        quint64 deviceID = btn->getIndexOne();

        if(deviceID == devID){
            devListWidget->takeAt(i);
            qint16 height = curFixedContentHeight + devListWidget->height();
            this->setBodyRealHeight(height);
            return;
        }
    }
}

void DClusterUserHasDevicesListPage::displayConcretePage(BasePage *page)
{
    if(page == NULL) return;
    page->show();
    connect(page, SIGNAL(destroyed()), this, SLOT(on_deleteSubPages()));
    connect(page, SIGNAL(readyRead(quint16,char*,quint16)), this, SLOT(on_recvDataFromOtherPage(quint16,char*,quint16)));
}

DClusterUserHasDevicesListPage::~DClusterUserHasDevicesListPage()
{
    SAFE_DELETE(authorizedDeviceInfoList);
}


