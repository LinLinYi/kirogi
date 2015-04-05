#include <QVBoxLayout>
#include <QHBoxLayout>

#include "HintMsgPage.h"
#include "LoadingPage.h"
#include "NavigationBar.h"
#include "UserProtocol.hpp"
#include "InputPacket.h"
#include "GroupButton.h"
#include "ImageConfig.hpp"
#include "AssemblyData.h"
#include "SheetStyleConfig.hpp"
#include "DClusterAddManagerPage.h"

DClusterAddManagerPage::DClusterAddManagerPage(QWidget* parent, quint64 clusterID)
    :BasePage(parent)
    , curClusterID(clusterID)
    , curManagerID(0)
    , loadingPage(NULL)
{
    initializeWidget();
}

void DClusterAddManagerPage::initializeWidget()
{
    //提示信息页面操作
    hintMsgPage = new HintMsgPage(this);
    hintMsgPage->hide();
    connect(hintMsgPage, SIGNAL(submitClicked(bool)), this, SLOT(on_hintMsgPage(bool)));

    //导航栏
    QString strTitle = tr("选择管理员");
    QString strRightBtn = tr("关闭");
    navigationBar = new NavigationBar(this);
    navigationBar->setTitleText(strTitle);
    navigationBar->setRightText(strRightBtn);
    connect(navigationBar, SIGNAL(rightClicked()), this, SLOT(on_btnClosed_clicked()));

    QHBoxLayout* pHLTop = new QHBoxLayout();
    pHLTop->addWidget(navigationBar);
    pHLTop->setSpacing(0);
    pHLTop->setMargin(0);
    this->setTopbarLayout(pHLTop);

    //选择管理员列表
    selectManagerLayout = new QVBoxLayout;
    selectManagerLayout->setMargin(0);
    selectManagerLayout->setSpacing(0);

    this->setBodyPartLayout(selectManagerLayout);
    this->installScrollViewportArea();
    timerID = this->startTimer(ConstNum::MAX_TIME_INTERVAL);
}

void DClusterAddManagerPage::fetchData(InputPacket &inpack)
{
    quint16 cmdCode = inpack.getCmdCode();
    switch(cmdCode){
        case CommandCode::DCLUSTER_SETTING_USER_AUTHORIZE:
            handleDevClusterSettingManager(inpack);
            break;
        case CommandCode::FETCH_DCLUSTER_USER_LIST:
            handleDevClusterUserList(inpack);
            break;
    }
}

void DClusterAddManagerPage::timerEvent(QTimerEvent*)
{
    AssemblyData assemblyData;
    assemblyData.append(curClusterID);
    char* data = assemblyData.getAssemblyData();
    qint16 dataLen = assemblyData.getAssemblyDataLen();
    emit readyRead(CommandCode::FETCH_DCLUSTER_USER_LIST, data, dataLen);
    this->killTimer(timerID);
}

void DClusterAddManagerPage::handleNetError()
{
    if(loadingPage == NULL) return;

    curManagerID = 0;
    QString lblText = tr("网络错误,请重试");
    this->handleHintInfo(lblText);
}

void DClusterAddManagerPage::handleHintInfo(QString& text)
{
    hintMsgPage->setText(text);
    hintMsgPage->show();
    SAFE_DELETE(loadingPage);
}

void DClusterAddManagerPage::handleDevClusterUserList(InputPacket &inpack)
{
    char* data = inpack.getPackData();
    quint16 dataLen = inpack.getPackDataLen();
    userInfoList.appendList(data, dataLen);

    QHash<quint64, DClusterUserInfo*> usrList = userInfoList.fetchList();
    QHashIterator<quint64, DClusterUserInfo*> iter(usrList);
    while(iter.hasNext()){
        iter.next();
        DClusterUserInfo* info = iter.value();

        if(info->getUserRole() == Mi::Operator || btnUserList.contains(info->getUserID())){
            QPixmap pixmap(ImagePath::USER_PORTRAIT);
            pixmap.setDevicePixelRatio(2.0);
            GroupButton* btn = new GroupButton(this);
            btn->setIndexOne(info->getUserID());
            btn->setPixMap(pixmap);
            btn->setText(info->getUserName());
            btn->setFixedHeight(this->screenHeight()*0.12);
            btn->setFixedWidth(this->screenWidth());
            btn->setAttribute(Qt::WA_LayoutUsesWidgetRect);
            btn->setStyleSheet(SheetStyle::GROUPBUTTON_BOTTOMBORDER);
            btnUserList.insert(info->getUserID(), btn);
            connect(btn, SIGNAL(clicked()), this, SLOT(on_btnSelectManager_clicked()));

            if(selectManagerLayout->count() == 0){
                selectManagerLayout->addWidget(btn);
                selectManagerLayout->addStretch(0);
            }
            else
                selectManagerLayout->insertWidget(selectManagerLayout->count() - 1, btn);
        }
    }
}

void DClusterAddManagerPage::handleDevClusterSettingManager(InputPacket &inpack)
{
    QString strText;
    quint8 ansCode = inpack.getAnswerCode();
    if(ansCode == Mi::Success){
        strText = tr("设置管理员成功");
        hintMsgPage->setStatus(true);
    }
    else if(ansCode == Mi::Failure){
        strText = tr("设置管理员失败, 请重试");
        curManagerID = 0;
    }
    this->handleHintInfo(strText);
}

void DClusterAddManagerPage::on_btnSelectManager_clicked()
{
    GroupButton* btn = dynamic_cast<GroupButton*>(sender());
    quint8 role = Mi::SeniorManager;
    curManagerID = btn->getIndexOne();

    AssemblyData assemblyData;
    assemblyData.append(curClusterID);
    assemblyData.append(role);
    assemblyData.append(curManagerID);
    char* data = assemblyData.getAssemblyData();
    quint16 dataLen = assemblyData.getAssemblyDataLen();
    emit readyRead(CommandCode::DCLUSTER_SETTING_USER_AUTHORIZE, data, dataLen);

    //页面加载窗口
    if(loadingPage == NULL){
        loadingPage = new LoadingPage(this);
        loadingPage->show();
    }
}

void DClusterAddManagerPage::on_hintMsgPage(bool isJump)
{
    hintMsgPage->hide();
    if(!isJump) return;

    this->hide();
    this->deleteLater();
}

void DClusterAddManagerPage::on_btnClosed_clicked()
{
    this->hide();
    this->deleteLater();
}

DClusterAddManagerPage::~DClusterAddManagerPage()
{

}
