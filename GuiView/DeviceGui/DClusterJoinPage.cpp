#include <QHBoxLayout>
#include <QVBoxLayout>

#include "ToolKit.h"
#include "DClusterInfo.h"
#include "GroupButton.h"
#include "NavigationBar.h"
#include "HintMsgPage.h"
#include "LoadingPage.h"
#include "InputPacket.h"
#include "AssemblyData.h"
#include "ImageConfig.hpp"
#include "UserProtocol.hpp"
#include "DClusterJoinPage.h"
#include "SheetStyleConfig.hpp"

DClusterJoinPage::DClusterJoinPage(QWidget* parent, DClusterInfo& info)
    :BasePage(parent)
    , btnTitleInfo(NULL)
    , btnDescribe(NULL)
    , curClusterInfo(info)
    , loadingPage(NULL)
{
    this->initializeWidget();
}

void DClusterJoinPage::initializeWidget()
{
    //提示信息页面操作;
    hintMsgPage = new HintMsgPage(this);
    hintMsgPage->hide();
    connect(hintMsgPage, SIGNAL(submitClicked(bool)), this, SLOT(on_hintMsgPage(bool)));

    //导航栏
    QString strTitle = tr("群资料");
    QString strLeftBtn = tr("返回");
    QPixmap leftArrowsPixmap(ImagePath::NAVIGATION_LEFT_ARROWS);
    leftArrowsPixmap.setDevicePixelRatio(2);
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

    //设备群信息
    QPixmap pixmap(ImagePath::DCLUSTER_PORTRAIT);
    btnTitleInfo = new GroupButton(this);
    btnTitleInfo->setPixMap(pixmap);
    btnTitleInfo->setFixedHeight(this->screenHeight()*0.12);
    btnTitleInfo->setFixedWidth(this->screenWidth());
    btnTitleInfo->setAttribute(Qt::WA_LayoutUsesWidgetRect);

    btnDescribe = new GroupButton(this);
    btnDescribe->setAttribute(Qt::WA_LayoutUsesWidgetRect);
    btnDescribe->setStyleSheet(SheetStyle::GROUPBUTTON_NOBORDER);
    btnDescribe->setText(tr("描述"));
    btnDescribe->setFixedHeight(this->screenHeight()*0.08);
    btnDescribe->setContentsMargins(0,0,this->screenWidth()*0.03, 0);

    //添加管理员按钮
    btnJoinDevCluster = new QPushButton(this);
    if(this->hasJoinedCluster()){
        btnJoinDevCluster->setText(tr("返回设备页"));
        connect(btnJoinDevCluster, SIGNAL(clicked()), this, SLOT(on_backClusterManagementPage_clicked()));

    }else{
        btnJoinDevCluster->setText(tr("加入群组"));
        connect(btnJoinDevCluster, SIGNAL(clicked()), this, SLOT(on_btnJoinDevCluster_clicked()));
    }
    btnJoinDevCluster->setFixedHeight(this->bottomBarHeight());
    btnJoinDevCluster->setStyleSheet("background-color:white; border-radius:3px; font:15px; color:#0090ff;");

    QVBoxLayout* vbJoinDevCluster = new QVBoxLayout;
    vbJoinDevCluster->addWidget(btnJoinDevCluster);
    vbJoinDevCluster->setSpacing(0);
    vbJoinDevCluster->setMargin(0);
    this->setBottombarLayout(vbJoinDevCluster);

    QVBoxLayout* vbTotalLayout = new QVBoxLayout;
    vbTotalLayout->addWidget(btnTitleInfo);
    vbTotalLayout->addWidget(btnDescribe);
    vbTotalLayout->addSpacing(this->screenHeight()*0.05);
    vbTotalLayout->setMargin(0);
    vbTotalLayout->setSpacing(0);
    vbTotalLayout->addStretch(0);
    this->setBodyPartLayout(vbTotalLayout);

    //屏幕触摸滚动设置
    this->setClusterInfo(curClusterInfo);
    this->setBodyScreenHeight(scrollAreaHasBottomBarHeight());
    this->installScrollViewportArea();
}

bool DClusterJoinPage::hasJoinedCluster()
{
    quint64 clusID = curClusterInfo.getClusterID();
    return DClusterInfo::isExistClusterInfo(clusID);
}

void DClusterJoinPage::fetchData(InputPacket& inpack)
{
    quint16 cmdCode = inpack.getCmdCode();
    switch(cmdCode)
    {
        case CommandCode::NET_ERROR:
            this->handleNetError();
        case CommandCode::DCLUSTER_APPLY_REQ:
            this->handleJoinDevCluster(inpack);
            break;
    }
}

void DClusterJoinPage::handleHintInfo(QString& text)
{
    hintMsgPage->setText(text);
    hintMsgPage->show();
    SAFE_DELETE(loadingPage);
}

void DClusterJoinPage::handleNetError()
{
    if(loadingPage == NULL) return;
    QString lblText = tr("网络错误,请重试");
    this->handleHintInfo(lblText);
}

void DClusterJoinPage::handleJoinDevCluster(InputPacket &inpack)
{
    quint8 ansCode = inpack.getAnswerCode();
    if(ansCode == Mi::Failure){
        QString strText = tr("发送请求失败,请重试");
        this->handleHintInfo(strText);
    }
    else if(ansCode == Mi::Success){
        QString strText = tr("请求发送成功");
        hintMsgPage->setStatus(true);
        this->handleHintInfo(strText);
    }
}

void DClusterJoinPage::setClusterInfo(DClusterInfo& info)
{
    quint64 clusID = info.getClusterID();
    btnTitleInfo->setText(info.getClusterName());
    btnTitleInfo->setRemarks(QString(tr("群ID：")) + QString::number(clusID & ConstNum::DEVICE_ID_BASIC_POSTFIX));

    QString describe = info.getDescribe().left(12) + "...";
    btnDescribe->setText(QString(tr("描述")));
    btnDescribe->setDescribe(describe);
    btnDescribe->setContentsMargins(this->screenWidth()*0.05, 0, this->screenWidth()*0.05, 0);
}

void DClusterJoinPage::on_btnJoinDevCluster_clicked()
{
    quint64 clusID = curClusterInfo.getClusterID();
    AssemblyData assemblyData;
    assemblyData.append(clusID);
    char* data = assemblyData.getAssemblyData();
    quint16 dataLen = assemblyData.getAssemblyDataLen();
    emit readyRead(CommandCode::DCLUSTER_APPLY_REQ, data, dataLen);

    //页面加载窗口
    if(loadingPage == NULL){
        loadingPage = new LoadingPage(this);
        loadingPage->show();
    }
}

void DClusterJoinPage::on_backClusterManagementPage_clicked()
{
    quint64 clusID = curClusterInfo.getClusterID();
    AssemblyData assemblyData;
    assemblyData.append(clusID);
    char* data = assemblyData.getAssemblyData();
    quint16 dataLen = assemblyData.getAssemblyDataLen();
    emit readyRead(CommandCode::BACK_DEVICE_PAGE, data, dataLen);
}

void DClusterJoinPage::on_hintMsgPage(bool isJump)
{
    if(!isJump){
        hintMsgPage->hide();
        return;
    }
    emit readyRead(CommandCode::BACK_DEVICE_PAGE, NULL, 0);
}

void DClusterJoinPage::on_btnPrePage_clicked()
{
    this->hide();
    this->deleteLater();
}

DClusterJoinPage::~DClusterJoinPage()
{

}
