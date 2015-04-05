#include <QToolButton>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "GroupButton.h"
#include "UserProtocol.hpp"
#include "HintMsgPage.h"
#include "AssemblyData.h"
#include "LoadingPage.h"
#include "ImageConfig.hpp"
#include "DClusterInfo.h"
#include "MyDeviceInfoList.h"
#include "ListWidget.h"
#include "InputPacket.h"
#include "NavigationBar.h"
#include "SheetStyleConfig.hpp"
#include "DClusterDataSource.h"
#include "DClusterDeviceListPage.h"
#include "DClusterUserListPage.h"
#include "DClusterDeleteVerifyPage.h"
#include "DClusterSetManagerPage.h"
#include "DClusterAlterDescribePage.h"
#include "DClusterAlterRemarksPage.h"
#include "DClusterDetailInfoPage.h"
#include "DClusterManagementPage.h"
#include "DClusterClaimDeviceListPage.h"
#include "DClusterTransferSearchPage.h"

DClusterManagementPage::DClusterManagementPage(QWidget* parent, quint64 clusterID)
    :BasePage(parent)
    , curClusterID(clusterID)
    , curSolidBodyHeight(0)
    , loadingPage(NULL)
    , hintMsgExitCluster(NULL)
    , clusterUserListPage(NULL)
    , clusterSetManagerPage(NULL)
    , clusterDeleteVerifyPage(NULL)
    , clusterDeviceListPage(NULL)
    , clusterClaimDeviceListPage(NULL)
    , clusterTransferSearchPage(NULL)
    , clusterAlterDescribePage(NULL)
    , clusterAlterRemarksPage(NULL)
    , clusterDetailInfoPage(NULL)
{
    this->initializeWidget();
}

void DClusterManagementPage::initializeWidget()
{
    //加载、提示信息页面操作
    hintMsgPage = new HintMsgPage(this);
    hintMsgPage->hide();
    connect(hintMsgPage, SIGNAL(submitClicked(bool)), this, SLOT(on_hintMsgPage(bool)));

    //导航栏
    QString strTitle = tr("群管理");
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

    //主体部分
    btnClusterInfo = new GroupButton(this);
    QPixmap headPixmap(ImagePath::DCLUSTER_PORTRAIT);
    btnClusterInfo->setPixMap(headPixmap);
    btnClusterInfo->setFixedWidth(this->screenWidth());
    btnClusterInfo->setFixedHeight(this->screenHeight()*0.12);
    btnClusterInfo->setStyleSheet(SheetStyle::GROUPBUTTON_BOTTOMBORDER);
    btnClusterInfo->setAttribute(Qt::WA_LayoutUsesWidgetRect);
    curSolidBodyHeight += btnClusterInfo->height();
    connect(btnClusterInfo, SIGNAL(clicked()), this, SLOT(on_btnClusterDetail_clicked()));

    //群信息操作
    btnClusterRemarks = new GroupButton(this);
    btnClusterRemarks->setStyleSheet(SheetStyle::GROUPBUTTON_BOTTOMBORDER);
    setGroupButton(btnClusterRemarks, tr("备注"));
    connect(btnClusterRemarks, SIGNAL(clicked()), this, SLOT(on_btnClusterRemarks_clicked()));

    btnClusterDescribe = new GroupButton(this);
    btnClusterDescribe->setStyleSheet(SheetStyle::GROUPBUTTON_BOTTOMBORDER);
    setGroupButton(btnClusterDescribe, tr("介绍"));
    connect(btnClusterDescribe, SIGNAL(clicked()), this, SLOT(on_btnClusterDescribe_clicked()));

    btnClusterStatus = new GroupButton(this);
    btnClusterStatus->setStyleSheet(SheetStyle::GROUPBUTTON_BOTTOMBORDER);
    setGroupButton(btnClusterStatus, tr("状态"));
    connect(btnClusterStatus, SIGNAL(clicked()), this, SLOT(on_btnClusterStatus_clicked()));

    btnClusterSize = new GroupButton(this);
    btnClusterSize->setStyleSheet(SheetStyle::GROUPBUTTON_NOBORDER);
    setGroupButton(btnClusterSize, tr("大小"));
    curSolidBodyHeight += btnClusterSize->height();
    connect(btnClusterSize, SIGNAL(clicked()), this, SLOT(on_btnClusterSize_clicked()));

    clusterDetailListWidget = new ListWidget(this);
    clusterDetailListWidget->addWidget(btnClusterRemarks);
    clusterDetailListWidget->addWidget(btnClusterDescribe);
    clusterDetailListWidget->addWidget(btnClusterStatus);
    clusterDetailListWidget->addWidget(btnClusterSize);
    this->setListWidget(clusterDetailListWidget);
    
    //群控制
    //btnDeviceClaim = new GroupButton(this);
    //btnDeviceClaim->setStyleSheet(SheetStyle::GROUPBUTTON_BOTTOMBORDER);
    //setGroupButton(btnDeviceClaim, tr("设备认领"));
    //connect(btnDeviceClaim, SIGNAL(clicked()), this, SLOT(on_btnDeviceClaim_clicked()));
    
    btnDeviceTransfer = new GroupButton(this);
    btnDeviceTransfer->setStyleSheet(SheetStyle::GROUPBUTTON_NOBORDER);
    setGroupButton(btnDeviceTransfer, tr("设备移交"));
    btnDeviceTransfer->setContentsMargins(this->width()*0.03, 0, this->screenWidth()*0.02, 0);
    connect(btnDeviceTransfer, SIGNAL(clicked()), this, SLOT(on_btnDeviceTransfer_clicked()));

    //clusterDevHandleListWidget = new ListWidget(this);
    //clusterDevHandleListWidget->addWidget(btnDeviceClaim);
    //clusterDevHandleListWidget->addWidget(btnDeviceTransfer);
    //this->setListWidget(clusterDevHandleListWidget);

    //群成员列表
    btnDevicesList = new GroupButton(this);
    btnDevicesList->setStyleSheet(SheetStyle::GROUPBUTTON_BOTTOMBORDER);
    setGroupButton(btnDevicesList, tr("设备列表"));
    connect(btnDevicesList, SIGNAL(clicked()), this, SLOT(on_btnDevicesList_clicked()));

    btnUsersList = new GroupButton(this);
    btnUsersList->setStyleSheet(SheetStyle::GROUPBUTTON_NOBORDER);
    setGroupButton(btnUsersList, tr("成员列表"));
    connect(btnUsersList, SIGNAL(clicked()), this, SLOT(on_btnUsersList_clicked()));

    clusterMemberListWidget = new ListWidget(this);
    clusterMemberListWidget->addWidget(btnDevicesList);
    clusterMemberListWidget->addWidget(btnUsersList);
    this->setListWidget(clusterMemberListWidget);

    //群权限设置
    btnClusterAuthorize = new GroupButton(this);
    btnClusterAuthorize->setStyleSheet(SheetStyle::GROUPBUTTON_UPANDDOWNBORDER);
    setGroupButton(btnClusterAuthorize, tr("群权限设置"));
    btnClusterAuthorize->setContentsMargins(this->screenWidth()*0.03,0,this->screenWidth()*0.02,0);
    connect(btnClusterAuthorize, SIGNAL(clicked()), this, SLOT(on_btnClusterAuthorize_clicked()));

    //退出群
     btnExitCluster = new QPushButton(this);
     btnExitCluster->setFixedHeight(this->screenHeight()*0.074);
     btnExitCluster->setStyleSheet("color:white; background-color: rgb(236,80,80); border-radius: 3px; font:16px");
     curSolidBodyHeight += btnExitCluster->height();
     connect(btnExitCluster, SIGNAL(clicked()), this, SLOT(on_btnExitCluster_clicked()));

     QVBoxLayout* exitClusterLayout = new QVBoxLayout;
     exitClusterLayout->addWidget(btnExitCluster);
     exitClusterLayout->setContentsMargins(this->screenWidth()*0.05, 0, this->screenWidth()*0.05, 0);

     QVBoxLayout* vbTotalLayout = new QVBoxLayout;
     vbTotalLayout->addWidget(btnClusterInfo);
     vbTotalLayout->addSpacing(this->screenHeight()*0.037);
     curSolidBodyHeight += this->screenHeight()*0.037;
     vbTotalLayout->addWidget(clusterDetailListWidget);
     vbTotalLayout->addSpacing(this->screenHeight()*0.037);
     curSolidBodyHeight += this->screenHeight()*0.037;
     vbTotalLayout->addWidget(btnDeviceTransfer);
     vbTotalLayout->addSpacing(this->screenHeight()*0.037);
     curSolidBodyHeight += this->screenHeight()*0.037;
     vbTotalLayout->addWidget(clusterMemberListWidget);
     vbTotalLayout->addSpacing(this->screenHeight()*0.037);
     curSolidBodyHeight += this->screenHeight()*0.037;
     vbTotalLayout->addWidget(btnClusterAuthorize);
     vbTotalLayout->addSpacing(this->screenHeight()*0.037);
     curSolidBodyHeight += this->screenHeight()*0.037;
     vbTotalLayout->addLayout(exitClusterLayout);
     vbTotalLayout->addSpacing(this->screenHeight()*0.037);
     curSolidBodyHeight += this->screenHeight()*0.037;
     vbTotalLayout->addStretch(0);
     vbTotalLayout->setSpacing(0);
     vbTotalLayout->setMargin(0);
     this->setBodyPartLayout(vbTotalLayout);
    
     //屏幕触摸滚动设置
     this->loadLocalData();
     this->installScrollViewportArea();
     this->setBodyRealHeight(curSolidBodyHeight);
}

void DClusterManagementPage::fetchData(InputPacket &inpack)
{
    quint16 cmdCode = inpack.getCmdCode();
    DPRINT("-->DevClusterManagementPage receive data, the cmdCode is:0x%x", cmdCode);

    switch(cmdCode)
    {
        case CommandCode::NET_ERROR:
            this->handleNetError();
            break;
        case CommandCode::DCLUSTER_EXIT:
            this->handleExitDevCluster(inpack);
            break;
        case CommandCode::DCLUSTER_DELETE:
            this->handleDeleteDevCluster(inpack);
            break;
    }

    this->transferDataToAnotherPage(inpack, clusterTransferSearchPage);
    this->transferDataToAnotherPage(inpack, clusterSetManagerPage);
    this->transferDataToAnotherPage(inpack, clusterUserListPage);
    this->transferDataToAnotherPage(inpack, clusterDeleteVerifyPage);
    this->transferDataToAnotherPage(inpack, clusterDeviceListPage);
    this->transferDataToAnotherPage(inpack, clusterAlterDescribePage);
    this->transferDataToAnotherPage(inpack, clusterAlterRemarksPage);
    this->transferDataToAnotherPage(inpack, clusterDetailInfoPage);
    this->transferDataToAnotherPage(inpack, clusterClaimDeviceListPage);
}

void DClusterManagementPage::handleNetError()
{
    if(loadingPage == NULL) return;
    QString lblText = tr("网络连接错误,请重试");
    this->handleHintInfo(lblText);
}

void DClusterManagementPage::handleHintInfo(QString& text)
{
    hintMsgPage->setText(text);
    hintMsgPage->show();
    SAFE_DELETE(loadingPage);
}

void DClusterManagementPage::handleExitDevCluster(InputPacket &inpack)
{
    quint8 ansCode = inpack.getAnswerCode();
    if(ansCode == Mi::Failure){
        QString strText = tr("退出群失败,请重试");
        this->handleHintInfo(strText);
    }
    else if(ansCode == Mi::Success){
        QString strText = tr("退出群成功");
        hintMsgPage->setStatus(true);
        this->handleHintInfo(strText);
    }
}

void DClusterManagementPage::handleDeleteDevCluster(InputPacket &inpack)
{
    quint8 ansCode = inpack.getAnswerCode();
    if(ansCode == Mi::Failure){
        QString strText = tr("删除群请求失败,请重试");
        this->handleHintInfo(strText);
    }
    else if(ansCode == Mi::Success){
        SAFE_DELETE(loadingPage);
        clusterDeleteVerifyPage = new DClusterDeleteVerifyPage(this, curClusterID);
        this->displayConcretePage(clusterDeleteVerifyPage);
    }
}

void DClusterManagementPage::loadLocalData()
{
    QPixmap headPixmap(ImagePath::DCLUSTER_PORTRAIT);
    curClusterInfo.loadDataSource(curClusterID);
    btnClusterInfo->setPixMap(headPixmap);

    curClusterName = curClusterInfo.getClusterName();
    btnClusterInfo->setText(curClusterName);
    btnClusterStatus->setDescribe(tr("未认证"));
    btnClusterSize->setDescribe(tr("40"));

    QString remarks = curClusterInfo.getRemarks();
    QString strRemarks = this->cutOutString(remarks);
    btnClusterRemarks->setDescribe(strRemarks);

    QString describe = curClusterInfo.getDescribe();
    QString strDetail = this->cutOutString(describe);
    btnClusterDescribe->setDescribe(strDetail);
    curUserPermission = curClusterInfo.getUserRole();

    if(curUserPermission == Mi::SySManager || curUserPermission == Mi::SySManager)
        btnExitCluster->setText(tr("解散该群"));
    else
        btnExitCluster->setText(tr("退出该群"));
}

void DClusterManagementPage::transferDataToAnotherPage(InputPacket &inpack, BasePage *basePage)
{
    if(basePage == NULL) return;
    basePage->fetchData(inpack);
}

void DClusterManagementPage::setListWidget(ListWidget *listWidget)
{
    listWidget->setStyleSheet(SheetStyle::LISTWIDGET);
    listWidget->setFixedWidth(this->screenWidth());
    listWidget->setContentsMargins(this->screenWidth()*0.03,0,0,0);
}

void DClusterManagementPage::setGroupButton(GroupButton* gButton, QString title, QString titleDetail)
{
    QPixmap pixmap(ImagePath::RIGHT_HOLLOW_ARROWS);
    gButton->setReserve(pixmap);
    gButton->setText(title);
    gButton->setDescribe(titleDetail);
    gButton->setAttribute(Qt::WA_LayoutUsesWidgetRect);
    gButton->setFixedHeight(this->screenHeight()*0.0775);
    gButton->setContentsMargins(0,0,this->screenWidth()*0.02,0);
    curSolidBodyHeight += gButton->height();
}

void DClusterManagementPage::displayConcretePage(BasePage* page)
{
    if(page == NULL) return;
    page->show();
    connect(page, SIGNAL(destroyed()), this, SLOT(on_deleteSubPages()));
    connect(page, SIGNAL(readyRead(quint16,char*,quint16)), this, SLOT(on_recvDataFromOtherPage(quint16,char*,quint16)));
}


bool DClusterManagementPage::isPermission()
{
    if(curUserPermission == Mi::Operator){
        QString strText = tr("没有操作权限,请联系管理员.");
        this->handleHintInfo(strText);
        return false;
    }
    return true;
}

QString DClusterManagementPage::cutOutString(const QString &str)
{
    QString cutStr = str;
    if(cutStr.length() > 12)
        cutStr = str.leftJustified(12, '.', true) + QString("...");
    else if(cutStr.length() == 0)
        cutStr = QString("");
    return cutStr;
}

void DClusterManagementPage::on_deleteSubPages()
{
    clusterAlterRemarksPage = NULL;
    clusterAlterDescribePage = NULL;
    clusterTransferSearchPage = NULL;
    clusterSetManagerPage = NULL;
    clusterDeleteVerifyPage = NULL;
    clusterUserListPage = NULL;
    clusterDeviceListPage = NULL;
    clusterClaimDeviceListPage = NULL;
    clusterDetailInfoPage = NULL;
    this->loadLocalData();
}

void DClusterManagementPage::on_btnClusterRemarks_clicked()
{
    if(!this->isPermission()) return;
    clusterAlterRemarksPage = new DClusterAlterRemarksPage(this, curClusterID);
    this->displayConcretePage(clusterAlterRemarksPage);
}

void DClusterManagementPage::on_btnClusterDescribe_clicked()
{
    if(!this->isPermission()) return;
    clusterAlterDescribePage = new DClusterAlterDescribePage(this, curClusterID);
    this->displayConcretePage(clusterAlterDescribePage);
}

void DClusterManagementPage::on_btnClusterStatus_clicked()
{
    if(!this->isPermission()) return;
}

void DClusterManagementPage::on_btnClusterSize_clicked()
{
    if(!this->isPermission()) return;
}

void DClusterManagementPage::on_btnDeviceClaim_clicked()
{
    clusterClaimDeviceListPage = new DClusterClaimDeviceListPage(this, curClusterID);
    this->displayConcretePage(clusterClaimDeviceListPage);
}

void DClusterManagementPage::on_btnDevicesList_clicked()
{
    if(!this->isPermission()) return;
    clusterDeviceListPage = new DClusterDeviceListPage(this, curClusterID);
    this->displayConcretePage(clusterDeviceListPage);
}

void DClusterManagementPage::on_btnUsersList_clicked()
{
    if(!this->isPermission()) return;
    clusterUserListPage = new DClusterUserListPage(this, curClusterID);
    this->displayConcretePage(clusterUserListPage);
}

void DClusterManagementPage::on_btnDeviceTransfer_clicked()
{
    if(!this->isPermission()) return;
    clusterTransferSearchPage = new DClusterTransferSearchPage(this, curClusterID);
    clusterTransferSearchPage->setCurClusterName(curClusterName);
    this->displayConcretePage(clusterTransferSearchPage);
}

void DClusterManagementPage::on_btnClusterAuthorize_clicked()
{
    if(!this->isPermission()) return;
    clusterSetManagerPage = new DClusterSetManagerPage(this, curClusterID);
    this->displayConcretePage(clusterSetManagerPage);
}

void DClusterManagementPage::on_btnExitCluster_clicked()
{
    QString strText = tr("是否确定退出该群?");
    if(hintMsgExitCluster == NULL) {
        hintMsgExitCluster = new HintMsgPage(this, HintMsgPage::SubmitAndCancel);
    }

    hintMsgExitCluster->setText(strText);
    hintMsgExitCluster->show();
    connect(hintMsgExitCluster, SIGNAL(submitClicked(bool)), this, SLOT(on_hintMsgExitCluster_Submit_clicked(bool)));
    connect(hintMsgExitCluster, SIGNAL(cancelClicked()), this, SLOT(on_hintMsgExitCluster_Cancel_clicked()));
}

void DClusterManagementPage::on_hintMsgExitCluster_Submit_clicked(bool)
{
    quint64 clusterID = curClusterInfo.getClusterID();
    quint64 clusterRole = curClusterInfo.getUserRole();

    AssemblyData assemblyData;
    assemblyData.append(clusterID);
    char* data = assemblyData.getAssemblyData();
    quint16 dataLen = assemblyData.getAssemblyDataLen();

    if(clusterRole == Mi::SySManager)
        emit readyRead(CommandCode::DCLUSTER_DELETE, data, dataLen);
    else if(clusterRole == Mi::SeniorManager || clusterRole == Mi::Operator)
        emit readyRead(CommandCode::DCLUSTER_EXIT, data, dataLen);

    //页面加载窗口
    hintMsgExitCluster->hide();
    if(loadingPage == NULL){
        loadingPage = new LoadingPage(this);
        loadingPage->show();
    }
}

void DClusterManagementPage::on_hintMsgExitCluster_Cancel_clicked()
{
    hintMsgExitCluster->hide();
}

void DClusterManagementPage::on_hintMsgPage(bool isJump)
{
    hintMsgPage->hide();
    if(!isJump) return;
    this->hide();
    this->deleteLater();
}

void DClusterManagementPage::on_btnPrePage_clicked()
{
    this->hide();
    this->deleteLater();
}

void DClusterManagementPage::on_btnClusterDetail_clicked()
{
    clusterDetailInfoPage = new DClusterDetailInfoPage(this, curClusterID);
    this->displayConcretePage(clusterDetailInfoPage);
}

void DClusterManagementPage::on_recvDataFromOtherPage(quint16 cmdCode, char *data, quint16 dataLen)
{
    if(cmdCode == CommandCode::BACK_DEVICE_PAGE){
        this->hide();
        this->deleteLater();
        return;
    }
    emit readyRead(cmdCode, data, dataLen);
}

DClusterManagementPage::~DClusterManagementPage()
{

}
