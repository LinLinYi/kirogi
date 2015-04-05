#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

#include "CheckButton.h"
#include "DClusterUserInfoList.h"
#include "DClusterAddManagerPage.h"
#include "UserProtocol.hpp"
#include "AssemblyData.h"
#include "HintMsgPage.h"
#include "NavigationBar.h"
#include "InputPacket.h"
#include "GlobalDefine.hpp"
#include "ImageConfig.hpp"
#include "LoadingPage.h"
#include "SheetStyleConfig.hpp"
#include "DClusterSetManagerPage.h"

DClusterSetManagerPage::DClusterSetManagerPage(QWidget* parent, quint64 clusterID)
    : BasePage(parent)
    , curClusterID(clusterID)
    , userInfoList(NULL)
    , loadingPage(NULL)
    , clusterAddManagerPage(NULL)
{
    this->initializeWidget();
}

void DClusterSetManagerPage::initializeWidget()
{
    //提示信息页面操作
    hintMsgPage = new HintMsgPage(this);
    hintMsgPage->hide();
    connect(hintMsgPage, SIGNAL(submitClicked(bool)), this, SLOT(on_hintMsgPage(bool)));

    //导航栏
    QString strTitle = tr("设置管理员");
    QString strRightBtn = tr("完成");
    navigationBar = new NavigationBar(this);
    navigationBar->setTitleText(strTitle);
    navigationBar->setRightText(strRightBtn);
    connect(navigationBar, SIGNAL(rightClicked()), this, SLOT(on_btnComplete_clicked()));

    QHBoxLayout* pHLTop = new QHBoxLayout();
    pHLTop->addWidget(navigationBar);
    pHLTop->setSpacing(0);
    pHLTop->setMargin(0);
    this->setTopbarLayout(pHLTop);

    //添加管理员按钮
    btnAddManager = new QPushButton(this);
    btnAddManager->setText(tr("设置管理员"));
    btnAddManager->setFixedHeight(this->bottomBarHeight());
    btnAddManager->setStyleSheet("background-color:white; border-radius:3px; font:15px; color:#0090ff;");
    connect(btnAddManager, SIGNAL(clicked()), this, SLOT(on_btnAddManager_clicked()));

    QVBoxLayout* vbAddManager = new QVBoxLayout;
    vbAddManager->addWidget(btnAddManager);
    vbAddManager->setSpacing(0);
    vbAddManager->setMargin(0);
    this->setBottombarLayout(vbAddManager);

    //群列表
    managerLayout = new QVBoxLayout;
    managerLayout->setMargin(0);
    managerLayout->setSpacing(0);
    managerLayout->setAlignment(Qt::AlignTop);
    this->setBodyPartLayout(managerLayout);
    
    this->setBodyScreenHeight(scrollAreaHasBottomBarHeight());
    this->installScrollViewportArea();
    timerID = this->startTimer(ConstNum::MAX_TIME_INTERVAL);
}

void DClusterSetManagerPage::fetchData(InputPacket &inpack)
{
    quint16 cmdCode = inpack.getCmdCode();
    switch(cmdCode)
    {
        case CommandCode::NET_ERROR:
            this->handleNetError();
            break;
        case CommandCode::FETCH_DCLUSTER_USER_LIST:
            this->handleDevClusterUserListInfo(inpack);
            break;
        case CommandCode::DCLUSTER_SETTING_USER_AUTHORIZE:
            this->handleDevClusterAuthorizedInfo(inpack);
            break;
    }
}

void DClusterSetManagerPage::timerEvent(QTimerEvent *)
{
    AssemblyData assemblyData;
    assemblyData.append(curClusterID);
    char* data = assemblyData.getAssemblyData();
    quint16 dataLen = assemblyData.getAssemblyDataLen();
    emit readyRead(CommandCode::FETCH_DCLUSTER_USER_LIST, data, dataLen);
    this->killTimer(timerID);
}

void DClusterSetManagerPage::handleHintInfo(QString& text)
{
    hintMsgPage->setText(text);
    hintMsgPage->show();
    SAFE_DELETE(loadingPage);
}

void DClusterSetManagerPage::handleNetError()
{
    if(loadingPage == NULL) return;
    QString strText = tr("网络错误,请重试");
    handleHintInfo(strText);
}

void DClusterSetManagerPage::handleDevClusterUserListInfo(InputPacket &inpack)
{
    if(clusterAddManagerPage != NULL){
        clusterAddManagerPage->fetchData(inpack);
        return;
    }

    char* data = inpack.getPackData();
    quint16 dataLen = inpack.getPackDataLen();

    SAFE_DELETE(userInfoList);
    userInfoList = new DClusterUserInfoList;
    userInfoList->appendList(data, dataLen);

    QHash<quint64, DClusterUserInfo*> infoList = userInfoList->fetchList();
    QHashIterator<quint64, DClusterUserInfo*> iter(infoList);
    while(iter.hasNext())
    {
        iter.next();
        DClusterUserInfo* info = iter.value();
        this->appendManagerToList(info);
    }
}

void DClusterSetManagerPage::handleDevClusterAuthorizedInfo(InputPacket& inpack)
{
    quint8 ansCode = inpack.getAnswerCode();
    if(clusterAddManagerPage != NULL){
        clusterAddManagerPage->fetchData(inpack);
        if(ansCode == Mi::Success){//添加管理员成功
            quint64 managerID = clusterAddManagerPage->getManagerID();

            QHash<quint64, DClusterUserInfo*> infoList = userInfoList->fetchList();
            QHashIterator<quint64, DClusterUserInfo*> iter(infoList);
            while(iter.hasNext())
            {
                iter.next();
                DClusterUserInfo* userInfo = dynamic_cast<DClusterUserInfo*>(iter.value());
                if(managerID == userInfo->getUserID()){
                    userInfo->setUserRole(Mi::SeniorManager);
                    this->appendManagerToList(userInfo);
                    return;
                }
            }
        }
        return;
    }

    QString strText;
    if(ansCode == Mi::Success){
        strText = tr("取消管理员成功");
        QHashIterator<quint64, CheckButton*> iter(btnCheckList);
        while(iter.hasNext())
        {
            iter.next();
            CheckButton* btn = iter.value();
            if(btn->getUserID() == curCancelManagerID){
                managerLayout->removeWidget(btn);
                SAFE_DELETE(btn);
            }
        }
    }
    else if(ansCode == Mi::Failure)
        strText = tr("取消管理员失败, 请重试");

    this->handleHintInfo(strText);
}

void DClusterSetManagerPage::appendManagerToList(DClusterUserInfo *info)
{
    if(info->getUserRole() == Mi::SySManager || info->getUserRole() == Mi::SeniorManager)
    {
        CheckButton* btn = new CheckButton(this);
        btn->setUserID(info->getUserID());
        btn->setUserName(info->getUserName());
        btn->setClusterID(info->getClusterID());

        if(info->getUserRole() == Mi::SySManager){
            btn->setReviewText(tr("群主"));
            btn->setReviewEnable(false);
        }else{
            btn->setReviewText(tr("取消管理员"));
            connect(btn, SIGNAL(reviewClicked()), this, SLOT(on_btnCancelManager_clicked()));
        }

        QPixmap pixmap(ImagePath::USER_PORTRAIT);
        pixmap.setDevicePixelRatio(2.0);
        btn->setPixMap(pixmap);
        btn->setFixedHeight(this->screenHeight()*0.1);
        btn->setFixedWidth(this->screenWidth());
        btn->setAttribute(Qt::WA_LayoutUsesWidgetRect);
        btn->setStyleSheet(SheetStyle::CHECKBUTTON_BOTTOM_BORDER);
        btnCheckList.insert(info->getUserID(), btn);

        if(managerLayout->count() == 0){
            managerLayout->addWidget(btn);
            managerLayout->addStretch(0);
        }
        else{
            if(info->getUserRole() == Mi::SySManager)
                managerLayout->insertWidget(0, btn);
            else
                managerLayout->insertWidget(managerLayout->count() - 1, btn);
        }
    }
}

void DClusterSetManagerPage::displayConcretePage(BasePage *page)
{
    if(page == NULL) return;
    page->show();
    connect(page, SIGNAL(destroyed()), this, SLOT(on_deleteSubPages()));
    connect(page, SIGNAL(readyRead(quint16,char*,quint16)), this, SLOT(on_recvDataFromOtherPage(quint16,char*,quint16)));
}

void DClusterSetManagerPage::on_deleteSubPages()
{
    clusterAddManagerPage = NULL;
}

void DClusterSetManagerPage::on_btnComplete_clicked()
{
    this->hide();
    this->deleteLater();
}

void DClusterSetManagerPage::on_btnAddManager_clicked()
{
    clusterAddManagerPage = new DClusterAddManagerPage(this, curClusterID);
    this->displayConcretePage(clusterAddManagerPage);
}

void DClusterSetManagerPage::on_btnCancelManager_clicked()
{
    CheckButton* btn = dynamic_cast<CheckButton*>(sender());
    btn->setReviewText(tr("取消管理员"));
    quint8 role = Mi::Operator;
    curCancelManagerID = btn->getUserID();

    AssemblyData assemblyData;
    assemblyData.append(curClusterID);
    assemblyData.append(role);
    assemblyData.append(curCancelManagerID);

    char* data = assemblyData.getAssemblyData();
    quint16 dataLen = assemblyData.getAssemblyDataLen();
    emit readyRead(CommandCode::DCLUSTER_SETTING_USER_AUTHORIZE, data, dataLen);

    //页面加载窗口
    if(loadingPage == NULL){
        loadingPage = new LoadingPage(this);
        loadingPage->show();
    }
}

void DClusterSetManagerPage::on_recvDataFromOtherPage(quint16 cmdCode, char *data, quint16 dataLen)
{
    emit readyRead(cmdCode, data, dataLen);
}

void DClusterSetManagerPage::on_hintMsgPage(bool isJump)
{
    if(!isJump)
        hintMsgPage->hide();
}

DClusterSetManagerPage::~DClusterSetManagerPage()
{
    SAFE_DELETE(userInfoList);
}
