#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>

#include "InputPacket.h"
#include "SearchBar.h"
#include "UserProtocol.hpp"
#include "HintMsgPage.h"
#include "ImageConfig.hpp"
#include "NavigationBar.h"
#include "GlobalDefine.hpp"
#include "AssemblyData.h"
#include "LoadingPage.h"
#include "SheetStyleConfig.hpp"
#include "DClusterInviteUserPage.h"
#include "DClusterSearchUserPage.h"

DClusterSearchUserPage::DClusterSearchUserPage(QWidget* parent, quint64 clusterID)
    :BasePage(parent)
    , curClusterID(clusterID)
    , loadingPage(NULL)
    , inviteUserPage(NULL)
{
    this->initializeWidget();
}

void DClusterSearchUserPage::initializeWidget()
{
    //提示信息页面操作
    hintMsgpage = new HintMsgPage(this);
    hintMsgpage->hide();
    connect(hintMsgpage, SIGNAL(submitClicked(bool)), this, SLOT(on_hintMsgpage(bool)));

    //导航栏
    QString strTitle = tr("查找");
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

    //搜索标签
    QLabel* lblSearchBuddyHint = new QLabel(this);
    lblSearchBuddyHint->setText(tr("请输入成员名称/关键词"));
    lblSearchBuddyHint->setAlignment(Qt::AlignLeft);
    lblSearchBuddyHint->setStyleSheet("color:rgb(122,123,128); font:16px;");
    lblSearchBuddyHint->setContentsMargins(this->screenWidth()*0.047, 0, 0, this->screenWidth()*0.01);

    //搜索栏
    QString searchPlaceText = QString(tr("成员名称/关键词"));
    searchBar = new SearchBar(this);
    searchBar->setPlaceholderText(searchPlaceText);
    connect(searchBar, SIGNAL(searchInputCompleted(QString&)), this, SLOT(on_searchBar(QString&)));

    QVBoxLayout* vbSearchLayout = new QVBoxLayout;
    vbSearchLayout->addSpacing(this->screenHeight()*0.026);
    vbSearchLayout->addWidget(lblSearchBuddyHint);
    vbSearchLayout->addWidget(searchBar);
    vbSearchLayout->setSpacing(0);
    vbSearchLayout->setMargin(0);
    vbSearchLayout->addStretch(0);
    this->setBodyPartLayout(vbSearchLayout);

    //屏幕触摸滚动设置
    this->installScrollViewportArea();
}

void DClusterSearchUserPage::fetchData(InputPacket &inpack)
{
    quint16 cmdCode = inpack.getCmdCode();
    DPRINT("-->DevClusterSearchUserPage receive data, the cmdCode is:0x%x", cmdCode);

    switch(cmdCode)
    {
        case CommandCode::NET_ERROR:
            this->handleNetError();
            break;
        case CommandCode::DCLUSTER_SEARCH_USER:
            this->handleSearchUserResult(inpack);
            break;
    }
    this->transferDataToAnotherPage(inpack, inviteUserPage);
}

void DClusterSearchUserPage::handleNetError()
{
    if(loadingPage == NULL) return;
    QString strText = tr("网络错误,请重试");
    this->handleHintInfo(strText);
}

void DClusterSearchUserPage::handleSearchUserResult(InputPacket &inpack)
{
    quint8 ansCode = inpack.getAnswerCode();
    if(ansCode == Mi::Absent){
        QString strText = tr("未找到符合搜索条件的成员");
        this->handleHintInfo(strText);
        return;
    }
    else if(ansCode == Mi::Success){
        SAFE_DELETE(loadingPage);
        inviteUserPage = new DClusterInviteUserPage(this, curClusterID);
        this->displayConcretePage(inviteUserPage);
    }
}

void DClusterSearchUserPage::displayConcretePage(BasePage *page)
{
    if(page == NULL) return;
    page->show();
    connect(page, SIGNAL(destroyed()), this, SLOT(on_deleteSubPages()));
    connect(page, SIGNAL(readyRead(quint16,char*,quint16)), this, SLOT(on_recvDataFromOtherPage(quint16,char*,quint16)));
}

void DClusterSearchUserPage::transferDataToAnotherPage(InputPacket &inpack, BasePage *basePage)
{
    if(basePage == NULL) return;
    basePage->fetchData(inpack);
}

void DClusterSearchUserPage::on_searchBar(QString& text)
{
    if(text.isEmpty()){
        QString strText = tr("请输入邀请的成员名称");
        handleHintInfo(strText);
        return;
    }

    QString searchText = text;
    AssemblyData assemblyData;
    assemblyData.append(searchText);
    char* data = assemblyData.getAssemblyData();
    quint16 dataLen = assemblyData.getAssemblyDataLen();
    emit readyRead(CommandCode::DCLUSTER_SEARCH_USER, data, dataLen);

    if(loadingPage == NULL){
        loadingPage = new LoadingPage(this);
        loadingPage->show();
    }
}

void DClusterSearchUserPage::handleHintInfo(QString& text)
{
    hintMsgpage->setText(text);
    hintMsgpage->show();
    SAFE_DELETE(loadingPage);
}

void DClusterSearchUserPage::on_hintMsgpage(bool isJump)
{
    if(!isJump)
        hintMsgpage->hide();
}

void DClusterSearchUserPage::on_deleteSubPages()
{
    inviteUserPage = NULL;
}

void DClusterSearchUserPage::on_recvDataFromOtherPage(quint16 cmdCode, char* data, quint16 dataLen)
{
    emit readyRead(cmdCode, data, dataLen);
}

void DClusterSearchUserPage::on_btnPrePage_clicked()
{
    this->hide();
    this->deleteLater();
}

DClusterSearchUserPage::~DClusterSearchUserPage()
{

}

