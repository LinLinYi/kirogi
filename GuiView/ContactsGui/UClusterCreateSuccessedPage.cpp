#include <QLabel>
#include <QFrame>
#include <QVBoxLayout>

#include "BasePage.h"
#include "GroupButton.h"
#include "InputPacket.h"
#include "UClusterInfo.h"
#include "UserProtocol.hpp"
#include "NavigationBar.h"
#include "ImageConfig.hpp"
#include "SheetStyleConfig.hpp"
#include "UClusterCreateSuccessedPage.h"

UClusterCreateSuccessedPage::UClusterCreateSuccessedPage(QWidget* parent, quint64 clusID)
    : BasePage(parent)
    , curClusterID(clusID)
    , btnTitleInfo(NULL)
    , curClusterInfo(NULL)
{
    this->initializeWidget();
}

void UClusterCreateSuccessedPage::initializeWidget()
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
    QPixmap pixmap(ImagePath::UCLUSTER_PORTRAIT);
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

void UClusterCreateSuccessedPage::fetchData(InputPacket& inpack)
{
    quint16 cmdCode = inpack.getCmdCode();
    switch(cmdCode){
        case CommandCode::FETCH_USER_CLUSTER_INFO:
            this->handleClusterInfo(inpack);
            break;
    }
}

void UClusterCreateSuccessedPage::handleClusterInfo(InputPacket &inpack)
{
    quint64 factor = ConstNum::DEVICE_ID_BASIC_POSTFIX;
    char* data = inpack.getPackData();
    SAFE_DELETE(curClusterInfo);
    curClusterInfo = new UClusterInfo;
    curClusterInfo->unpack(data);

    QString clusterID = QString(tr("群ID：")) + QString::number(curClusterID & factor);
    QString clusterDescribe = curClusterInfo->getDescribe();
    QString strDetail = clusterDescribe.leftJustified(15);

    btnTitleInfo->setText(curClusterInfo->getClusterName());
    btnTitleInfo->setRemarks(clusterID);
    btnDescribe->setText(QString(tr("描述")));
    btnDescribe->setDescribe(strDetail);
}

void UClusterCreateSuccessedPage::on_btnPerfactClusterInfo_clicked()
{

}

void UClusterCreateSuccessedPage::on_btnInviationDevice_clicked()
{

}

void UClusterCreateSuccessedPage::on_btnCompleted_clicked()
{
    this->hide();
    this->deleteLater();
    emit readyRead(CommandCode::BACK_CONTACT_PAGE, NULL, 0);
}

UClusterCreateSuccessedPage::~UClusterCreateSuccessedPage()
{

}

