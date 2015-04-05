#include <QVBoxLayout>
#include <QTextEdit>

#include "AssemblyData.h"
#include "InputPacket.h"
#include "LoadingPage.h"
#include "HintMsgPage.h"
#include "GlobalDefine.hpp"
#include "UserProtocol.hpp"
#include "DClusterDevInfo.h"
#include "ImageConfig.hpp"
#include "SheetStyleConfig.hpp"
#include "NavigationBar.h"
#include "DClusterDevAlterDescribePage.h"

DClusterDevAlterDescribePage::DClusterDevAlterDescribePage(QWidget* parent, quint64 clusID, quint64 devID)
    :BasePage(parent)
    , curClusterID(clusID)
    , curDeviceID(devID)
    , loadingPage(NULL)
{
    this->initializeWidget();
}

void DClusterDevAlterDescribePage::initializeWidget()
{
    //提示信息页面操作
    hintMsgPage = new HintMsgPage(this);
    hintMsgPage->hide();
    connect(hintMsgPage, SIGNAL(submitClicked(bool)), this, SLOT(on_hintMsgPage(bool)));

    //导航栏
    QString strTitle = tr("设备描述");
    QString strLeftBtn = tr("群设备");
    QString strRightBtn = tr("提交");
    QPixmap leftArrowsPixmap(ImagePath::NAVIGATION_LEFT_ARROWS);
    navigationBar = new NavigationBar(this);
    navigationBar->setTitleText(strTitle);
    navigationBar->setLeftText(strLeftBtn);
    navigationBar->setRightText(strRightBtn);
    navigationBar->setLeftPixMap(leftArrowsPixmap);
    connect(navigationBar, SIGNAL(leftClicked()), this, SLOT(on_btnPrePage_clicked()));
    connect(navigationBar, SIGNAL(rightClicked()), this, SLOT(on_btnSubmitPage_clicked()));

    QHBoxLayout* pHLTop = new QHBoxLayout();
    pHLTop->addWidget(navigationBar);
    pHLTop->setSpacing(0);
    pHLTop->setMargin(0);
    this->setTopbarLayout(pHLTop);

    //介绍
    editDescribe = new QTextEdit(this);
    editDescribe->setFixedHeight(this->screenHeight()*0.30);
    editDescribe->setContentsMargins(this->screenWidth()*0.02, this->screenWidth()*0.02, this->screenWidth()*0.02, 0);
    editDescribe->setStyleSheet("border-style:solid; border-radius:0px; border-width: 0px; \
                                border-color:rgb(208, 208, 208); background-color:white; font:15px;");

    QVBoxLayout* vblTotalLayout = new QVBoxLayout;
    vblTotalLayout->addSpacing(this->screenHeight()*0.026);
    vblTotalLayout->addWidget(editDescribe);
    vblTotalLayout->setMargin(0);
    vblTotalLayout->setAlignment(Qt::AlignTop);
    vblTotalLayout->setSpacing(0);
    vblTotalLayout->addStretch(0);
    this->setBodyPartLayout(vblTotalLayout);

    //屏幕触摸滚动设置
    this->installScrollViewportArea();
    this->loadLocalData();
}

void DClusterDevAlterDescribePage::fetchData(InputPacket &inpack)
{
    quint16 cmdCode = inpack.getCmdCode();
    switch(cmdCode)
    {
        case CommandCode::DCLUSTER_DEVICE_ALTER_DESCRIBE:
            this->handleDescribeInfo(inpack);
            break;
        case CommandCode::NET_ERROR:
            this->handleNetError();
            break;
    }
}

void DClusterDevAlterDescribePage::loadLocalData()
{
    curDeviceInfo.loadDataSource(curClusterID, curDeviceID);
    editDescribe->setText(curDeviceInfo.getDescribe());
}

void DClusterDevAlterDescribePage::handleDescribeInfo(InputPacket& inpack)
{
    quint16 ansCode = inpack.getAnswerCode();
    if(ansCode == Mi::Success){
        QString describe = editDescribe->toPlainText().trimmed();
        curDeviceInfo.updateDescribe(describe);

        QString strText = tr("修改成功");
        hintMsgPage->setStatus(true);
        this->handleHintInfo(strText);
    }else if(ansCode == Mi::Failure){
        QString strText = tr("修改失败,请重试");
        this->handleHintInfo(strText);
    }else
        SAFE_DELETE(loadingPage)
}

void DClusterDevAlterDescribePage::handleHintInfo(QString &strText)
{
    hintMsgPage->setText(strText);
    hintMsgPage->show();
    SAFE_DELETE(loadingPage);
}

void DClusterDevAlterDescribePage::handleNetError()
{
    if(loadingPage == NULL) return;
    QString strText = tr("网络错误,请重试");
    this->handleHintInfo(strText);
}

void DClusterDevAlterDescribePage::on_btnSubmitPage_clicked()
{
    QString strClusteDetail = editDescribe->toPlainText().trimmed();
    AssemblyData assemblyData;
    assemblyData.append(curClusterID);
    assemblyData.append(curDeviceID);
    assemblyData.append(strClusteDetail);
    char* data = assemblyData.getAssemblyData();
    quint16 dataLen = assemblyData.getAssemblyDataLen();
    emit readyRead(CommandCode::DCLUSTER_DEVICE_ALTER_DESCRIBE, data, dataLen);

    if(loadingPage == NULL){
        loadingPage = new LoadingPage(this);
        loadingPage->show();
    }
}

void DClusterDevAlterDescribePage::on_hintMsgPage(bool isJump)
{
    hintMsgPage->hide();
    if(!isJump) return;
    this->hide();
    this->deleteLater();
}

void DClusterDevAlterDescribePage::on_btnPrePage_clicked()
{
    this->hide();
    this->deleteLater();
}

DClusterDevAlterDescribePage::~DClusterDevAlterDescribePage()
{

}


