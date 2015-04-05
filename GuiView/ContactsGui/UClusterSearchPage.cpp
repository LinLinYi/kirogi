#include <QVBoxLayout>
#include <QHBoxLayout>

#include "HintMsgPage.h"
#include "ImageConfig.hpp"
#include "NavigationBar.h"
#include "AssemblyData.h"
#include "LoadingPage.h"
#include "UserProtocol.hpp"
#include "GlobalDefine.hpp"
#include "UClusterSearchPage.h"
#include "InputPacket.h"
#include "SearchBar.h"
#include "UClusterSearchResultPage.h"

UClusterSearchPage::UClusterSearchPage(QWidget* parent)
    :BasePage(parent)
    , loadingPage(NULL)
    ,clusterSearchResultPage(NULL)
{
    this->initializeWidget();
}

void UClusterSearchPage::initializeWidget()
{
    //加载、提示信息页面操作
    hintMsgpage = new HintMsgPage(this);
    hintMsgpage->hide();
    connect(hintMsgpage, SIGNAL(submitClicked(bool)), this, SLOT(on_hintMsgpage(bool)));

    //导航栏
    QString strTitle = tr("查找用户群");
    QString strLeftBtn = tr("添加");
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
    QString searchPlaceText = QString(tr("群名称/关键词"));
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

void UClusterSearchPage::fetchData(InputPacket &inpack)
{
    quint16 cmdCode = inpack.getCmdCode();
    DPRINT("-->SearchUserClusterPage receive data, the cmdCode is:0x%x", cmdCode);

    switch(cmdCode)
    {
        case CommandCode::NET_ERROR:
            this->handleNetError();
        case CommandCode::SEARCH_UCLUSTER:
            this->handleSearchDevCluster(inpack);
            break;
    }
    this->transferDataToAnotherPage(inpack, clusterSearchResultPage);
}

void UClusterSearchPage::handleNetError()
{
    if(loadingPage == NULL)  return;
    QString lblText = tr("网络错误,请重试");
    this->handleHintInfo(lblText);
}

void UClusterSearchPage::handleHintInfo(QString& text)
{
    hintMsgpage->setText(text);
    hintMsgpage->show();
    SAFE_DELETE(loadingPage);
}

void UClusterSearchPage::handleSearchDevCluster(InputPacket &inpack)
{
    quint8 ansCode = inpack.getAnswerCode();
    if(ansCode == Mi::Absent){
        QString strText = tr("未找到符合搜索条件的群");
        this->handleHintInfo(strText);
        return;
    }else if(ansCode == Mi::Success){
        SAFE_DELETE(loadingPage);
        clusterSearchResultPage = new UClusterSearchResultPage(this);
        this->displayConcretePage(clusterSearchResultPage);
    }
}

void UClusterSearchPage::displayConcretePage(BasePage *page)
{
    if(page == NULL) return;
    page->show();
    connect(page, SIGNAL(destroyed()), this, SLOT(on_deleteSubPages()));
    connect(page, SIGNAL(readyRead(quint16,char*,quint16)), this, SLOT(on_recvDataFromOtherPage(quint16,char*,quint16)));
}

void UClusterSearchPage::transferDataToAnotherPage(InputPacket &inpack, BasePage *basePage)
{
    if(basePage == NULL) return;
    basePage->fetchData(inpack);
}

void UClusterSearchPage::on_searchBar(QString& text)
{
    //搜索内容
    QString searchText = text;
    AssemblyData assemblyData;
    assemblyData.append(searchText);
    char* data = assemblyData.getAssemblyData();
    quint16 dataLen = assemblyData.getAssemblyDataLen();
    emit readyRead(CommandCode::SEARCH_UCLUSTER, data, dataLen);

    //页面加载窗口
    if(loadingPage == NULL){
        loadingPage = new LoadingPage(this);
        loadingPage->show();
    }
}

void UClusterSearchPage::on_hintMsgpage(bool isJump)
{
    if(!isJump)
        hintMsgpage->hide();
}

void UClusterSearchPage::on_deleteSubPages()
{
    clusterSearchResultPage = NULL;
}

void UClusterSearchPage::on_recvDataFromOtherPage(quint16 cmdCode, char* data, quint16 dataLen)
{
    emit readyRead(cmdCode, data, dataLen);
}

void UClusterSearchPage::on_btnPrePage_clicked()
{
    this->hide();
    this->deleteLater();
}

UClusterSearchPage::~UClusterSearchPage()
{

}
