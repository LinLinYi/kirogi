#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

#include "CheckButton.h"
#include "ScreenPara.h"


CheckButton::CheckButton(QWidget *parent) : GPushButton(parent)
{
    this->initializeWidget();
}

void CheckButton::initializeWidget()
{
    screenHeight = ScreenPara::getInstance()->screenHeight();
    screenWidth = ScreenPara::getInstance()->screenWidth();

    //头像
    lblHeadImg = new QLabel(this);
    lblHeadImg->setAlignment(Qt::AlignCenter);
    lblHeadImg->setStyleSheet("border-radius: 3px; border-width: 0px;");
    lblHeadImg->setFixedWidth(screenWidth*0.15);
    lblHeadImg->setAttribute(Qt::WA_LayoutUsesWidgetRect);

    //名称
    lblName = new QLabel(this);
    lblName->setAttribute(Qt::WA_LayoutUsesWidgetRect);
    lblName->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    lblName->setFixedHeight(screenHeight*0.04);
    lblName->setStyleSheet(QString("QLabel { font:bold 14px; color:black; border-width: 0px; }"));

    //提示信息
    lblDescribe = new QLabel(this);
    lblDescribe->hide();
    lblDescribe->setAttribute(Qt::WA_LayoutUsesWidgetRect);
    lblDescribe->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    lblDescribe->setStyleSheet(QString("QLabel { color:gray; padding-top: 5px; font: 10px;  border-width: 0px; }"));

    QVBoxLayout* vbInfoLayout = new QVBoxLayout();
    vbInfoLayout->addStretch(0);
    vbInfoLayout->addWidget(lblName);
    vbInfoLayout->addWidget(lblDescribe);
    vbInfoLayout->addStretch(0);
    vbInfoLayout->setMargin(0);
    vbInfoLayout->setSpacing(0);

    //验证按钮
    btnReview = new QPushButton(this);
    btnReview->setText(tr("同意"));
    btnReview->setFixedWidth(screenWidth*0.25);
    btnReview->setAttribute(Qt::WA_LayoutUsesWidgetRect);
    btnReview->setStyleSheet("color:#0090ff; font: 15px; border-width: 0px; border-radius:3px;");
    connect(btnReview, SIGNAL(clicked()), this, SLOT(on_btnReview_clicked()));

    QHBoxLayout* hbLayout = new QHBoxLayout();
    hbLayout->addWidget(lblHeadImg);
    hbLayout->addLayout(vbInfoLayout);
    hbLayout->addWidget(btnReview);
    hbLayout->setMargin(1);
    hbLayout->setSpacing(0);

    this->setLayout(hbLayout);
}

void CheckButton::setClusterID(const quint64 id)
{
    clusterID = id;
}

void CheckButton::setUserID(const quint64 id)
{
    userID = id;
}

void CheckButton::setPixMap(QPixmap pixmap)
{
    headImg = pixmap;
    headImg.setDevicePixelRatio(2);
    lblHeadImg->setPixmap(headImg);
}

void CheckButton::setReviewText(const QString& text)
{
    btnReview->setText(text);
}

void CheckButton::setUserName(const QString& name)
{
    userName = name;
    lblName->setText(userName);
}

void CheckButton::setClusterName(const QString& name)
{
    clusterName = name;
}

void CheckButton::setDescribeInfo(const QString& info)
{
    lblDescribe->setText(info);
    lblDescribe->show();
}

void CheckButton::setCheckType(Mi::ClusterJoin type)
{
    checkType = type;
}

void CheckButton::setCheckStatus(Mi::MsgStatus status)
{
    curCheckStatus = status;
}

void CheckButton::setReserveID(quint64 id)
{
    reserveID = id;
}

void CheckButton::setReserveName(QString &name)
{
    reserveName = name;
}

void CheckButton::setReviewEnable(bool enable)
{
    btnReview->setEnabled(enable);
    if(enable == false)
        btnReview->setStyleSheet(QString("QPushButton { color:#8c8c8c; font: 15px; border-width: 0px; border-radius:3px;}"));
    else
        btnReview->setStyleSheet(QString("QPushButton { color:#0090ff; font: 15px; border-width: 0px; border-radius:3px;}"));
}

void CheckButton::on_btnReview_clicked()
{
    btnReview->setStyleSheet(QString("QPushButton { color:#0090ff; font: 15px; border-width: 0px; border-radius:3px;}"));
    emit reviewClicked();
}

CheckButton::~CheckButton()
{

}
