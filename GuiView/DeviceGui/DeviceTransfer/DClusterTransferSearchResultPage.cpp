#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

#include "UserProtocol.hpp"
#include "ListWidget.h"
#include "ImageConfig.hpp"
#include "NavigationBar.h"
#include "InputPacket.h"
#include "LoadingPage.h"
#include "SheetStyleConfig.hpp"
#include "DClusterInfoList.h"
#include "GroupButton.h"
#include "DClusterTransferDevicePage.h"
#include "DClusterTransferSearchResultPage.h"

DClusterTransferSearchResultPage::DClusterTransferSearchResultPage(QWidget* parent, quint64 clusterID)
    : BasePage(parent)
    , clusterTransferDevicePage(NULL)
    , curClusterID(clusterID)
    , clusterInfoList(NULL)
{
    this->initializeWidget();
}

void DClusterTransferSearchResultPage::initializeWidget()
{
    //导航栏
    QString strTitle = "群资料";
    QString strLeftBtn = "返回";
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

    //群标题信息
    QPixmap pixmap(ImagePath::DEVICE_PORTRAIT);
    btnTitleInfo = new GroupButton(this);
    btnTitleInfo->setPixMap(pixmap);
    btnTitleInfo->setFixedHeight(this->screenHeight()*0.12);
    btnTitleInfo->setFixedWidth(this->screenWidth());
    btnTitleInfo->setAttribute(Qt::WA_LayoutUsesWidgetRect);

    //群详细信息
    btnRemaks = new GroupButton(this);
    btnRemaks->setAttribute(Qt::WA_LayoutUsesWidgetRect);
    btnRemaks->setStyleSheet(SheetStyle::GROUPBUTTON_BOTTOMBORDER);
    btnRemaks->setText(tr("备注"));
    btnRemaks->setFixedHeight(this->screenHeight()*0.08);
    btnRemaks->setContentsMargins(0,0,this->screenWidth()*0.03, 0);

    btnDescribe = new GroupButton(this);
    btnDescribe->setAttribute(Qt::WA_LayoutUsesWidgetRect);
    btnDescribe->setStyleSheet(SheetStyle::GROUPBUTTON_NOBORDER);
    btnDescribe->setText(tr("描述"));
    btnDescribe->setFixedHeight(this->screenHeight()*0.08);
    btnDescribe->setContentsMargins(0,0,this->screenWidth()*0.03, 0);

    ListWidget* clusInfoList = new ListWidget(this);
    clusInfoList->setStyleSheet(SheetStyle::LISTWIDGET);
    clusInfoList->addWidget(btnRemaks);
    clusInfoList->addWidget(btnDescribe);
    clusInfoList->setContentsMargins(this->screenWidth()*0.05,0,0,0);

    btnDeviceTransfer = new QPushButton(this);
    btnDeviceTransfer->setText("设备移交");
    btnDeviceTransfer->setFixedHeight(this->bottomBarHeight());
    btnDeviceTransfer->setStyleSheet("background-color:white; border-radius:3px; font:15px; color:#0090ff;");
    connect(btnDeviceTransfer, SIGNAL(clicked()), this, SLOT(on_btnDeviceTransfer_clicked()));
    
    QVBoxLayout* vbTransferDevice = new QVBoxLayout;
    vbTransferDevice->addWidget(btnDeviceTransfer);
    vbTransferDevice->setSpacing(0);
    vbTransferDevice->setMargin(0);
    this->setBottombarLayout(vbTransferDevice);

    QVBoxLayout* vbTotalLayout = new QVBoxLayout;
    vbTotalLayout->addWidget(btnTitleInfo);
    vbTotalLayout->addWidget(clusInfoList);
    vbTotalLayout->setMargin(0);
    vbTotalLayout->setSpacing(0);
    vbTotalLayout->addStretch(0);
    this->setBodyPartLayout(vbTotalLayout);

    //屏幕触摸滚动设置
    this->setBodyScreenHeight(this->scrollAreaHasBottomBarHeight());
    this->installScrollViewportArea();
}

void DClusterTransferSearchResultPage::fetchData(InputPacket &inpack)
{
    quint16 cmdCode = inpack.getCmdCode();
    DPRINT("-->DevClusterTransferDeviceSearchResultPage receive data, the cmdCode is:0x%x", cmdCode);

    switch(cmdCode)
    {
        case CommandCode::SEARCH_DCLUSTER:
            this->handleSearchClusterInfo(inpack);
            break;
    }
    this->transferDataToAnotherPage(inpack, clusterTransferDevicePage);
}

void DClusterTransferSearchResultPage::handleSearchClusterInfo(InputPacket &inpack)
{
    char* dataField = inpack.getPackData();
    quint16 dataLen = inpack.getPackDataLen();

    SAFE_DELETE(clusterInfoList);
    clusterInfoList = new DClusterInfoList;
    clusterInfoList->appendList(dataField, dataLen);

    QHash<quint64, DClusterInfo*> clusterList = clusterInfoList->fetchList();
    QHashIterator<quint64, DClusterInfo*> iter(clusterList);

    while (iter.hasNext()) {
        iter.next();
        DClusterInfo* clusterInfo = iter.value();
        tranClusterID = clusterInfo->getClusterID();
        btnTitleInfo->setText(clusterInfo->getClusterName());
        btnRemaks->setDescribe(clusterInfo->getRemarks());
        btnDescribe->setDescribe(clusterInfo->getDescribe());
    }
}

void DClusterTransferSearchResultPage::displayConcretePage(BasePage *page)
{
    if(page == NULL) return;
    page->show();
    connect(page, SIGNAL(destroyed()), this, SLOT(on_deleteSubPages()));
    connect(page, SIGNAL(readyRead(quint16,char*,quint16)), this, SLOT(on_recvDataFromOtherPage(quint16,char*,quint16)));
}

void DClusterTransferSearchResultPage::transferDataToAnotherPage(InputPacket &inpack, BasePage* basePage)
{
    if(basePage == NULL) return;
    basePage->fetchData(inpack);
}

void DClusterTransferSearchResultPage::on_btnDeviceTransfer_clicked()
{
    clusterTransferDevicePage = new DClusterTransferDevicePage(this, curClusterID);
    clusterTransferDevicePage->setTranClusterID(tranClusterID);
    this->displayConcretePage(clusterTransferDevicePage);
}

void DClusterTransferSearchResultPage::on_deleteSubPages()
{
    clusterTransferDevicePage = NULL;
}

void DClusterTransferSearchResultPage::on_recvDataFromOtherPage(quint16 cmdCode, char *data, quint16 dataLen)
{
    emit readyRead(cmdCode, data, dataLen);
}

void DClusterTransferSearchResultPage::on_btnPrePage_clicked()
{
    this->hide();
    this->deleteLater();
}

DClusterTransferSearchResultPage::~DClusterTransferSearchResultPage()
{
    SAFE_DELETE(clusterInfoList);
}

