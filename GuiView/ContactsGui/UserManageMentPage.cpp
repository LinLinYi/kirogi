#include <QHBoxLayout>
#include <QVBoxLayout>

#include "UserManagementPage.h"
#include "TouchScroller.h"
#include "UserProtocol.hpp"
#include "ListWidget.h"
#include "ImageConfig.hpp"
#include "NavigationBar.h"
#include "AssemblyData.h"
#include "GlobalDefine.hpp"
#include "SheetStyleConfig.hpp"
#include "InputPacket.h"
#include "GroupButton.h"
#include "UserJoinPage.h"
#include "SearchBar.h"
#include "UClusterSearchPage.h"
#include "UClusterCreatePage.h"
#include "LoadingPage.h"
#include "HintMsgPage.h"

UserManagementPage::UserManagementPage(QWidget* parent)
    : BasePage(parent)
    , loadingPage(NULL)
    , userJoinPage(NULL)
    , clusterSearchPage(NULL)
    , clusterCreatePage(NULL)
{
    this->initializeWidget();
}

void UserManagementPage::initializeWidget()
{
    //提示信息页面操作;
    hintMsgPage = new HintMsgPage(this);
    hintMsgPage->hide();
    connect(hintMsgPage, SIGNAL(submitClicked(bool)), this, SLOT(on_hintMsgPage(bool)));

    //导航栏
    QString strTitle = tr("添加");
    QString strLeftBtn = tr("联系人");
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
    QString searchPlaceText = QString(tr("好友账号/邮箱/手机号码"));
    searchBar = new SearchBar(this);
    searchBar->setPlaceholderText(searchPlaceText);
    connect(searchBar, SIGNAL(searchInputCompleted(QString&)), this, SLOT(on_searchBar(QString&)));

    //扫一扫
    //btnSweep = new GroupButton(this);
    //btnSweep->setStyleSheet(SheetStyle::GROUPBUTTON_UPANDDOWNBORDER);

    //QPixmap sweepPixmap(ImagePath::SWEEP);
    //sweepPixmap.setDevicePixelRatio(2.0);
    //this->setGroupButton(btnSweep, sweepPixmap, QString(tr("扫一扫")));
    //connect(btnSweep, SIGNAL(clicked()), SLOT(on_btnSweep_clicked()));

    //查找群组
    btnSearchCluster = new GroupButton(this);
    btnSearchCluster->setStyleSheet(SheetStyle::GROUPBUTTON_BOTTOMBORDER);

    QPixmap addClusterPixmap(ImagePath::SEARCH_USER_CLUSTER);
    addClusterPixmap.setDevicePixelRatio(2.0);
    this->setGroupButton(btnSearchCluster, addClusterPixmap, QString(tr("查找用户群")));
    connect(btnSearchCluster, SIGNAL(clicked()), SLOT(on_btnSearchCluster_clicked()));

    //创建群组
    btnCreateCluster = new GroupButton(this);
    btnCreateCluster->setStyleSheet(SheetStyle::GROUPBUTTON_NOBORDER);

    QPixmap createClusterPixmap(ImagePath::CREATE_USER_CLUSTER);
    createClusterPixmap.setDevicePixelRatio(2.0);
    this->setGroupButton(btnCreateCluster, createClusterPixmap, QString(tr("创建用户群")));
    connect(btnCreateCluster, SIGNAL(clicked()), SLOT(on_btnCreateCluster_clicked()));

    ListWidget* clusterOper = new ListWidget(this);
    clusterOper->setStyleSheet(SheetStyle::LISTWIDGET);
    clusterOper->addWidget(btnSearchCluster);
    clusterOper->addWidget(btnCreateCluster);
    clusterOper->setFixedWidth(this->screenWidth());
    clusterOper->setContentsMargins(this->screenWidth()*0.05,0,0,0);

    QVBoxLayout* vblTotalLayout = new QVBoxLayout;
    vblTotalLayout->addSpacing(this->screenHeight()*0.026);
    vblTotalLayout->addWidget(searchBar);
    //vbOperLayout->addWidget(btnSweep);
    //vbOperLayout->addSpacing(this->screenHeight()*0.037);
    vblTotalLayout->addWidget(clusterOper);
    vblTotalLayout->setSpacing(this->screenHeight()*0.037);
    vblTotalLayout->addStretch(0);
    vblTotalLayout->setMargin(0);
    this->setBodyPartLayout(vblTotalLayout);

    //屏幕触摸滚动设置
    this->installScrollViewportArea();
}

void UserManagementPage::fetchData(InputPacket& inpack)
{
    quint16 cmdCode = inpack.getCmdCode();
    DPRINT("-->UserManagementPage receive data, the cmdCode is:0x%x", cmdCode);

    switch(cmdCode){
    case CommandCode::NET_ERROR:
        this->handleNetError();
        break;
    case CommandCode::SEARCH_USER:
        this->handleUserSearchResult(inpack);
        break;
    }
    this->transferDataToAnotherPage(inpack, userJoinPage);
    this->transferDataToAnotherPage(inpack,clusterCreatePage);
    this->transferDataToAnotherPage(inpack,clusterSearchPage);
}

void UserManagementPage::handleNetError()
{
    if(loadingPage == NULL) return;
    QString lblText = tr("网络错误,请重试");
    this->handleHintInfo(lblText);
}

void UserManagementPage::handleHintInfo(QString& text)
{
    hintMsgPage->setText(text);
    hintMsgPage->show();
    SAFE_DELETE(loadingPage);
}

void UserManagementPage::handleUserSearchResult(InputPacket &inpack)
{
    quint8 ansCode = inpack.getAnswerCode();
    if(ansCode == Mi::Success){
        SAFE_DELETE(loadingPage);
        hintMsgPage->hide();
        userJoinPage = new UserJoinPage(this);
        userJoinPage->fetchData(inpack);
        this->displayConcretePage(userJoinPage);
    }else if(ansCode == Mi::Absent){
        QString strText = QString("该用户不存在, 请检查！");
        this->handleHintInfo(strText);
    }
}

void UserManagementPage::displayConcretePage(BasePage *page)
{
    if(page == NULL) return;
    page->show();
    connect(page, SIGNAL(destroyed()), this, SLOT(on_deleteSubPages()));
    connect(page, SIGNAL(readyRead(quint16,char*,quint16)), this, SLOT(on_recvDataFromOtherPage(quint16,char*,quint16)));
}

void UserManagementPage::transferDataToAnotherPage(InputPacket &inpack, BasePage* basePage)
{
    if(basePage == NULL) return;
    basePage->fetchData(inpack);
}

void UserManagementPage::setGroupButton(GroupButton* groupButton, QPixmap photo, QString title)
{
    groupButton->setPixMap(photo);
    groupButton->setText(title);

    QPixmap pixmap(ImagePath::RIGHT_HOLLOW_ARROWS);
    groupButton->setReserve(pixmap);
    groupButton->setFixedHeight(this->screenHeight()*0.0775);
    groupButton->setAttribute(Qt::WA_LayoutUsesWidgetRect);
    groupButton->setContentsMargins(0, 0, this->screenWidth()*0.03, 0);
}

void UserManagementPage::on_searchBar(QString &searchValue)
{
    //搜索内容（中文、英文）
    AssemblyData assemblyData;
    assemblyData.append(searchValue);
    char* data = assemblyData.getAssemblyData();
    quint16 dataLen = assemblyData.getAssemblyDataLen();
    emit readyRead(CommandCode::SEARCH_USER, data, dataLen);

    if(loadingPage == NULL){
        loadingPage = new LoadingPage(this);
        loadingPage->show();
    }
}

void UserManagementPage::on_btnSweep_clicked()
{

}

void UserManagementPage::on_btnSearchCluster_clicked()
{
    SAFE_DELETE(clusterSearchPage);
    clusterSearchPage = new UClusterSearchPage(this);
    this->displayConcretePage(clusterSearchPage);
}

void UserManagementPage::on_btnCreateCluster_clicked()
{
    clusterCreatePage = new UClusterCreatePage(this);
    this->displayConcretePage(clusterCreatePage);
}

void UserManagementPage::on_deleteSubPages()
{
    clusterCreatePage = NULL;
    clusterSearchPage = NULL;
    userJoinPage = NULL;
}

void UserManagementPage::on_recvDataFromOtherPage(quint16 cmdCode, char *packData, quint16 dataLen)
{
    if(cmdCode == CommandCode::BACK_CONTACT_PAGE){
        this->hide();
        this->deleteLater();
        return;
    }
     emit readyRead(cmdCode, packData, dataLen);
}

void UserManagementPage::on_btnPrePage_clicked()
{
    this->hide();
    this->deleteLater();
}

void UserManagementPage::on_hintMsgPage(bool isJump)
{
    if(!isJump){
        hintMsgPage->hide();
        return;
    }
}

UserManagementPage::~UserManagementPage()
{

}
