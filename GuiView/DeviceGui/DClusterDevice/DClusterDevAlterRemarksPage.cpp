#include <QVBoxLayout>
#include <QLineEdit>

#include "AssemblyData.h"
#include "InputPacket.h"
#include "GlobalDefine.hpp"
#include "DClusterDevInfo.h"
#include "UserProtocol.hpp"
#include "HintMsgPage.h"
#include "LoadingPage.h"
#include "ImageConfig.hpp"
#include "SheetStyleConfig.hpp"
#include "NavigationBar.h"
#include "DClusterDevAlterRemarksPage.h"

DClusterDevAlterRemarksPage::DClusterDevAlterRemarksPage(QWidget* parent, quint64 clusID, quint64 devID)
    :BasePage(parent)
    , curClusterID(clusID)
    , curDeviceID(devID)
    , loadingPage(NULL)
{
    this->initializeWidget();
}

void DClusterDevAlterRemarksPage::initializeWidget()
{
    //提示信息页面操作
    hintMsgPage = new HintMsgPage(this);
    hintMsgPage->hide();
    connect(hintMsgPage, SIGNAL(submitClicked(bool)), this, SLOT(on_hintMsgPage(bool)));

    //导航栏
    QString strTitle = tr("设备备注");
    QString strLeftBtn = tr("群管理");
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

    //备注
    editRemarks = new QLineEdit(this);
    editRemarks->setTextMargins(this->screenWidth()*0.02, 0, 0, 0);
    editRemarks->setFixedHeight(this->screenHeight()*0.07);
    editRemarks->setStyleSheet("border-style:solid; border-radius:0px; border-width: 0px;\
                               border-color:rgb(208, 208, 208); background-color:white; font:15px;");

    QVBoxLayout* vblTotalLayout = new QVBoxLayout;
    vblTotalLayout->addSpacing(this->screenHeight()*0.026);
    vblTotalLayout->addWidget(editRemarks);
    vblTotalLayout->setMargin(0);
    vblTotalLayout->setAlignment(Qt::AlignTop);
    vblTotalLayout->setSpacing(0);
    vblTotalLayout->addStretch(0);
    this->setBodyPartLayout(vblTotalLayout);

    //屏幕触摸滚动设置
    this->installScrollViewportArea();
    this->loadLocalData();
}

void DClusterDevAlterRemarksPage::fetchData(InputPacket &inpack)
{
    quint16 cmdCode = inpack.getCmdCode();
    switch(cmdCode)
    {
        case CommandCode::DCLUSTER_DEVICE_ALTER_REMARKS:
            this->handleRemarksInfo(inpack);
            break;
        case CommandCode::NET_ERROR:
            this->handleNetError();
            break;
    }
}

void DClusterDevAlterRemarksPage::loadLocalData()
{
    curDeviceInfo.loadDataSource(curClusterID, curDeviceID);
    editRemarks->setText(curDeviceInfo.getRemarks());
}

void DClusterDevAlterRemarksPage::handleRemarksInfo(InputPacket& inpack)
{
    quint16 ansCode = inpack.getAnswerCode();
    if(ansCode == Mi::Success){
        QString strRemarks = editRemarks->text().trimmed();
        curDeviceInfo.updateRemarks(strRemarks);

        QString strText = tr("修改成功");
        hintMsgPage->setStatus(true);
        this->handleHintInfo(strText);
    }else if(ansCode == Mi::Failure){
        QString strText = tr("修改失败,请重试");
        this->handleHintInfo(strText);
    }else
        SAFE_DELETE(loadingPage)
}

void DClusterDevAlterRemarksPage::handleHintInfo(QString &strText)
{
    hintMsgPage->setText(strText);
    hintMsgPage->show();
    SAFE_DELETE(loadingPage);
}

void DClusterDevAlterRemarksPage::handleNetError()
{
    if(loadingPage == NULL) return;
    QString strText = tr("网络错误,请重试");
    this->handleHintInfo(strText);
}

void DClusterDevAlterRemarksPage::on_btnSubmitPage_clicked()
{
    QString strClusteDetail = editRemarks->text().trimmed();
    AssemblyData assemblyData;
    assemblyData.append(curClusterID);
    assemblyData.append(curDeviceID);
    assemblyData.append(strClusteDetail);
    char* data = assemblyData.getAssemblyData();
    quint16 dataLen = assemblyData.getAssemblyDataLen();
    emit readyRead(CommandCode::DCLUSTER_DEVICE_ALTER_REMARKS, data, dataLen);

    if(loadingPage == NULL){
        loadingPage = new LoadingPage(this);
        loadingPage->show();
    }
}

void DClusterDevAlterRemarksPage::on_hintMsgPage(bool isJump)
{
    hintMsgPage->hide();
    if(!isJump) return;
    this->hide();
}

void DClusterDevAlterRemarksPage::on_btnPrePage_clicked()
{
    this->hide();
    this->deleteLater();
}

DClusterDevAlterRemarksPage::~DClusterDevAlterRemarksPage()
{

}


