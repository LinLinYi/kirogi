#include <QHBoxLayout>
#include <QVBoxLayout>

#include "UserProtocol.hpp"
#include "HintMsgPage.h"
#include "LoadingPage.h"
#include "InputPacket.h"
#include "ListWidget.h"
#include "AssemblyData.h"
#include "NavigationBar.h"
#include "GroupButton.h"
#include "ImageConfig.hpp"
#include "SheetStyleConfig.hpp"
#include "DClusterUserInfoList.h"
#include "DClusterDelUserPage.h"

DClusterDelUserPage::DClusterDelUserPage(QWidget* parent, quint64 clusterID)
    : BasePage(parent)
    , curClusterID(clusterID)
    , curFixedContentHeight(0)
    , selectDelUserCount(0)
    , loadingPage(NULL)
{
    this->initialWidget();
}

void DClusterDelUserPage::loadLocalData()
{
    userInfoList = new DClusterUserInfoList(curClusterID);
    userInfoList->loadDataSource();
}

void DClusterDelUserPage::initialWidget()
{
    //提示信息页面操作
    hintMsgPage = new HintMsgPage(this);
    hintMsgPage->hide();
    connect(hintMsgPage, SIGNAL(submitClicked(bool)), this, SLOT(on_hintMsgPage(bool)));

    //导航栏
    QString strTitle = tr("删除成员");
    QString strLeftBtn = tr("取消");
    QString strRightBtn = tr("删除");
    QPixmap leftArrowsPixmap(ImagePath::NAVIGATION_LEFT_ARROWS);
    navigationBar = new NavigationBar(this);
    navigationBar->setTitleText(strTitle);
    navigationBar->setLeftText(strLeftBtn);
    navigationBar->setLeftPixMap(leftArrowsPixmap);
    navigationBar->setRightText(strRightBtn);
    connect(navigationBar, SIGNAL(leftClicked()), this, SLOT(on_btnPrePage_clicked()));
    connect(navigationBar, SIGNAL(rightClicked()), this, SLOT(on_btnDeleteSubmit_clicked()));

    QHBoxLayout* pHLTop = new QHBoxLayout();
    pHLTop->addWidget(navigationBar);
    pHLTop->setSpacing(0);
    pHLTop->setMargin(0);
    this->setTopbarLayout(pHLTop);

    //群成员布局
    userListWidget = new ListWidget(this);
    userListWidget->setStyleSheet(SheetStyle::LISTWIDGET);
    userListWidget->setContentsMargins(this->screenWidth()*0.03,0,0,0);
    curFixedContentHeight += userListWidget->height();

    QVBoxLayout* vblTotalLayout = new QVBoxLayout;
    vblTotalLayout->addWidget(userListWidget);
    vblTotalLayout->addStretch(0);
    vblTotalLayout->setMargin(0);
    vblTotalLayout->setSpacing(0);
    vblTotalLayout->setAlignment(Qt::AlignTop);
    this->setBodyPartLayout(vblTotalLayout);

    //屏幕触摸滚动设置
    this->installScrollViewportArea();
    this->setBodyRealHeight(curFixedContentHeight);

    this->loadLocalData();
    this->deployUserList(userInfoList->fetchList());
}

void DClusterDelUserPage::fetchData(InputPacket &inpack)
{
    quint16 cmdCode = inpack.getCmdCode();
    switch(cmdCode)
    {
        case CommandCode::NET_ERROR:
            this->handleNetError();
            break;
        case CommandCode::DCLUSTER_DELETE_USER:
            this->handleDelUserReply(inpack);
            break;
    }
}

void DClusterDelUserPage::handleNetError()
{
    if(loadingPage == NULL) return;
    QString lblText = tr("网络错误,请重试");
    this->handleHintInfo(lblText);
}

void DClusterDelUserPage::handleDelUserReply(InputPacket &inpack)
{
    quint8 ansCode = inpack.getAnswerCode();
    if(ansCode == Mi::Success){
        QString strText = tr("删除成功");
        hintMsgPage->setStatus(true);
        this->handleHintInfo(strText);
    }else if(ansCode == Mi::Failure){
        QString strText = tr("删除失败，请重试");
        this->handleHintInfo(strText);
    }
}

void DClusterDelUserPage::handleHintInfo(QString& text)
{
    hintMsgPage->setText(text);
    hintMsgPage->show();
    SAFE_DELETE(loadingPage);
}

void DClusterDelUserPage::deployUserList(QHash<quint64, DClusterUserInfo *> list)
{
    QHashIterator<quint64, DClusterUserInfo*> iter(list);
    while(iter.hasNext()){
        iter.next();
        DClusterUserInfo* info = dynamic_cast<DClusterUserInfo*>(iter.value());
        GroupButton* btn = new GroupButton(this);
        this->setUserButton(btn, info);
    }

    if(curFixedContentHeight <= this->bodyPartHeight())
        this->setBodyRealHeight(this->bodyPartHeight());
    else
        this->setBodyRealHeight(curFixedContentHeight);
}

void DClusterDelUserPage::setUserButton(GroupButton* btn, DClusterUserInfo* info)
{
    if(btn == NULL || info == NULL) return;

    //设备按钮设置
    QPixmap pixmap(ImagePath::USER_PORTRAIT);
    QPixmap pixmapReserve(ImagePath::SELECT_CANCEL);
    pixmap.setDevicePixelRatio(2.0);
    pixmapReserve.setDevicePixelRatio(2.0);

    btn->setIndexOne(info->getUserID());
    btn->setText(info->getUserName());
    btn->setPixMap(pixmap);
    btn->setFixedHeight(this->screenHeight()*0.1);
    btn->setContentsMargins(0, 0, this->screenWidth()*0.03, 0);
    btn->setReserve(pixmapReserve);
    btn->setAttribute(Qt::WA_LayoutUsesWidgetRect);
    btn->setStyleSheet(SheetStyle::GROUPBUTTON_NOBORDER);
    userListWidget->addWidget(btn);
    connect(btn, SIGNAL(clicked()), this, SLOT(on_selectDelUser_clicked()));

    int index = userListWidget->indexof(btn);
    if(index > 0){//始终保持底边无边线
        QWidget* widget = userListWidget->itemAt(index - 1);
        GroupButton* temp = dynamic_cast<GroupButton*>(widget);
        temp->setStyleSheet(SheetStyle::GROUPBUTTON_BOTTOMBORDER);
        btn->setStyleSheet(SheetStyle::GROUPBUTTON_NOBORDER);
    }
}

void DClusterDelUserPage::on_btnPrePage_clicked()
{
    this->hide();
    this->deleteLater();
}

void DClusterDelUserPage::on_hintMsgPage(bool isJump)
{
    hintMsgPage->hide();
    if(!isJump) return;
    this->hide();
    this->deleteLater();
}

void DClusterDelUserPage::on_btnDeleteSubmit_clicked()
{
    if(selectDelUserCount == 0) {
        QString strText = tr("请选择删除的成员");
        this->handleHintInfo(strText);
        return;
    }

    AssemblyData assemblyData;
    assemblyData.append(curClusterID);
    assemblyData.append(selectDelUserCount);

    for(int i = 0; i < userListWidget->count(); i++){
        QWidget* widget = userListWidget->itemAt(i);
        GroupButton* btn = dynamic_cast<GroupButton*>(widget);
        if(btn->getSelectFlag()){
            quint64 userID = btn->getIndexOne();
            assemblyData.append(userID);
            delUserIdList.append(userID);
        }
    }

    char* data = assemblyData.getAssemblyData();
    quint16 dataLen = assemblyData.getAssemblyDataLen();
    emit readyRead(CommandCode::DCLUSTER_DELETE_USER, data, dataLen);

    //加载等待页面
    if(loadingPage == NULL){
        loadingPage = new LoadingPage(this);
        loadingPage->show();
    }
}

void DClusterDelUserPage::on_selectDelUser_clicked()
{
        GroupButton* groupButton = dynamic_cast<GroupButton*>(sender());
        QPixmap pixmapBtnDelete(ImagePath::SELECT_DELETE);
        pixmapBtnDelete.setDevicePixelRatio(2.0);

        QPixmap pixmapBtnCancel(ImagePath::SELECT_CANCEL);
        pixmapBtnCancel.setDevicePixelRatio(2.0);

        if(groupButton->getSelectFlag() == false){
            groupButton->setReserve(pixmapBtnDelete);
            groupButton->setSelectFlag(true);
            selectDelUserCount++;
        }
        else{
            groupButton->setReserve(pixmapBtnCancel);
            groupButton->setSelectFlag(false);
            selectDelUserCount--;
        }
}

QList<quint64> DClusterDelUserPage::getDelUserList()
{
    return delUserIdList;
}

DClusterDelUserPage::~DClusterDelUserPage()
{

}
