#include <QLabel>
#include <QVBoxLayout>

#include "GroupButton.h"
#include "InputPacket.h"
#include "DClusterInfo.h"
#include "UserProtocol.hpp"
#include "NavigationBar.h"
#include "ImageConfig.hpp"
#include "NavigationBar.h"
#include "LoadingPage.h"
#include "HintMsgPage.h"
#include "SheetStyleConfig.hpp"
#include "DClusterCreateSuccessedPage.h"

DClusterCreateSuccessedPage::DClusterCreateSuccessedPage(QWidget* parent, quint64 clusID)
    : BasePage(parent)
    , curClusterID(clusID)
    , btnTitleInfo(NULL)
    , curClusterInfo(NULL)
{
    this->initializeWidget();
}

void DClusterCreateSuccessedPage::initializeWidget()
{
    //导航栏
    QString strTitle = tr("创建成功");
    navigationBar = new NavigationBar(this);
    navigationBar->setTitleText(strTitle);

    QHBoxLayout* pHLTop = new QHBoxLayout();
    pHLTop->addWidget(navigationBar);
    pHLTop->setSpacing(0);
    pHLTop->setMargin(0);
    this->setTopbarLayout(pHLTop);

    //群标题信息
    QPixmap pixmap(ImagePath::DCLUSTER_PORTRAIT);
    btnTitleInfo = new GroupButton(this);
    btnTitleInfo->setPixMap(pixmap);
    btnTitleInfo->setFixedHeight(this->screenHeight()*0.12);
    btnTitleInfo->setFixedWidth(this->screenWidth());
    btnTitleInfo->setAttribute(Qt::WA_LayoutUsesWidgetRect);

    btnDescribe = new GroupButton(this);
    btnDescribe->setAttribute(Qt::WA_LayoutUsesWidgetRect);
    btnDescribe->setStyleSheet(SheetStyle::GROUPBUTTON_NOBORDER);
    btnDescribe->setText(tr("描述"));
    btnDescribe->setFixedHeight(this->screenHeight()*0.08);
    btnDescribe->setContentsMargins(this->screenWidth()*0.05, 0, this->screenWidth()*0.05, 0);

    //完成
    btnComplete = new QPushButton(tr("完成"),this);
    btnComplete->setFixedHeight(this->screenHeight()*0.074);
    btnComplete->setStyleSheet(SheetStyle::CREATECLUSTER_SUCCESS_BUTTON);
    connect(btnComplete, SIGNAL(clicked()), this, SLOT(on_btnCompleted_clicked()));

    QVBoxLayout* vbBtnComplete = new QVBoxLayout();
    vbBtnComplete->addWidget(btnComplete);
    vbBtnComplete->setContentsMargins(this->screenWidth()*0.054, 0, this->screenWidth()*0.054, 0);

    QVBoxLayout* vbTotalLayout = new QVBoxLayout();
    vbTotalLayout->addWidget(btnTitleInfo);
    vbTotalLayout->addWidget(btnDescribe);
    vbTotalLayout->addSpacing(this->screenHeight()*0.08);
    vbTotalLayout->addLayout(vbBtnComplete);
    vbTotalLayout->setAlignment(Qt::AlignTop);
    vbTotalLayout->setMargin(0);
    vbTotalLayout->setSpacing(0);
    vbTotalLayout->addStretch(0);
    this->setBodyPartLayout(vbTotalLayout);

    //屏幕触摸滚动设置
    this->installScrollViewportArea();
}

void DClusterCreateSuccessedPage::fetchData(InputPacket& inpack)
{
    quint16 cmdCode = inpack.getCmdCode();
    switch(cmdCode){
        case CommandCode::FETCH_DCLUSTER_INFO:
            this->handleClusterInfo(inpack);
            break;
    }
}

void DClusterCreateSuccessedPage::handleClusterInfo(InputPacket &inpack)
{
    SAFE_DELETE(curClusterInfo);
    quint64 factor = ConstNum::DEVICE_ID_BASIC_POSTFIX;
    char* data = inpack.getPackData();
    curClusterInfo = new DClusterInfo;
    curClusterInfo->unpack(data);

    QString clusterName = curClusterInfo->getClusterName();
    QString clusterID = QString(tr("群ID：")) + QString::number(curClusterID & factor);
    QString clusterDescribe = curClusterInfo->getDescribe();
    QString strDetail = clusterDescribe.leftJustified(15);

    btnTitleInfo->setText(clusterName);
    btnTitleInfo->setRemarks(clusterID);
    btnDescribe->setText(QString(tr("描述")));
    btnDescribe->setDescribe(strDetail);
}

void DClusterCreateSuccessedPage::on_btnPerfactClusterInfo_clicked()
{

}

void DClusterCreateSuccessedPage::on_btnInviationDevice_clicked()
{

}

void DClusterCreateSuccessedPage::on_btnCompleted_clicked()
{
    emit readyRead(CommandCode::BACK_DEVICE_PAGE, NULL, 0);
}

DClusterCreateSuccessedPage::~DClusterCreateSuccessedPage()
{

}

