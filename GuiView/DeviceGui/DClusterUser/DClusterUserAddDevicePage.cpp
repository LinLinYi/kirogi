#include <QVBoxLayout>
#include <QHBoxLayout>

#include "AssemblyData.h"
#include "GlobalDefine.hpp"
#include "InputPacket.h"
#include "UserProtocol.hpp"
#include "HintMsgPage.h"
#include "NavigationBar.h"
#include "NavigationBar.h"
#include "ImageConfig.hpp"
#include "GroupButton.h"
#include "LoadingPage.h"
#include "DClusterDevInfoList.h"
#include "SheetStyleConfig.hpp"
#include "DClusterAuthDevInfoList.h"
#include "DClusterUserAddDevicePage.h"

DClusterUserAddDevicePage::DClusterUserAddDevicePage(QWidget* parent, quint64 clusterID, quint64 userID)
    : BasePage(parent)
    , curClusterID(clusterID)
    , curUserID(userID)
    , curFixedContentHeight(0)
    , deviceAddCount(0)
    , loadingPage(NULL)
    , deviceList(NULL)
{
    initializeWidget();
}

void DClusterUserAddDevicePage::initializeWidget()
{
    //加提示信息页面操作
    hintMsgPage = new HintMsgPage(this);
    hintMsgPage->hide();
    connect(hintMsgPage, SIGNAL(submitClicked(bool)), this, SLOT(on_hintMsgPage(bool)));

    //导航栏
    QString strTitle = tr("添加设备");
    QString strLeftBtn = tr("返回");
    QString strRightBtn = tr("确认");
    QPixmap leftArrowsPixmap(ImagePath::NAVIGATION_LEFT_ARROWS);
    navigationBar = new NavigationBar(this);
    navigationBar->setTitleText(strTitle);
    navigationBar->setLeftText(strLeftBtn);
    navigationBar->setRightText(strRightBtn);
    navigationBar->setLeftPixMap(leftArrowsPixmap);
    connect(navigationBar, SIGNAL(leftClicked()), this, SLOT(on_btnPrePage_clicked()));
    connect(navigationBar, SIGNAL(rightClicked()), this, SLOT(on_addDevice_clicked()));

    QHBoxLayout* pHLTop = new QHBoxLayout();
    pHLTop->addWidget(navigationBar);
    pHLTop->setSpacing(0);
    pHLTop->setMargin(0);
    this->setTopbarLayout(pHLTop);

    //群成员布局
    deviceListLayout = new QVBoxLayout();
    deviceListLayout->setMargin(0);
    deviceListLayout->setSpacing(0);
    deviceListLayout->setContentsMargins(0, 0, 0, 0);

    QVBoxLayout* vblTotalLayout = new QVBoxLayout();
    vblTotalLayout->addLayout(deviceListLayout);
    vblTotalLayout->addStretch(0);
    vblTotalLayout->setMargin(0);
    vblTotalLayout->setSpacing(0);
    vblTotalLayout->setAlignment(Qt::AlignTop);
    this->setBodyPartLayout(vblTotalLayout);

    //屏幕触摸滚动设置
    this->installScrollViewportArea();
    timerID = this->startTimer(ConstNum::MAX_TIME_INTERVAL);
}

void DClusterUserAddDevicePage::fetchData(InputPacket &inpack)
{
    quint16 cmdCode = inpack.getCmdCode();
    DPRINT("-->DevClusterUserAddDevicePage receive data, the cmdCode is:0x%x", cmdCode);

    switch(cmdCode)
    {
        case CommandCode::NET_ERROR:
            handleNetError();
            break;
        case CommandCode::DCLUSTER_USER_ADD_DEVICE:
            recvDeviceInfoFromServer(inpack);
            break;
        case CommandCode::FETCH_DCLUSTER_DEVICE_LIST:
            handleDevClusterDeviceList(inpack);
            break;
        case CommandCode::FETCH_USER_AUTHORIZED_DEV_LIST:
            handleAuthorizedDeviceList(inpack);
            break;
    }
}

void DClusterUserAddDevicePage::timerEvent(QTimerEvent*)
{
    AssemblyData assemblyData;
    assemblyData.append(curClusterID);
    char* data = assemblyData.getAssemblyData();
    quint16 dataLen = assemblyData.getAssemblyDataLen();
    emit readyRead(CommandCode::FETCH_DCLUSTER_DEVICE_LIST, data, dataLen);
    this->killTimer(timerID);
}

void DClusterUserAddDevicePage::handleNetError()
{
    if(loadingPage == NULL) return;
    for(int i = 0; i < devIdList.count(); i++)
        devIdList.removeAt(i);

    QString lblText = tr("服务器连接错误, 请重试");
    this->handleHintInfo(lblText);
}


void DClusterUserAddDevicePage::handleHintInfo(QString& text)
{
    hintMsgPage->setText(text);
    hintMsgPage->show();
    SAFE_DELETE(loadingPage);
}

void DClusterUserAddDevicePage::handleDevClusterDeviceList(InputPacket &inpack)
{
    char* data = inpack.getPackData();
    quint16 dataLen = inpack.getPackDataLen();

    SAFE_DELETE(deviceList);
    deviceList = new DClusterDevInfoList;
    deviceList->appendList(data, dataLen);

    //获取已授权设备列表
    AssemblyData assemblyData;
    assemblyData.append(curClusterID);
    assemblyData.append(curUserID);
    char* sendBuf = assemblyData.getAssemblyData();
    quint16 sendBufLen = assemblyData.getAssemblyDataLen();
    emit readyRead(CommandCode::FETCH_USER_AUTHORIZED_DEV_LIST, sendBuf, sendBufLen);
}

void DClusterUserAddDevicePage::handleAuthorizedDeviceList(InputPacket &inpack)
{
    //已授权设备列表
    char* data = inpack.getPackData();
    quint16 dataLen = inpack.getPackDataLen();
    DClusterAuthDevInfoList authorizedList;
    authorizedList.appendList(data, dataLen);
    QHash<quint64, DClusterAuthDevInfo*> authList = authorizedList.fetchList();
    QHashIterator<quint64, DClusterAuthDevInfo*> iterAuth(authList);

    //群设备列表
    QHash<quint64, DClusterDevInfo*> devList = deviceList->fetchList();
    QHashIterator<quint64, DClusterDevInfo*> iterCluster(devList);

    //未授权设备列表
    while(iterCluster.hasNext())
    {
        iterCluster.next();
        bool flag = true;
        quint64 clusDevID = iterCluster.value()->getDeviceID();
        if(authList.count() > 0){
            while(iterAuth.hasNext())
            {
                iterAuth.next();
                DClusterAuthDevInfo* authDevInfo = dynamic_cast<DClusterAuthDevInfo*>(iterAuth.value());
                quint64 authDevID = authDevInfo->getDeviceID();
                if(authDevID == clusDevID){
                    flag = false;
                    deviceList->remove(authDevID);
                    break;
                }
                else if(!iterAuth.hasNext())
                    flag = true;
            }
            iterAuth.toFront();
        }

        if(flag){
            DClusterDevInfo* devInfo = dynamic_cast<DClusterDevInfo*>(iterCluster.value());
            setDeviceButton(devInfo);

            if(curFixedContentHeight <= this->bodyPartHeight())
                this->setBodyRealHeight(this->bodyPartHeight());
            else
                this->setBodyRealHeight(curFixedContentHeight);
        }
    }
}

void DClusterUserAddDevicePage::recvDeviceInfoFromServer(InputPacket& inpack)
{
    quint8 ansCode = inpack.getAnswerCode();
    if(ansCode == Mi::Success){
        QString strText = tr("设备添加成功");
        hintMsgPage->setStatus(true);
        this->handleHintInfo(strText);
    }
    else if(ansCode == Mi::Failure){
        QString strText = tr("设备添加失败, 请重试");
        this->handleHintInfo(strText);
    }
}

void DClusterUserAddDevicePage::setDeviceButton(DClusterDevInfo *devInfo)
{
    if(devInfo == NULL) return;
    QPixmap pixmap(ImagePath::DEVICE_PORTRAIT);
    pixmap.setDevicePixelRatio(2.0);

    QPixmap pixmapReserve(ImagePath::SELECT_CANCEL);
    pixmapReserve.setDevicePixelRatio(2.0);

    //设备按钮设置
    GroupButton* btn = new GroupButton(this);
    btn->setIndexOne(devInfo->getDeviceID());
    btn->setText(devInfo->getDeviceName());
    btn->setPixMap(pixmap);
    btn->setFixedHeight(this->screenHeight()*0.1);
    btn->setFixedWidth(this->screenWidth());
    btn->setStyleSheet(SheetStyle::GROUPBUTTON_BOTTOMBORDER);
    btn->setReserve(pixmapReserve);
    btn->setAttribute(Qt::WA_LayoutUsesWidgetRect);
    devicesBtnList.insert(devInfo->getDeviceID(), btn);
    connect(btn, SIGNAL(clicked()), this, SLOT(on_btnDeviceButton_clicked()));

    deviceListLayout->addWidget(btn);
    curFixedContentHeight += btn->height();
    this->repaint();
}

void DClusterUserAddDevicePage::on_addDevice_clicked()
{
    if(deviceAddCount == 0){
        QString strText = tr("请选择添加的设备");
        this->handleHintInfo(strText);
        return;
    }

    QHashIterator<quint64, GroupButton*> iter(devicesBtnList);
    AssemblyData assemblyData;
    assemblyData.append(curClusterID);
    assemblyData.append(curUserID);
    assemblyData.append(deviceAddCount);

    while(iter.hasNext())
    {
        iter.next();
        GroupButton* groupButton = iter.value();
        if(groupButton->getSelectFlag()){
            quint64 deviceID = groupButton->getIndexOne();
            assemblyData.append(deviceID);
            devIdList.append(deviceID);

            quint8 authRole = Mi::SeniorManager;
            assemblyData.append(authRole);
        }
    }

    char* data = assemblyData.getAssemblyData();
    quint16 dataLen = assemblyData.getAssemblyDataLen();
    emit readyRead(CommandCode::DCLUSTER_USER_ADD_DEVICE, data, dataLen);

    //页面提示信息
    if(loadingPage == NULL){
        loadingPage = new LoadingPage(this);
        loadingPage->show();
    }
}

void DClusterUserAddDevicePage::on_btnDeviceButton_clicked()
{
    GroupButton* groupButton = dynamic_cast<GroupButton*>(sender());
    QPixmap pixmapBtnDelete(ImagePath::SELECT_SUBMIT);
    pixmapBtnDelete.setDevicePixelRatio(2.0);

    QPixmap pixmapBtnCancel(ImagePath::SELECT_CANCEL);
    pixmapBtnCancel.setDevicePixelRatio(2.0);

    if(groupButton->getSelectFlag() == false){
        groupButton->setReserve(pixmapBtnDelete);
        groupButton->setSelectFlag(true);
        deviceAddCount++;
    }
    else{
        groupButton->setReserve(pixmapBtnCancel);
        groupButton->setSelectFlag(false);
        deviceAddCount--;
    }

    if(deviceAddCount > 0){
        QString strLeftBtn = tr("取消");
        QPixmap leftArrowsPixmap("");
        navigationBar->setLeftText(strLeftBtn);
        navigationBar->setLeftPixMap(leftArrowsPixmap);
        disconnect(navigationBar, SIGNAL(leftClicked()), this, SLOT(on_btnPrePage_clicked()));
        connect(navigationBar, SIGNAL(leftClicked()), this, SLOT(on_btnCancel_clicked()));
    }
    else{
        QString strLeftBtn = tr("返回");
        QPixmap leftArrowsPixmap(ImagePath::NAVIGATION_LEFT_ARROWS);
        navigationBar->setLeftText(strLeftBtn);
        navigationBar->setLeftPixMap(leftArrowsPixmap);
        disconnect(navigationBar, SIGNAL(leftClicked()), this, SLOT(on_btnCancel_clicked()));
        connect(navigationBar, SIGNAL(leftClicked()), this, SLOT(on_btnPrePage_clicked()));
    }
}

void DClusterUserAddDevicePage::on_btnCancel_clicked()
{
    QHashIterator<quint64, GroupButton*> iter(devicesBtnList);
    QPixmap pixmapBtnCancel(ImagePath::SELECT_CANCEL);
    pixmapBtnCancel.setDevicePixelRatio(2.0);

    while(iter.hasNext()){
        iter.next();
        GroupButton* groupButton = iter.value();
        if(groupButton->getSelectFlag()){
            groupButton->setReserve(pixmapBtnCancel);
            groupButton->setSelectFlag(false);
            deviceAddCount--;
        }
    }

    QString strLeftBtn = tr("返回");
    QPixmap leftArrowsPixmap(ImagePath::NAVIGATION_LEFT_ARROWS);
    navigationBar->setLeftText(strLeftBtn);
    navigationBar->setLeftPixMap(leftArrowsPixmap);
    disconnect(navigationBar, SIGNAL(leftClicked()), this, SLOT(on_btnCancel_clicked()));
    connect(navigationBar, SIGNAL(leftClicked()), this, SLOT(on_btnPrePage_clicked()));
}

void DClusterUserAddDevicePage::on_hintMsgPage(bool isJump)
{
    if(!isJump)
        hintMsgPage->hide();
    else{
        this->hide();
        this->deleteLater();
    }
}

void DClusterUserAddDevicePage::on_btnPrePage_clicked()
{
    this->hide();
    this->deleteLater();
}

DClusterUserAddDevicePage::~DClusterUserAddDevicePage()
{
    SAFE_DELETE(deviceList);
}

