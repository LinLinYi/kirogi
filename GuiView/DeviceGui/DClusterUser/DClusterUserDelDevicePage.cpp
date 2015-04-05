#include <QVBoxLayout>
#include <QHBoxLayout>

#include "GroupButton.h"
#include "UserProtocol.hpp"
#include "HintMsgPage.h"
#include "LoadingPage.h"
#include "NavigationBar.h"
#include "ImageConfig.hpp"
#include "InputPacket.h"
#include "AssemblyData.h"
#include "SheetStyleConfig.hpp"
#include "DClusterAuthDevInfoList.h"
#include "DClusterUserDelDevicePage.h"

DClusterUserDelDevicePage::DClusterUserDelDevicePage(QWidget* parent, quint64 clusterID, quint64 userID)
    : BasePage(parent)
    , curClusterID(clusterID)
    , curUserID(userID)
    , curFixedContentHeight(0)
    , deviceDelCount(0)
    , loadingPage(NULL)
    , deviceList(NULL)
{
    initializeWidget();
}

void DClusterUserDelDevicePage::initializeWidget()
{
    //提示信息页面操作
    hintMsgPage = new HintMsgPage(this);
    hintMsgPage->hide();
    connect(hintMsgPage, SIGNAL(submitClicked(bool)), this, SLOT(on_hintMsgPage(bool)));

    //导航栏
    QString strTitle = tr("删除设备");
    QString strLeftBtn = tr("返回");
    QString strRightBtn = tr("删除");
    QPixmap leftArrowsPixmap(ImagePath::NAVIGATION_LEFT_ARROWS);
    navigationBar = new NavigationBar(this);
    navigationBar->setTitleText(strTitle);
    navigationBar->setLeftText(strLeftBtn);
    navigationBar->setRightText(strRightBtn);
    navigationBar->setLeftPixMap(leftArrowsPixmap);
    connect(navigationBar, SIGNAL(leftClicked()), this, SLOT(on_btnPrePage_clicked()));
    connect(navigationBar, SIGNAL(rightClicked()), this, SLOT(on_btnDelDevice_clicked()));

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

void DClusterUserDelDevicePage::fetchData(InputPacket &inpack)
{
    quint16 cmdCode = inpack.getCmdCode();
    DPRINT("-->DevClusterUserDelDevicePage receive data, the cmdCode is:0x%x", cmdCode);

    switch(cmdCode)
    {
        case CommandCode::NET_ERROR:
            this->handleNetError();
            break;
        case CommandCode::DCLUSTER_USER_DELETE_DEVICE:
            this->handleDelDeviceInfo(inpack);
            break;
        case CommandCode::FETCH_USER_AUTHORIZED_DEV_LIST:
            this->handleAuthorizedDeviceList(inpack);
            break;
    }
}

void DClusterUserDelDevicePage::timerEvent(QTimerEvent*)
{
    AssemblyData assemblyData;
    assemblyData.append(curClusterID);
    assemblyData.append(curUserID);
    char* data = assemblyData.getAssemblyData();
    quint16 dataLen = assemblyData.getAssemblyDataLen();
    emit readyRead(CommandCode::FETCH_USER_AUTHORIZED_DEV_LIST, data, dataLen);
    this->killTimer(timerID);
}

void DClusterUserDelDevicePage::handleNetError()
{
    if(loadingPage == NULL)
        return;

    for(int i = 0; i < devIdList.count(); i++)
        devIdList.removeAt(i);

    QString lblText = tr("网络错误, 请重试");
    this->handleHintInfo(lblText);
}

void DClusterUserDelDevicePage::handleAuthorizedDeviceList(InputPacket &inpack)
{
    if(deviceList == NULL)
        deviceList = new DClusterAuthDevInfoList;

    char* data = inpack.getPackData();
    quint16 dataLen = inpack.getPackDataLen();
    deviceList->appendList(data, dataLen);
    
    QHash<quint64, DClusterAuthDevInfo*> devList = deviceList->fetchList();
    QHashIterator<quint64, DClusterAuthDevInfo*> iter(devList);

    while(iter.hasNext())
    {
        iter.next();
        DClusterAuthDevInfo* devInfo = dynamic_cast<DClusterAuthDevInfo*>(iter.value());
        this->setDeviceButton(devInfo);

        if(curFixedContentHeight <= this->bodyPartHeight())
            this->setBodyRealHeight(this->bodyPartHeight());
        else
            this->setBodyRealHeight(curFixedContentHeight);
    }
}

void DClusterUserDelDevicePage::handleDelDeviceInfo(InputPacket& inpack)
{
    quint8 ansCode = inpack.getAnswerCode();
    if(ansCode == Mi::Success){
        QString strText = tr("设备删除成功");
        hintMsgPage->setStatus(true);
        this->handleHintInfo(strText);
    }
    else if(ansCode == Mi::Failure){
        QString strText = tr("设备删除失败, 请重试");
        this->handleHintInfo(strText);
    }
}

void DClusterUserDelDevicePage::setDeviceButton(DClusterAuthDevInfo *devInfo)
{
    if(devInfo == NULL)
        return;

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
}

void DClusterUserDelDevicePage::on_btnDelDevice_clicked()
{
    if(deviceDelCount == 0){
        QString strText = tr("请选择删除的设备");
        handleHintInfo(strText);
        return;
    }

    QHashIterator<quint64, GroupButton*> iter(devicesBtnList);
    AssemblyData assemblyData;
    assemblyData.append(curClusterID);
    assemblyData.append(curUserID);
    assemblyData.append(deviceDelCount);

    while(iter.hasNext())
    {
        iter.next();
        GroupButton* groupButton = iter.value();
        if(groupButton->getSelectFlag()){
            quint64 deviceID = groupButton->getIndexOne();
            assemblyData.append(deviceID);
            devIdList.append(deviceID);

            quint8 authCode = Mi::SeniorManager;
            assemblyData.append(authCode);
        }
    }

    char* data = assemblyData.getAssemblyData();
    quint16 dataLen = assemblyData.getAssemblyDataLen();
    emit readyRead(CommandCode::DCLUSTER_USER_DELETE_DEVICE, data, dataLen);

    //页面提示信息
    if(loadingPage == NULL){
        loadingPage = new LoadingPage(this);
        loadingPage->show();
    }
}

void DClusterUserDelDevicePage::on_btnDeviceButton_clicked()
{
    GroupButton* groupButton = dynamic_cast<GroupButton*>(sender());
    QPixmap pixmapBtnDelete(ImagePath::SELECT_SUBMIT);
    pixmapBtnDelete.setDevicePixelRatio(2.0);

    QPixmap pixmapBtnCancel(ImagePath::SELECT_CANCEL);
    pixmapBtnCancel.setDevicePixelRatio(2.0);

    if(groupButton->getSelectFlag() == false){
        groupButton->setReserve(pixmapBtnDelete);
        groupButton->setSelectFlag(true);
        deviceDelCount++;
    }
    else{
        groupButton->setReserve(pixmapBtnCancel);
        groupButton->setSelectFlag(false);
        deviceDelCount--;
    }

    if(deviceDelCount > 0){
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

void DClusterUserDelDevicePage::on_btnCancel_clicked()
{
    QHashIterator<quint64, GroupButton*> iter(devicesBtnList);
    QPixmap pixmapBtnCancel(ImagePath::SELECT_CANCEL);
    pixmapBtnCancel.setDevicePixelRatio(2.0);

    while(iter.hasNext())
    {
        iter.next();
        GroupButton* groupButton = iter.value();
        if(groupButton->getSelectFlag()){
            groupButton->setReserve(pixmapBtnCancel);
            groupButton->setSelectFlag(false);
            deviceDelCount--;
        }
    }

    QString strLeftBtn = tr("返回");
    QPixmap leftArrowsPixmap(ImagePath::NAVIGATION_LEFT_ARROWS);
    navigationBar->setLeftText(strLeftBtn);
    navigationBar->setLeftPixMap(leftArrowsPixmap);
    disconnect(navigationBar, SIGNAL(leftClicked()), this, SLOT(on_btnCancel_clicked()));
    connect(navigationBar, SIGNAL(leftClicked()), this, SLOT(on_btnPrePage_clicked()));
}

void DClusterUserDelDevicePage::on_hintMsgPage(bool isJump)
{
    hintMsgPage->hide();
    if(!isJump) return;
    this->hide();
    this->deleteLater();
}

void DClusterUserDelDevicePage::handleHintInfo(QString& text)
{
    hintMsgPage->setText(text);
    hintMsgPage->show();
    SAFE_DELETE(loadingPage);
}

void DClusterUserDelDevicePage::setUserID(quint64 usrID)
{
    curUserID = usrID;
}

void DClusterUserDelDevicePage::on_btnPrePage_clicked()
{
    this->hide();
    this->deleteLater();
}

DClusterUserDelDevicePage::~DClusterUserDelDevicePage()
{
    SAFE_DELETE(deviceList);
}
