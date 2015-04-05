#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

#include "UserProtocol.hpp"
#include "HintMsgPage.h"
#include "NavigationBar.h"
#include "ImageConfig.hpp"
#include "GlobalDefine.hpp"
#include "InputPacket.h"
#include "AssemblyData.h"
#include "LoadingPage.h"
#include "ImageConfig.hpp"
#include "SearchBar.h"
#include "DClusterInfo.h"
#include "DClusterTransferSearchResultPage.h"
#include "DClusterTransferSearchPage.h"

DClusterTransferSearchPage::DClusterTransferSearchPage(QWidget* parent, quint64 clusterID) :
    BasePage(parent)
  , curClusterID(clusterID)
  , loadingPage(NULL)
  , clusterTransferSearchResultPage(NULL)
{
    this->initializeWidget();
}

void DClusterTransferSearchPage::initializeWidget()
{
    //提示信息页面操作
    hintMsgpage = new HintMsgPage(this);
    hintMsgpage->hide();
    connect(hintMsgpage, SIGNAL(submitClicked(bool)), this, SLOT(on_hintMsgpage(bool)));

    //导航栏
    QString strTitle = "移交到设备群";
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

    //搜索栏
    QString searchPlaceText = QString("设备群名称/关键词");
    searchBar = new SearchBar(this);
    searchBar->setPlaceholderText(searchPlaceText);
    connect(searchBar, SIGNAL(searchInputCompleted(QString&)), this, SLOT(on_searchBar(QString&)));

    QVBoxLayout* vbSearchLayout = new QVBoxLayout;
    vbSearchLayout->addSpacing(this->screenHeight()*0.026);
    vbSearchLayout->addWidget(searchBar);
    vbSearchLayout->setSpacing(0);
    vbSearchLayout->setMargin(0);
    vbSearchLayout->addStretch(0);
    this->setBodyPartLayout(vbSearchLayout);

    //屏幕触摸滚动设置
    this->installScrollViewportArea();
}

void DClusterTransferSearchPage::fetchData(InputPacket &inpack)
{
    quint16 cmdCode = inpack.getCmdCode();
    DPRINT("-->DevClusterTransferDeviceSearchPage receive data, the cmdCode is:0x%x", cmdCode);

    switch(cmdCode)
    {
        case CommandCode::NET_ERROR:
            this->handleNetError();
            break;
        case CommandCode::SEARCH_DCLUSTER:
            this->handleSearchClusterResult(inpack);
            break;
    }
    this->transferDataToAnotherPage(inpack, clusterTransferSearchResultPage);
}

void DClusterTransferSearchPage::setCurClusterName(QString &name)
{
    curClusterName = name;
}

void DClusterTransferSearchPage::handleNetError()
{
    if(loadingPage == NULL)
        return;

    QString lblText = "网络错误, 请重试";
    this->handleHintInfo(lblText);
}

void DClusterTransferSearchPage::handleHintInfo(QString& text)
{
    hintMsgpage->setText(text);
    hintMsgpage->show();
    SAFE_DELETE(loadingPage);
}

void DClusterTransferSearchPage::handleSearchClusterResult(InputPacket &inpack)
{
    quint8 ansCode = inpack.getAnswerCode();
    if(ansCode == Mi::Absent){
        QString strText = "未找到符合搜索条件的群";
        this->handleHintInfo(strText);
        return;
    }
    else if(ansCode == Mi::Success){
        SAFE_DELETE(loadingPage);
        clusterTransferSearchResultPage = new DClusterTransferSearchResultPage(this, curClusterID);
        this->displayConcretePage(clusterTransferSearchResultPage);
    }
}

void DClusterTransferSearchPage::displayConcretePage(BasePage *page)
{
    if(page == NULL) return;
    page->show();
    connect(page, SIGNAL(destroyed()), this, SLOT(on_deleteSubPages()));
    connect(page, SIGNAL(readyRead(quint16,char*,quint16)), this, SLOT(on_recvDataFromOtherPage(quint16,char*,quint16)));
}

void DClusterTransferSearchPage::transferDataToAnotherPage(InputPacket &inpack, BasePage *basePage)
{
    if(basePage == NULL) return;
    basePage->fetchData(inpack);
}

void DClusterTransferSearchPage::on_searchBar(QString& text)
{
    if(text.isEmpty()){
        QString strText = "请输入群名称.";
        this->handleHintInfo(strText);
        return;
    }

    if(curClusterName == text){
        QString strText = "不能移交到设备所在群.";
        this->handleHintInfo(strText);
        return;
    }

    AssemblyData assemblyData;
    assemblyData.append(text);
    char* data = assemblyData.getAssemblyData();
    quint16 dataLen = assemblyData.getAssemblyDataLen();
    emit readyRead(CommandCode::SEARCH_DCLUSTER, data, dataLen);

    if(loadingPage == NULL){
        loadingPage = new LoadingPage(this);
        loadingPage->show();
    }
}

void DClusterTransferSearchPage::on_hintMsgpage(bool isJump)
{
    if(!isJump)
        hintMsgpage->hide();
}

void DClusterTransferSearchPage::on_deleteSubPages()
{
    clusterTransferSearchResultPage = NULL;
}

void DClusterTransferSearchPage::on_recvDataFromOtherPage(quint16 cmdCode, char* data, quint16 dataLen)
{
    if(cmdCode == CommandCode::BACK_CLUSTER_MANAGE_PAGE){
        this->hide();
        this->deleteLater();
        return;
    }
    emit readyRead(cmdCode, data, dataLen);
}

void DClusterTransferSearchPage::on_btnPrePage_clicked()
{
    this->hide();
    this->deleteLater();
}

DClusterTransferSearchPage::~DClusterTransferSearchPage()
{

}

