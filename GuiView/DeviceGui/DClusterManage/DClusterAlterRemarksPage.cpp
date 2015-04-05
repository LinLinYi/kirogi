#include <QVBoxLayout>
#include <QLineEdit>

#include "AssemblyData.h"
#include "ImageConfig.hpp"
#include "DClusterInfo.h"
#include "UserProtocol.hpp"
#include "NavigationBar.h"
#include "HintMsgPage.h"
#include "InputPacket.h"
#include "LoadingPage.h"
#include "SheetStyleConfig.hpp"
#include "DClusterAlterRemarksPage.h"

DClusterAlterRemarksPage::DClusterAlterRemarksPage(QWidget* parent, quint64 clusID)
    :BasePage(parent)
    , curClusterID(clusID)
    , loadingPage(NULL)
{
    this->initializeWidget();
}

void DClusterAlterRemarksPage::initializeWidget()
{
    //提示信息页面操作
    hintMsgPage = new HintMsgPage(this);
    hintMsgPage->hide();
    connect(hintMsgPage, SIGNAL(submitClicked(bool)), this, SLOT(on_hintMsgPage(bool)));

    //导航栏
    QString strTitle = tr("群备注");
    QString strLeftBtn = tr("返回");
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

void DClusterAlterRemarksPage::fetchData(InputPacket &inpack)
{
    quint16 cmdCode = inpack.getCmdCode();
    switch(cmdCode)
    {
        case CommandCode::DCLUSTER_ALTER_REMARKS:
            this->handleRemarksInfo(inpack);
            break;
        case CommandCode::NET_ERROR:
            this->handleNetError();
            break;
    }
}

void DClusterAlterRemarksPage::loadLocalData()
{
    curClusterInfo.loadDataSource(curClusterID);
    editRemarks->setText(curClusterInfo.getRemarks());
}

void DClusterAlterRemarksPage::handleRemarksInfo(InputPacket& inpack)
{
    quint16 ansCode = inpack.getAnswerCode();
    if(ansCode == Mi::Success){
        QString strRemarks = editRemarks->text().trimmed();
        curClusterInfo.updateRemarks(strRemarks);

        QString strText = tr("修改成功");
        hintMsgPage->setStatus(true);
        this->handleHintInfo(strText);
    }else if(ansCode == Mi::Failure){
        QString strText = tr("修改失败,请重试");
        this->handleHintInfo(strText);
    }else
        SAFE_DELETE(loadingPage)
}

void DClusterAlterRemarksPage::handleHintInfo(QString &strText)
{
    hintMsgPage->setText(strText);
    hintMsgPage->show();
    SAFE_DELETE(loadingPage);
}

void DClusterAlterRemarksPage::handleNetError()
{
    if(loadingPage == NULL) return;
    QString strText = tr("网络错误,请重试");
    this->handleHintInfo(strText);
}

void DClusterAlterRemarksPage::on_btnSubmitPage_clicked()
{
    QString strClusteDetail = editRemarks->text().trimmed();
    AssemblyData assemblyData;
    assemblyData.append(curClusterID);
    assemblyData.append(strClusteDetail);
    char* data = assemblyData.getAssemblyData();
    quint16 dataLen = assemblyData.getAssemblyDataLen();
    emit readyRead(CommandCode::DCLUSTER_ALTER_REMARKS, data, dataLen);

    if(loadingPage == NULL){
        loadingPage = new LoadingPage(this);
        loadingPage->show();
    }
}

void DClusterAlterRemarksPage::on_hintMsgPage(bool isJump)
{
    hintMsgPage->hide();
    if(!isJump) return;

    this->hide();
    this->deleteLater();
}

void DClusterAlterRemarksPage::on_btnPrePage_clicked()
{
    this->hide();
    this->deleteLater();
}

DClusterAlterRemarksPage::~DClusterAlterRemarksPage()
{

}
