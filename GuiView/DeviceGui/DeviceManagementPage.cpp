#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>

#include "InputPacket.h"
#include "GroupButton.h"
#include "SearchBar.h"
#include "DClusterSearchPage.h"
#include "DClusterSearchResultPage.h"
#include "DClusterCreatePage.h"
#include "DeviceManagementPage.h"
#include "TouchScroller.h"
#include "UserProtocol.hpp"
#include "StatusBarParam.h"
#include "ListWidget.h"
#include "ImageConfig.hpp"
#include "SheetStyleConfig.hpp"
#include "NavigationBar.h"
#include "GlobalDefine.hpp"

DeviceManagementPage::DeviceManagementPage(QWidget* parent)
    : BasePage(parent)
    , clusterSearchPage(NULL)
    , clusterCreatePage(NULL)
{
    this->initializeWidget();
}

void DeviceManagementPage::initializeWidget()
{
    //导航栏
    QString strTitle = tr("添加");
    QString strLeftBtn = tr("设备");
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

    //搜索栏
    QString searchPlaceText = QString(tr("设备名称/设备ID"));
    searchBar = new SearchBar(this);
    searchBar->setPlaceholderText(searchPlaceText);
    connect(searchBar, SIGNAL(searchInputCompleted(QString&)), this, SLOT(on_searchBar(QString&)));

    //扫一扫
    //btnSweep = new GroupButton(this);
    //btnSweep->setStyleSheet(SheetStyle::GROUPBUTTON_UPANDDOWNBORDER);
    //btnSweep->setAttribute(Qt::WA_LayoutUsesWidgetRect);
    //QPixmap sweepPixmap(ImagePath::SWEEP);
    //sweepPixmap.setDevicePixelRatio(2.0);
    //this->setGroupButton(btnSweep, sweepPixmap, QString(tr("扫一扫")));
    //btnSweep->setContentsMargins(this->screenWidth()*0.02, 0, 0, 0);
    //connect(btnSweep, SIGNAL(clicked()), SLOT(on_btnSweep_clicked()));

    //查找群组
    btnSearchCluster = new GroupButton(this);
    btnSearchCluster->setContentsMargins(0,0,0,0);
    btnSearchCluster->setAttribute(Qt::WA_LayoutUsesWidgetRect);
    btnSearchCluster->setStyleSheet(SheetStyle::GROUPBUTTON_BOTTOMBORDER);

    QPixmap addClusterPixmap(ImagePath::SEARCH_DEVICE_CLUSTER);
    addClusterPixmap.setDevicePixelRatio(2.0);
    this->setGroupButton(btnSearchCluster, addClusterPixmap, QString(tr("查找设备群")));
    connect(btnSearchCluster, SIGNAL(clicked()), SLOT(on_searchCluster_clicked()));

    //创建群组
    btnCreateCluster = new GroupButton(this);
    btnCreateCluster->setContentsMargins(0,0,0,0);
    btnCreateCluster->setAttribute(Qt::WA_LayoutUsesWidgetRect);
    btnCreateCluster->setStyleSheet(SheetStyle::GROUPBUTTON_NOBORDER);

    QPixmap createClusterPixmap(ImagePath::CREATE_DEVICE_CLUSTER);
    createClusterPixmap.setDevicePixelRatio(2.0);
    this->setGroupButton(btnCreateCluster, createClusterPixmap, QString(tr("创建设备群")));
    connect(btnCreateCluster, SIGNAL(clicked()), SLOT(on_createCluster_clicked()));

    ListWidget* clusterOper = new ListWidget(this);
    clusterOper->setStyleSheet(SheetStyle::LISTWIDGET);
    clusterOper->addWidget(btnSearchCluster);
    clusterOper->addWidget(btnCreateCluster);
    clusterOper->setFixedWidth(this->screenWidth());
    clusterOper->setContentsMargins(this->screenWidth()*0.05,0,0,0);

    QVBoxLayout* vblTotalLayout = new QVBoxLayout;
    vblTotalLayout->addSpacing(this->screenHeight()*0.026);
    vblTotalLayout->addWidget(searchBar);
    //vblTotalLayout->addSpacing(this->screenHeight()*0.037);
    //vblTotalLayout->addWidget(btnSweep);
    vblTotalLayout->addSpacing(this->screenHeight()*0.037);
    vblTotalLayout->addWidget(clusterOper);
    vblTotalLayout->setMargin(0);
    vblTotalLayout->addStretch(0);
    this->setBodyPartLayout(vblTotalLayout);

    //屏幕触摸滚动设置
    this->installScrollViewportArea();
}

void DeviceManagementPage::fetchData(InputPacket& inpack)
{
    quint16 cmdCode = inpack.getCmdCode();
    DPRINT("-->DeviceManagementPage receive data, the cmdCode:0x%x", cmdCode);

    this->transferDataToAnotherPage(inpack, clusterCreatePage);
    this->transferDataToAnotherPage(inpack, clusterSearchPage);
}

void DeviceManagementPage::transferDataToAnotherPage(InputPacket &inpack, BasePage *basePage)
{
    if(basePage == NULL) return;
    basePage->fetchData(inpack);
}

void DeviceManagementPage::setGroupButton(GroupButton* groupButton, QPixmap photo, QString title)
{
    groupButton->setPixMap(photo);
    groupButton->setText(title);

    QPixmap pixmap(ImagePath::RIGHT_HOLLOW_ARROWS);
    groupButton->setReserve(pixmap);
    groupButton->setFixedHeight(this->screenHeight()*0.0775);
    groupButton->setContentsMargins(0, 0, this->screenWidth()*0.03, 0);
}

void DeviceManagementPage::displayConcretePage(BasePage *page)
{
    if(page == NULL) return;
    page->show();
    connect(page, SIGNAL(destroyed()), this, SLOT(on_deleteSubPages()));
    connect(page, SIGNAL(readyRead(quint16,char*,quint16)), this, SLOT(on_recvDataFromOtherPage(quint16,char*,quint16)));
}

void DeviceManagementPage::on_searchBar(QString&)
{
    //////////////////////////////////////////
}

void DeviceManagementPage::on_btnSweep_clicked()
{
    //////////////////////////////////////////
}

void DeviceManagementPage::on_searchCluster_clicked()
{
    clusterSearchPage = new DClusterSearchPage(this);
    this->displayConcretePage(clusterSearchPage);
}

void DeviceManagementPage::on_createCluster_clicked()
{
    clusterCreatePage = new DClusterCreatePage(this);
    this->displayConcretePage(clusterCreatePage);
}

void DeviceManagementPage::on_deleteSubPages()
{
    clusterSearchPage = NULL;
    clusterCreatePage = NULL;
}

void DeviceManagementPage::on_recvDataFromOtherPage(quint16 cmdCode, char *data, quint16 dataLen)
{
    if(cmdCode == CommandCode::BACK_DEVICE_PAGE){
        this->hide();
        this->deleteLater();
        return;
    }
    emit readyRead(cmdCode, data, dataLen);
}

void DeviceManagementPage::on_btnPrePage_clicked()
{
    this->hide();
    this->deleteLater();
}

DeviceManagementPage::~DeviceManagementPage()
{

}

