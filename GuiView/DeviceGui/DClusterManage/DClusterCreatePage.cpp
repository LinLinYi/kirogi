#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>

#include "GlobalDefine.hpp"
#include "InputPacket.h"
#include "HintMsgPage.h"
#include "LoadingPage.h"
#include "NavigationBar.h"
#include "UserProtocol.hpp"
#include "DClusterCreatePage.h"
#include "ImageConfig.hpp"
#include "SheetStyleConfig.hpp"
#include "AssemblyData.h"
#include "DispersalData.h"
#include "DClusterCreateSuccessedPage.h"

DClusterCreatePage::DClusterCreatePage(QWidget* parent)
    : BasePage(parent)
    , loadingPage(NULL)
    , clusterCreateSuccessedPage(NULL)
{
    this->initializeWidget();
}

void DClusterCreatePage::initializeWidget()
{
    //提示信息页面操作
    hintMsgPage = new HintMsgPage(this);
    hintMsgPage->hide();
    connect(hintMsgPage, SIGNAL(submitClicked(bool)), this, SLOT(on_hintMsgPage(bool)));

    //导航栏
    QString strTitle = tr("创建设备群");
    QString strLeftBtn = tr("返回");
    QString strRightBtn = tr("提交");
    navigationBar = new NavigationBar(this);
    navigationBar->setTitleText(strTitle);
    navigationBar->setLeftText(strLeftBtn);
    navigationBar->setRightText(strRightBtn);

    QPixmap leftArrowsPixmap(ImagePath::NAVIGATION_LEFT_ARROWS);
    leftArrowsPixmap.setDevicePixelRatio(2);
    navigationBar->setLeftPixMap(leftArrowsPixmap);
    connect(navigationBar, SIGNAL(leftClicked()), this, SLOT(on_btnPrePage_clicked()));
    connect(navigationBar, SIGNAL(rightClicked()), this, SLOT(on_btnSubmitPage_clicked()));

    QHBoxLayout* pHLTop = new QHBoxLayout();
    pHLTop->addWidget(navigationBar);
    pHLTop->setSpacing(0);
    pHLTop->setMargin(0);
    this->setTopbarLayout(pHLTop);

    editClusterName = new QLineEdit(this);
    editClusterName->setPlaceholderText(tr("群名称(2~10字符)"));
    editClusterName->setFixedWidth(this->screenWidth());
    editClusterName->setFixedHeight(this->screenHeight()* 0.077);
    editClusterName->setTextMargins(this->screenWidth()*0.047, 0, 0, 0);
    editClusterName->setStyleSheet(SheetStyle::CREATECLUSTER_QLINEEDIT);

    editDescribe = new QTextEdit(this);
    editDescribe->setPlaceholderText(tr("群描述(5~150字符)"));
    editDescribe->setFixedHeight(this->screenHeight()*0.30);
    editDescribe->setFixedWidth(this->screenWidth());
    editDescribe->setContentsMargins(this->screenWidth()*0.047, this->screenWidth()*0.03, 0, this->screenWidth()*0.047);
    editDescribe->setStyleSheet(SheetStyle::CREATECLUSTER_QTEXTEDIT);

    QVBoxLayout* vbLayout = new QVBoxLayout();
    vbLayout->addWidget(editClusterName);
    vbLayout->addWidget(editDescribe);
    vbLayout->setAlignment(Qt::AlignTop);
    vbLayout->setSpacing(0);
    vbLayout->setMargin(0);
    vbLayout->addStretch(0);
    this->setBodyPartLayout(vbLayout);

    //屏幕触摸滚动设置
    this->installScrollViewportArea();
}

void DClusterCreatePage::fetchData(InputPacket &inpack)
{
    quint16 cmdCode = inpack.getCmdCode();
    DPRINT("CreateDevClusterPage receive data, the cmdCode is:0x%x", cmdCode);

    switch(cmdCode)
    {
        case CommandCode::DCLUSTER_CREATE:
            this->handleCreateDevCluster(inpack);
            break;
        case CommandCode::NET_ERROR:
            this->handleNetError();
            break;
    }
    this->transferDataToAnotherPage(inpack, clusterCreateSuccessedPage);
}

void DClusterCreatePage::handleNetError()
{
    if(loadingPage == NULL) return;
    QString lblText = tr("网络错误,请重试");
    this->handleHintInfo(lblText);
}

void DClusterCreatePage::handleHintInfo(QString &strText)
{
    hintMsgPage->setText(strText);
    hintMsgPage->show();
    SAFE_DELETE(loadingPage);
}

void DClusterCreatePage::handleCreateDevCluster(InputPacket &inpack)
{
    quint8 ansCode = inpack.getAnswerCode();
    if(ansCode == Mi::Success){
        DispersalData dispersalData;
        dispersalData.outlet(curClusterID, inpack.getPackData());

        SAFE_DELETE(loadingPage);
        clusterCreateSuccessedPage = new DClusterCreateSuccessedPage(this, curClusterID);
        this->displayConcretePage(clusterCreateSuccessedPage);
    }
    else{
        QString strText = tr("该群账号已注册");
        this->handleHintInfo(strText);
    }
}

void DClusterCreatePage::displayConcretePage(BasePage *page)
{
    if(page == NULL) return;
    page->show();
    connect(page, SIGNAL(destroyed()), this, SLOT(on_deleteSubPages()));
    connect(page, SIGNAL(readyRead(quint16,char*,quint16)), this, SLOT(on_recvDataFromOtherPage(quint16,char*,quint16)));
}

void DClusterCreatePage::transferDataToAnotherPage(InputPacket &inpack, BasePage *basePage)
{
    if(basePage == NULL) return;
    basePage->fetchData(inpack);
}

void DClusterCreatePage::on_btnSubmitPage_clicked()
{
    QString strClustername = editClusterName->text().trimmed();
    if(strClustername.length() < 2 || strClustername.length() >10 ){
        QString str = tr("群名称须为2~10个字");
        handleHintInfo(str);
        return;
    }

    QString strClusteDetail = editDescribe->toPlainText().trimmed();
    if(strClusteDetail.length() < 5 || strClusteDetail.length() > 150){
        QString str = tr("群描述须为5~150个字");
        handleHintInfo(str);
        return;
    }

    AssemblyData assemblyData;
    assemblyData.append(strClustername);
    assemblyData.append(strClusteDetail);
    char* data = assemblyData.getAssemblyData();
    quint16 dataLen = assemblyData.getAssemblyDataLen();
    emit readyRead(CommandCode::DCLUSTER_CREATE, data, dataLen);

    //页面加载窗口
    if(loadingPage == NULL){
        loadingPage = new LoadingPage(this);
        loadingPage->show();
    }
}

void DClusterCreatePage::on_hintMsgPage(bool isJump)
{
    if(!isJump)
        hintMsgPage->hide();
}

void DClusterCreatePage::on_deleteSubPages()
{
    clusterCreateSuccessedPage = NULL;
}

void DClusterCreatePage::on_btnPrePage_clicked()
{
    this->hide();
    this->deleteLater();
}

void DClusterCreatePage::on_recvDataFromOtherPage(quint16 cmdCode, char* data, quint16 dataLen)
{
    emit readyRead(cmdCode, data, dataLen);
}

DClusterCreatePage::~DClusterCreatePage()
{

}
