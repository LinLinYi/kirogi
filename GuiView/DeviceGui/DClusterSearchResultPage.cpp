#include <QVBoxLayout>

#include "ToolKit.h"
#include "InputPacket.h"
#include "UserProtocol.hpp"
#include "ImageConfig.hpp"
#include "NavigationBar.h"
#include "DClusterInfoList.h"
#include "GroupButton.h"
#include "DClusterJoinPage.h"
#include "SheetStyleConfig.hpp"
#include "DClusterSearchResultPage.h"

DClusterSearchResultPage::DClusterSearchResultPage(QWidget* parent)
    :BasePage(parent)
    , curFixedContentHeight(0)
    , clusterInfoList(NULL)
    , clusterJoinPage(NULL)
{
    this->initializeWidget();
}

void DClusterSearchResultPage::initializeWidget()
{
    QString strTitle = tr("查找结果");
    QString strLeftBtn = tr("返回");
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

    clusterListLayOut = new QVBoxLayout;
    clusterListLayOut->setSpacing(0);
    clusterListLayOut->setMargin(0);
    this->setBodyPartLayout(clusterListLayOut);

    //屏幕触摸滚动设置
    this->installScrollViewportArea();
}

void DClusterSearchResultPage::fetchData(InputPacket &inpack)
{
    quint16 cmdCode = inpack.getCmdCode();
    DPRINT("-->SearchDevClusterResultPage receive data, the cmdCode is:0x%x", cmdCode);

    switch(cmdCode){
        case CommandCode::SEARCH_DCLUSTER:
            this->handleSearchClusterResult(inpack);
            break;
    }
    this->transferDataToAnotherPage(inpack, clusterJoinPage);
}

void DClusterSearchResultPage::handleSearchClusterResult(InputPacket &inpack)
{
    SAFE_DELETE(clusterInfoList);
    clusterInfoList = new DClusterInfoList;

    char* dataField = inpack.getPackData();
    quint16 dataLen = inpack.getPackDataLen();
    clusterInfoList->appendList(dataField, dataLen, Mi::SaveList);
    QHash<quint64, DClusterInfo*> clusterList = clusterInfoList->fetchList();

    QHashIterator<quint64, DClusterInfo*> iter(clusterList);
    while (iter.hasNext()) {
        iter.next();
        GroupButton* btn = new GroupButton(this);
        this->setGroupButton(btn, iter.value());
        clusterListLayOut->addWidget(btn);
        curFixedContentHeight += btn->height();
    }
    
    clusterListLayOut->addStretch(0);
    this->setBodyRealHeight(curFixedContentHeight);
}

void DClusterSearchResultPage::setGroupButton(GroupButton* btn, DClusterInfo* baseInfo)
{
    //设备按钮设置
    QPixmap pixmapHead(ImagePath::DEVICE_PORTRAIT);
    QPixmap pixmapRight(ImagePath::RIGHT_HOLLOW_ARROWS);

    btn->setIndexOne(baseInfo->getClusterID());
    btn->setPixMap(pixmapHead);
    btn->setText(baseInfo->getClusterName());
    btn->setRemarks(baseInfo->getDescribe());
    btn->setReserve(pixmapRight);
    btn->setFixedHeight(this->screenHeight()*0.12);
    btn->setFixedWidth(this->screenWidth());
    btn->setStyleSheet(SheetStyle::GROUPBUTTON_BOTTOMBORDER);
    connect(btn, SIGNAL(clicked()), this, SLOT(on_toolBtnCluster_clicked()));
}

DClusterInfo* DClusterSearchResultPage::findClusterInfoFromList(quint64 clusID)
{
    QHash<quint64, DClusterInfo*> infoList = clusterInfoList->fetchList();
    DClusterInfo* info = infoList.value(clusID);
    return info;
}

void DClusterSearchResultPage::displayConcretePage(BasePage *page)
{
    if(page == NULL) return;
    page->show();
    connect(page, SIGNAL(destroyed()), this, SLOT(on_deleteSubPages()));
    connect(page, SIGNAL(readyRead(quint16,char*,quint16)), this, SLOT(on_recvDataFromOtherPage(quint16,char*,quint16)));
}

void DClusterSearchResultPage::transferDataToAnotherPage(InputPacket &inpack, BasePage *basePage)
{
    if(basePage == NULL) return;
    basePage->fetchData(inpack);
}

void DClusterSearchResultPage::on_toolBtnCluster_clicked()
{
    GroupButton* btnThis = dynamic_cast<GroupButton*>(sender());
    quint64 clusID = btnThis->getIndexOne();
    DClusterInfo* info = this->findClusterInfoFromList(clusID);
    DClusterInfo* clusInfo = dynamic_cast<DClusterInfo*>(info);

    clusterJoinPage = new DClusterJoinPage(this, *clusInfo);
    this->displayConcretePage(clusterJoinPage);
}

void DClusterSearchResultPage::on_deleteSubPages()
{
    clusterJoinPage = NULL;
}

void DClusterSearchResultPage::on_recvDataFromOtherPage(quint16 cmdCode, char *data, quint16 dataLen)
{
    emit readyRead(cmdCode, data, dataLen);
}

void DClusterSearchResultPage::on_btnPrePage_clicked()
{
    this->hide();
    this->deleteLater();
}

DClusterSearchResultPage::~DClusterSearchResultPage()
{
    SAFE_DELETE(clusterInfoList);
}
