#include <QVBoxLayout>

#include "UClusterInfoList.h"
#include "GroupButton.h"
#include "UClusterJoinPage.h"
#include "NavigationBar.h"
#include "UserProtocol.hpp"
#include "ImageConfig.hpp"
#include "InputPacket.h"
#include "HintMsgPage.h"
#include "LoadingPage.h"
#include "SheetStyleConfig.hpp"
#include "UClusterSearchResultPage.h"

UClusterSearchResultPage::UClusterSearchResultPage(QWidget* parent)
    :BasePage(parent)
    , curFixedContentHeight(0)
    , clusterInfoList(NULL)
    , clusterJoinPage(NULL)
{
    this->initializeWidget();
}

void UClusterSearchResultPage::initializeWidget()
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

void UClusterSearchResultPage::fetchData(InputPacket &inpack)
{
    quint16 cmdCode = inpack.getCmdCode();
    DPRINT("-->SearchUserClusterResultPage receive data, the cmdCode is:0x%x", cmdCode);

    switch(cmdCode){
        case CommandCode::SEARCH_UCLUSTER:
            this->handleSearchClusterResult(inpack);
            break;
    }
    this->transferDataToAnotherPage(inpack, clusterJoinPage);
}

void UClusterSearchResultPage::handleSearchClusterResult(InputPacket &inpack)
{
    SAFE_DELETE(clusterInfoList);
    clusterInfoList = new UClusterInfoList;

    char* dataField = inpack.getPackData();
    quint16 dataLen = inpack.getPackDataLen();
    clusterInfoList->appendList(dataField, dataLen, Mi::SaveList);
    QHash<quint64, UClusterInfo*> clusterList = clusterInfoList->fetchList();

    QHashIterator<quint64, UClusterInfo*> iter(clusterList);
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

void UClusterSearchResultPage::setGroupButton(GroupButton* btn, UClusterInfo* baseInfo)
{
    //设备按钮设置
    QPixmap pixmapHead(ImagePath::UCLUSTER_PORTRAIT);
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

UClusterInfo* UClusterSearchResultPage::findClusterInfoFromList(quint64 clusID)
{
    QHash<quint64, UClusterInfo*> infoList = clusterInfoList->fetchList();
    UClusterInfo* info = infoList.value(clusID);
    return info;
}

void UClusterSearchResultPage::displayConcretePage(BasePage *page)
{
    if(page == NULL) return;
    page->show();
    connect(page, SIGNAL(destroyed()), this, SLOT(on_deleteSubPages()));
    connect(page, SIGNAL(readyRead(quint16,char*,quint16)), this, SLOT(on_recvDataFromOtherPage(quint16,char*,quint16)));
}

void UClusterSearchResultPage::transferDataToAnotherPage(InputPacket &inpack, BasePage *basePage)
{
    if(basePage == NULL) return;
    basePage->fetchData(inpack);
}

void UClusterSearchResultPage::on_toolBtnCluster_clicked()
{
    GroupButton* btnThis = dynamic_cast<GroupButton*>(sender());
    quint64 clusID = btnThis->getIndexOne();
    UClusterInfo* info = this->findClusterInfoFromList(clusID);
    UClusterInfo* clusInfo = dynamic_cast<UClusterInfo*>(info);

    clusterJoinPage = new UClusterJoinPage(this, *clusInfo);
    this->displayConcretePage(clusterJoinPage);
}

void UClusterSearchResultPage::on_deleteSubPages()
{
    clusterJoinPage = NULL;
}

void UClusterSearchResultPage::on_recvDataFromOtherPage(quint16 cmdCode, char *data, quint16 dataLen)
{
    emit readyRead(cmdCode, data, dataLen);
}

void UClusterSearchResultPage::on_btnPrePage_clicked()
{
    this->hide();
    this->deleteLater();
}

UClusterSearchResultPage::~UClusterSearchResultPage()
{
    SAFE_DELETE(clusterInfoList);
}
