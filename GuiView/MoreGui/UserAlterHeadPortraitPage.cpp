#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

#include "UserInfo.h"
#include "UserProtocol.hpp"
#include"ImageConfig.hpp"
#include "ListWidget.h"
#include "GroupButton.h"
#include "ImageConfig.hpp"
#include "NavigationBar.h"
#include "SheetStyleConfig.hpp"
#include "UserAlterHeadPortraitPage.h"

UserAlterHeadPortraitPage::UserAlterHeadPortraitPage(QWidget* parent, quint64 accountID)
    : BasePage(parent)
    , curAccountID(accountID)
    , curFixedContentHeight(0)
    , curSelectedButton(NULL)
{
    this->initializeWidget();
}

void UserAlterHeadPortraitPage::initializeWidget()
{
    //导航栏
    QString strTitle = tr("选择头像");
    QString strLeftBtn = tr("取消");
    QString strRightBtn = tr("提交");
    QPixmap leftArrowsPixmap(ImagePath::NAVIGATION_LEFT_ARROWS);
    navigationBar = new NavigationBar(this);
    navigationBar->setTitleText(strTitle);
    navigationBar->setLeftText(strLeftBtn);
    navigationBar->setLeftPixMap(leftArrowsPixmap);
    navigationBar->setRightText(strRightBtn);
    connect(navigationBar, SIGNAL(leftClicked()), this, SLOT(on_btnPrePage_clicked()));
    connect(navigationBar, SIGNAL(rightClicked()), this, SLOT(on_btnSubmitImage_clicked()));

    QHBoxLayout* pHLTop = new QHBoxLayout();
    pHLTop->addWidget(navigationBar);
    pHLTop->setSpacing(0);
    pHLTop->setMargin(0);
    this->setTopbarLayout(pHLTop);

    //群成员布局
    imgListWidget = new ListWidget(this);
    imgListWidget->setStyleSheet(SheetStyle::LISTWIDGET);
    imgListWidget->setContentsMargins(this->screenWidth()*0.03,0,0,0);
    curFixedContentHeight += imgListWidget->height();
    this->loadLocalImage();

    QVBoxLayout* vblTotalLayout = new QVBoxLayout;
    vblTotalLayout->addWidget(imgListWidget);
    vblTotalLayout->addStretch(0);
    vblTotalLayout->setMargin(0);
    vblTotalLayout->setSpacing(0);
    vblTotalLayout->setAlignment(Qt::AlignTop);
    this->setBodyPartLayout(vblTotalLayout);

    //屏幕触摸滚动设置
    this->installScrollViewportArea();
    this->setBodyRealHeight(curFixedContentHeight);
}

void UserAlterHeadPortraitPage::loadLocalImage()
{
    //可选图片
    imgPathList.append(ImagePath::USER_HEADPORTRAIT001);
    imgPathList.append(ImagePath::USER_HEADPORTRAIT002);
    imgPathList.append(ImagePath::USER_HEADPORTRAIT003);
    imgPathList.append(ImagePath::USER_HEADPORTRAIT004);
    imgPathList.append(ImagePath::USER_HEADPORTRAIT005);
    imgPathList.append(ImagePath::USER_HEADPORTRAIT006);
    imgPathList.append(ImagePath::USER_HEADPORTRAIT007);
    imgPathList.append(ImagePath::USER_HEADPORTRAIT008);
    imgPathList.append(ImagePath::USER_HEADPORTRAIT009);
    imgPathList.append(ImagePath::USER_HEADPORTRAIT010);
    imgPathList.append(ImagePath::USER_HEADPORTRAIT011);
    imgPathList.append(ImagePath::USER_HEADPORTRAIT012);
    imgPathList.append(ImagePath::USER_HEADPORTRAIT013);
    imgPathList.append(ImagePath::USER_HEADPORTRAIT014);
    imgPathList.append(ImagePath::USER_HEADPORTRAIT015);

    for(int i = 0; i < imgPathList.count(); i++){
        GroupButton* btn = new GroupButton(this);
        this->setImageButton(btn, imgPathList.at(i));
        btn->setStyleSheet(SheetStyle::GROUPBUTTON_BOTTOMBORDER);
        imgListWidget->addWidget(btn);
        curFixedContentHeight += btn->height();
    }
    this->setBodyRealHeight(curFixedContentHeight);
}

void UserAlterHeadPortraitPage::fetchData(InputPacket&)
{
    //////////////////////Do Nothing/////////////////////
}

void UserAlterHeadPortraitPage::setImageButton(GroupButton* btn, const QString& userPortrait)
{
    //设备群按钮设置
    if(btn == NULL || userPortrait.length() == 0) return;

    QPixmap pixMap = QPixmap(userPortrait);
    btn->setPixMap(pixMap, this->screenHeight()*0.09, this->screenHeight()*0.09);
    btn->setFixedHeight(this->screenHeight()*0.1);
    btn->setStyleSheet(SheetStyle::GROUPBUTTON_TOPBORDER);
    btn->setAttribute(Qt::WA_LayoutUsesWidgetRect);
    btn->setContentsMargins(0,0,this->screenWidth()*0.03, 0);
    btn->setPortraitPath(userPortrait);

    QPixmap rPixmap(ImagePath::SELECT_CANCEL);
    btn->setReserve(rPixmap);
    connect(btn, SIGNAL(clicked()), this, SLOT(on_btnSelectImage_clicked()));
}

void UserAlterHeadPortraitPage::on_btnPrePage_clicked()
{
    this->hide();
    this->deleteLater();
}

void UserAlterHeadPortraitPage::on_btnSelectImage_clicked()
{
    GroupButton* btn = dynamic_cast<GroupButton*>(this->sender());
    if(curSelectedButton != NULL){
        QPixmap pixmap(ImagePath::SELECT_CANCEL);
        curSelectedButton->setReserve(pixmap);
    }

    curSelectedButton = btn;
    QPixmap rPixmap(ImagePath::SELECT_SUBMIT);
    curSelectedButton->setReserve(rPixmap);
}

void UserAlterHeadPortraitPage::on_btnSubmitImage_clicked()
{
    UserInfo userInfo;
    userInfo.updateHeadPortrait(curSelectedButton->getPortraitPath(), curAccountID);
    this->hide();
    this->deleteLater();
}

UserAlterHeadPortraitPage::~UserAlterHeadPortraitPage()
{

}

