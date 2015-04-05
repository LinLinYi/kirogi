#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include "QToolButton"

#include "MorePage.h"
#include "TouchScroller.h"
#include "UserProtocol.hpp"
#include "GlobalData.h"
#include "NavigationBar.h"
#include "InputPacket.h"
#include "GroupButton.h"
#include "UserSettingPage.h"
#include "PersonalInfoPage.h"
#include "ListWidget.h"
#include "NavigationBar.h"
#include "ImageConfig.hpp"
#include "SheetStyleConfig.hpp"

MorePage::MorePage(QWidget* parent, PageType type)
    : BasePage(parent, type)
    , userSettingPage(NULL)
    , personalInfoPage(NULL)
{
    curAccountID = GlobalData::getAccountID();
    curAccountName = GlobalData::getAccountName();
    this->initializeWidget();
}

void MorePage::initializeWidget()
{
    //导航栏
    QString strTitle = tr("更多...");
    navigationBar = new NavigationBar(this);
    navigationBar->setTitleText(strTitle);

    QHBoxLayout* pHLTop = new QHBoxLayout();
    pHLTop->addWidget(navigationBar);
    pHLTop->setSpacing(0);
    pHLTop->setMargin(0);
    this->setTopbarLayout(pHLTop);


    //个人信息
    QLabel* themeWidget = new QLabel;
    QPixmap pixmap = QPixmap(ImagePath::MOREPAGE_THEME);
    pixmap.setDevicePixelRatio(2);
    themeWidget->setAutoFillBackground(true);
    themeWidget->setFixedHeight(this->screenHeight()*0.25);
    themeWidget->setFixedWidth(this->screenWidth());
    themeWidget->setPixmap(pixmap);
    themeWidget->setScaledContents(true);

    btnPersonalInfo = new QToolButton;
    btnPersonalInfo->setFixedWidth(this->screenWidth());
    btnPersonalInfo->setFixedHeight(this->screenHeight()*0.15);
    btnPersonalInfo->setIconSize(QSize(btnPersonalInfo->height()*0.7, btnPersonalInfo->height()*0.7));
    btnPersonalInfo->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    btnPersonalInfo->setStyleSheet("font:16px; color:white;background-color:rgba(0,0,0,0)");
    connect(btnPersonalInfo, SIGNAL(clicked()), SLOT(on_btnPersonal_clicked()));

    QVBoxLayout* themeLayout = new QVBoxLayout;
    themeLayout->addWidget(btnPersonalInfo);
    themeLayout->setMargin(0);
    themeWidget->setLayout(themeLayout);

    //设置
    btnSetting = new GroupButton;
    btnSetting->setStyleSheet(SheetStyle::GROUPBUTTON_BOTTOMBORDER);
    QPixmap settingPixmap(ImagePath::SETTING);
    this->setGroupButton(btnSetting, settingPixmap, tr("设置"));
    connect(btnSetting, SIGNAL(clicked()), SLOT(on_btnSetting_clicked()));

    QVBoxLayout* vblTotalLayout = new QVBoxLayout;
    vblTotalLayout->addWidget(themeWidget);
    vblTotalLayout->addSpacing(this->screenHeight()*0.026);
    vblTotalLayout->addWidget(btnSetting);
    vblTotalLayout->setAlignment(Qt::AlignTop);
    vblTotalLayout->setMargin(0);
    vblTotalLayout->setSpacing(0);
    this->setBodyPartLayout(vblTotalLayout);

    //屏幕触摸滚动设置
    this->setBodyScreenHeight(this->scrollAreaHasBottomBarHeight());
    this->installScrollViewportArea();
    this->loadLocalData(curAccountID);
}

void MorePage::fetchData(InputPacket& inpack)
{
    quint16 cmdCode = inpack.getCmdCode();
    DPRINT("-->MorePage receive data, the cmdCode is:0x%x", cmdCode);

    this->loadLocalData(curAccountID);//刷新当前数据
    this->transferDataToAnotherPage(inpack, userSettingPage);
    this->transferDataToAnotherPage(inpack, personalInfoPage);
}

void MorePage::loadLocalData(quint64 userID)
{
    curUserInfo.loadDataSource(userID);
    if(curUserInfo.getRemarks().length() == 0){
        btnPersonalInfo->setText(curUserInfo.getUserName());
    }else{
        btnPersonalInfo->setText(curUserInfo.getRemarks());
    }

    QString imagePath = curUserInfo.getUserProtrait();
    if(imagePath.length() > 0){
        QPixmap pixmap(imagePath);
        pixmap.setDevicePixelRatio(2);
        btnPersonalInfo->setIcon(pixmap);
    }else{
        QPixmap pixmap(ImagePath::USER);
        pixmap.setDevicePixelRatio(2);
        btnPersonalInfo->setIcon(pixmap);
    }
}

void MorePage::displayConcretePage(BasePage *page)
{
    if(page == NULL) return;
    page->show();
    connect(page, SIGNAL(destroyed()), this, SLOT(on_deleteSubPages()));
    connect(page, SIGNAL(readyRead(quint16,char*,quint16)), this, SLOT(on_recvDataFromOtherPage(quint16,char*,quint16)));
}

void MorePage::transferDataToAnotherPage(InputPacket &inpack, BasePage* basePage)
{
    if(basePage == NULL) return;
    basePage->fetchData(inpack);
}

void MorePage::setListWidget(ListWidget *listWidget)
{
    listWidget->setStyleSheet(SheetStyle::LISTWIDGET);
    listWidget->setFixedWidth(this->screenWidth());
    listWidget->setContentsMargins(this->screenWidth()*0.03,0,0,0);
}

void MorePage::setGroupButton(GroupButton* btn, const QPixmap& photo, const QString& title)
{
    QPixmap titlePixmap = photo;
    btn->setPixMap(titlePixmap);
    btn->setText(title);

    QPixmap pixmap(ImagePath::RIGHT_HOLLOW_ARROWS);
    btn->setReserve(pixmap);
    btn->setAttribute(Qt::WA_LayoutUsesWidgetRect);
    btn->setContentsMargins(this->screenWidth()*0.02, 0,this->screenWidth()*0.02,0);
    btn->setFixedHeight(this->screenHeight()*0.0775);
}

void MorePage::on_btnPersonal_clicked()
{
    emit readyRead(CommandCode::BOTTOM_BAR_UNDISPLAY, NULL, 0);
    personalInfoPage = new PersonalInfoPage(this, curAccountID);
    this->displayConcretePage(personalInfoPage);
}

void MorePage::on_btnSetting_clicked()
{
    emit readyRead(CommandCode::BOTTOM_BAR_UNDISPLAY, NULL, 0);
    userSettingPage = new UserSettingPage(this, curAccountID);
    this->displayConcretePage(userSettingPage);
}

void MorePage::on_deleteSubPages()
{
    userSettingPage = NULL;
    personalInfoPage = NULL;
    this->loadLocalData(curAccountID);//刷新当前数据
    emit readyRead(CommandCode::BOTTOM_BAR_DISPLAY, NULL, 0);//显示底部任务栏
}

void MorePage::on_recvDataFromOtherPage(quint16 cmdCode, char *data, quint16 dataLen)
{
    emit readyRead(cmdCode, data, dataLen);
}

MorePage::~MorePage()
{

}
