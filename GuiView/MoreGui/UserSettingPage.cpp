#include <QHBoxLayout>
#include <QVBoxLayout>

#include "InputPacket.h"
#include "UserSettingPage.h"
#include "TouchScroller.h"
#include "UserProtocol.hpp"
#include "StatusBarParam.h"
#include "NavigationBar.h"
#include "ImageConfig.hpp"
#include "SheetStyleConfig.hpp"

UserSettingPage::UserSettingPage(QWidget* parent, quint64 accountID)
    : BasePage(parent)
    , curAccountID(accountID)
    , accountSafePage(NULL)
    , softwareFeedBackPage(NULL)
    , softwareAboutPage(NULL)
{
    this->initializeWidget();
}

void UserSettingPage::initializeWidget()
{
    //导航栏
    QString strTitle = tr("设置");
    QString strLeftBtn = tr("返回");
    QPixmap leftArrowsPixmap(ImagePath::NAVIGATION_LEFT_ARROWS);
    navigationBar = new NavigationBar(this);
    navigationBar->setTitleText(strTitle);
    navigationBar->setLeftText(strLeftBtn);
    navigationBar->setLeftPixMap(leftArrowsPixmap);
    connect(navigationBar, SIGNAL(leftClicked()), this, SLOT(on_btnPrePage_clicked()));

    QHBoxLayout* pHLTop = new QHBoxLayout();
    pHLTop->addWidget(navigationBar);
    pHLTop->setSpacing(0);
    pHLTop->setMargin(0);
    this->setTopbarLayout(pHLTop);

    //账号与安全
    btnAccountSafe = new GroupButton(this);
    this->setGroupButton(btnAccountSafe, tr("账号与安全"));
    btnAccountSafe->setContentsMargins(this->screenWidth()*0.03,0,this->screenWidth()*0.02,0);
    btnAccountSafe->setStyleSheet(SheetStyle::GROUPBUTTON_UPANDDOWNBORDER);
    connect(btnAccountSafe, SIGNAL(clicked()), SLOT(on_btnAccountSafe_clicked()));

    //反馈
    btnFeedback = new GroupButton(this);
    this->setGroupButton(btnFeedback, tr("反馈"));
    btnFeedback->setStyleSheet(SheetStyle::GROUPBUTTON_BOTTOMBORDER);
    connect(btnFeedback, SIGNAL(clicked()), SLOT(on_btnFeedback_clicked()));

    //关于
    btnAbout = new GroupButton(this);
    this->setGroupButton(btnAbout, tr("关于"));
    btnAbout->setStyleSheet(SheetStyle::GROUPBUTTON_NOBORDER);
    connect(btnAbout, SIGNAL(clicked()), SLOT(on_btnAbout_clicked()));

    generalListWidget = new ListWidget(this);
    generalListWidget->addWidget(btnFeedback);
    generalListWidget->addWidget(btnAbout);
    this->setListWidget(generalListWidget);

    //退出登录
    btnExitLogin = new QPushButton(this);
    btnExitLogin->setText(tr("退出登录"));
    btnExitLogin->setAttribute(Qt::WA_LayoutUsesWidgetRect);
    btnExitLogin->setFixedHeight(this->screenHeight()*0.074);
    btnExitLogin->setStyleSheet("color:white; background-color:rgb(236,80,80); border-radius: 0px; font:16px");
    connect(btnExitLogin, SIGNAL(clicked()), this, SLOT(on_btnExitLogin_clicked()));

    QVBoxLayout* vblTotalLayout = new QVBoxLayout;
    vblTotalLayout->addSpacing(this->screenHeight()*0.026);
    vblTotalLayout->addWidget(btnAccountSafe);
    vblTotalLayout->addSpacing(this->screenHeight()*0.037);
    vblTotalLayout->addWidget(generalListWidget);
    vblTotalLayout->addSpacing(this->screenHeight()*0.037);
    vblTotalLayout->addWidget(btnExitLogin);
    vblTotalLayout->setMargin(0);
    vblTotalLayout->setSpacing(0);
    vblTotalLayout->addStretch(0);
    this->setBodyPartLayout(vblTotalLayout);

    //屏幕触摸滚动设置
    this->installScrollViewportArea();
}

void UserSettingPage::fetchData(InputPacket& inpack)
{
    quint16 cmdCode = inpack.getCmdCode();
    DPRINT("-->UserSettingPage receive data, the cmdCode is:0x%x", cmdCode);
    this->transferDataToAnotherPage(inpack, accountSafePage);
    this->transferDataToAnotherPage(inpack, softwareFeedBackPage);
}

void UserSettingPage::setListWidget(ListWidget *listWidget)
{
    listWidget->setStyleSheet(SheetStyle::LISTWIDGET);
    listWidget->setFixedWidth(this->screenWidth());
    listWidget->setContentsMargins(this->screenWidth()*0.03,0,0,0);
}

void UserSettingPage::setGroupButton(GroupButton* groupButton, const QString& title)
{
    QPixmap pixmap(ImagePath::RIGHT_HOLLOW_ARROWS);
    groupButton->setReserve(pixmap);
    groupButton->setText(title);
    groupButton->setAttribute(Qt::WA_LayoutUsesWidgetRect);
    groupButton->setContentsMargins(0,0,this->screenWidth()*0.02, 0);
    groupButton->setFixedHeight(this->screenHeight()*0.0775);
}

void UserSettingPage::displayConcretePage(BasePage *page)
{
    if(page == NULL) return;
    page->show();
    connect(page, SIGNAL(destroyed()), this, SLOT(on_deleteSubPages()));
    connect(page, SIGNAL(readyRead(quint16,char*,quint16)), this, SLOT(on_recvDataFromOtherPage(quint16,char*,quint16)));
}

void UserSettingPage::transferDataToAnotherPage(InputPacket &inpack, BasePage *basePage)
{
    if(basePage == NULL) return;
    basePage->fetchData(inpack);
}

void UserSettingPage::on_recvDataFromOtherPage(quint16 cmdCode, char *data, quint16 dataLen)
{
    emit readyRead(cmdCode, data, dataLen);
}

void UserSettingPage::on_btnAccountSafe_clicked()
{
    accountSafePage = new AccountSafePage(this, curAccountID);
    this->displayConcretePage(accountSafePage);
}

void UserSettingPage::on_btnFeedback_clicked()
{
    softwareFeedBackPage = new SoftwareFeedBackPage(this, curAccountID);
    this->displayConcretePage(softwareFeedBackPage);
}

void UserSettingPage::on_btnAbout_clicked()
{
    softwareAboutPage = new SoftwareAboutPage(this, curAccountID);
    this->displayConcretePage(softwareAboutPage);
}

void UserSettingPage::on_btnExitLogin_clicked()
{
    emit readyRead(CommandCode::ACCOUNT_LOGOUT, NULL, 0);
}

void UserSettingPage::on_btnPrePage_clicked()
{
    this->hide();
    this->deleteLater();
}

void UserSettingPage::on_deleteSubPages()
{
    accountSafePage = NULL;
    softwareFeedBackPage = NULL;
    softwareAboutPage = NULL;
}

UserSettingPage::~UserSettingPage()
{

}

