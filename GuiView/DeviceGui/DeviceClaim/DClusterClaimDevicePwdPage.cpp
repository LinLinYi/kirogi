#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>

#include "UserProtocol.hpp"
#include "HintMsgPage.h"
#include "NavigationBar.h"
#include "InputPacket.h"
#include "GlobalDefine.hpp"
#include "LoadingPage.h"
#include "AssemblyData.h"
#include "SheetStyleConfig.hpp"
#include "DClusterClaimDevicePwdPage.h"

DClusterClaimDevicePwdPage::DClusterClaimDevicePwdPage(QWidget* parent, quint64 clusID, quint64 devID)
    : BasePage(parent)
    , curClusterID(clusID)
    , curDeviceID(devID)
    , loadingPage(NULL)
{
    this->initializeWidget();
}

void DClusterClaimDevicePwdPage::initializeWidget()
{
    //提示信息页面操作
    hintMsgPage = new HintMsgPage(this);
    hintMsgPage->hide();
    connect(hintMsgPage, SIGNAL(submitClicked(bool)), this, SLOT(on_hintMsgPage(bool)));

    //导航栏
    QString strTitle = tr("设备认领");
    QString strLeftBtn = tr("取消");
    navigationBar = new NavigationBar(this);
    navigationBar->setTitleText(strTitle);
    navigationBar->setLeftText(strLeftBtn);
    connect(navigationBar, SIGNAL(leftClicked()), this, SLOT(on_btnPrePage_clicked()));

    QHBoxLayout* pHLTop = new QHBoxLayout();
    pHLTop->addWidget(navigationBar);
    pHLTop->setSpacing(0);
    pHLTop->setMargin(0);
    this->setTopbarLayout(pHLTop);

    //主体部分
    editClaimPwd = new QLineEdit(this);
    editClaimPwd->setFixedHeight(this->screenHeight()*0.077);
    editClaimPwd->setPlaceholderText(tr("输入设备认领密码"));
    editClaimPwd->setStyleSheet(SheetStyle::QLINEEDIT);
    editClaimPwd->setEchoMode(QLineEdit::Password);
    editClaimPwd->setTextMargins(this->screenWidth()*0.02, 0, this->screenWidth()*0.02, 0);

    btnSubmit = new QPushButton(tr("提 交"), this);
    btnSubmit->setFixedHeight(this->screenHeight()*0.074);
    btnSubmit->setStyleSheet(SheetStyle::QPUSHBUTTON);
    connect(btnSubmit, SIGNAL(clicked()), this, SLOT(on_btnSubmit_clicked()));

    QVBoxLayout* vbLayout = new QVBoxLayout;
    vbLayout->addWidget(editClaimPwd);
    vbLayout->addSpacing(this->screenHeight()*0.03);
    vbLayout->addWidget(btnSubmit);
    vbLayout->setAlignment(Qt::AlignTop);
    vbLayout->addStretch(0);
    vbLayout->setSpacing(0);
    vbLayout->setContentsMargins(this->screenWidth()*0.054,this->screenHeight()*0.026,this->screenWidth()*0.054,0);
    this->setBodyPartLayout(vbLayout);
}

void DClusterClaimDevicePwdPage::fetchData(InputPacket &inpack)
{
    quint16 cmdCode = inpack.getCmdCode();
    switch(cmdCode)
    {
        case CommandCode::DCLUSTER_CLAIM_DEVICE:
            this->handleClaimDevicePwd(inpack);
            break;
        case CommandCode::NET_ERROR:
            this->handleNetError();
            break;
    }
}

void DClusterClaimDevicePwdPage::handleHintInfo(QString &strText)
{
    hintMsgPage->setText(strText);
    hintMsgPage->show();
    SAFE_DELETE(loadingPage);
}

void DClusterClaimDevicePwdPage::handleNetError()
{
    if(loadingPage == NULL) return;
    QString lblText = tr("网络连接错误,请重试");
    this->handleHintInfo(lblText);
}

void DClusterClaimDevicePwdPage::handleClaimDevicePwd(InputPacket &inpack)
{
    quint8 answerCode = inpack.getAnswerCode();
    if(answerCode == Mi::Success){
        QString strText = tr("认领请求已发送");
        hintMsgPage->setStatus(true);
        handleHintInfo(strText);
    }
    else if(answerCode == Mi::Failure){
        QString strText = tr("设备认领失败,请重试");
        handleHintInfo(strText);
    }
}

void DClusterClaimDevicePwdPage::on_btnSubmit_clicked()
{
    if(editClaimPwd->text().trimmed().length() == 0){
        QString str = tr("请输入设备认领密码");
        hintMsgPage->setText(str);
        hintMsgPage->show();
        return;
    }

    /* client--send--server: clusterID + deviceID + device password */
    QString strClaimPwd = editClaimPwd->text().trimmed();
    AssemblyData assemblyData;
    assemblyData.append(curClusterID);
    assemblyData.append(curDeviceID);
    assemblyData.append(strClaimPwd);
    char* data = assemblyData.getAssemblyData();
    quint16 dataLen = assemblyData.getAssemblyDataLen();
    emit readyRead(CommandCode::DCLUSTER_CLAIM_DEVICE, data, dataLen);

    if(loadingPage == NULL){
        loadingPage = new LoadingPage(this);
        loadingPage->show();
    }
}

void DClusterClaimDevicePwdPage::on_hintMsgPage(bool isJump)
{
    hintMsgPage->hide();
    if(!isJump) return;
    this->hide();
    this->deleteLater();
}

void DClusterClaimDevicePwdPage::on_btnPrePage_clicked()
{
    this->hide();
    this->deleteLater();
}

DClusterClaimDevicePwdPage::~DClusterClaimDevicePwdPage()
{

}
