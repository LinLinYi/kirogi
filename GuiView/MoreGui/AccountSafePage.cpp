#include <QHBoxLayout>
#include <QVBoxLayout>

#include "InputPacket.h"
#include "AccountSafePage.h"
#include "TouchScroller.h"
#include "UserProtocol.hpp"
#include "ImageConfig.hpp"
#include "NavigationBar.h"
#include "SheetStyleConfig.hpp"

AccountSafePage::AccountSafePage(QWidget* parent, quint64 accountID)
    : BasePage(parent)
    , curAccountID(accountID)
    , userPwdVerifyPage(NULL)
    , userMailModifyPage(NULL)
    , userInfo(NULL)
{
    userInfo = new UserInfo();
    userInfo->loadDataSource(curAccountID);
    this->initializeWidget();
}

void AccountSafePage::initializeWidget()
{
    //导航栏
    QString strTitle = tr("账号与安全");
    QString strLeftBtn = tr("设置");
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

    //账号
    btnMacNum = new GroupButton(this);
    btnMacNum->setStyleSheet(SheetStyle::GROUPBUTTON_BOTTOMBORDER);
    this->setGroupButton(btnMacNum, tr("账号"));
    btnMacNum->setDescribe(userInfo->getUserName());

    //邮箱
    btnMail = new GroupButton(this);
    btnMail->setStyleSheet(SheetStyle::GROUPBUTTON_BOTTOMBORDER);
    this->setGroupButton(btnMail, tr("邮箱"));
    connect(btnMail, SIGNAL(clicked()), SLOT(on_btnMail_clicked()));

    //密码
    btnMacPwd = new GroupButton(this);
    btnMacPwd->setStyleSheet(SheetStyle::GROUPBUTTON_NOBORDER);
    this->setGroupButton(btnMacPwd, tr("密码"));
    connect(btnMacPwd, SIGNAL(clicked()), SLOT(on_btnMacPwd_clicked()));

    modifyListWidget = new ListWidget(this);
    modifyListWidget->addWidget(btnMacNum);
    modifyListWidget->addWidget(btnMail);
    modifyListWidget->addWidget(btnMacPwd);
    this->setListWidget(modifyListWidget);

    QVBoxLayout* vblTotalLayout = new QVBoxLayout();
    vblTotalLayout->addSpacing(this->screenHeight()*0.026);
    vblTotalLayout->addWidget(modifyListWidget);
    vblTotalLayout->setMargin(0);
    vblTotalLayout->setSpacing(0);
    vblTotalLayout->addStretch(0);
    this->setBodyPartLayout(vblTotalLayout);

    //屏幕触摸滚动设置
    this->setBodyScreenHeight(this->scrollAreaHasBottomBarHeight());
    this->installScrollViewportArea();
}

void AccountSafePage::fetchData(InputPacket& inpack)
{
    quint16 cmdCode = inpack.getCmdCode();
    DPRINT("-->AccountSafePage receive data, the cmdCode is:0x%x", cmdCode);

    this->transferDataToAnotherPage(inpack, userMailModifyPage);
    this->transferDataToAnotherPage(inpack, userPwdVerifyPage);
}

void AccountSafePage::displayConcretePage(BasePage *page)
{
    if(page == NULL) return;
    page->show();
    connect(page, SIGNAL(destroyed()), this, SLOT(on_deleteSubPages()));
    connect(page, SIGNAL(readyRead(quint16,char*,quint16)), this, SLOT(on_recvDataFromOtherPage(quint16,char*,quint16)));
}

void AccountSafePage::transferDataToAnotherPage(InputPacket &inpack, BasePage* basePage)
{
    if(basePage == NULL) return;
    basePage->fetchData(inpack);
}

void AccountSafePage::transferDataToAnotherPage(InputPacket &inpack, UserVerifyPasswordPage* page)
{
    if(page == NULL) return;
    page->fetchData(inpack);
}

void AccountSafePage::setListWidget(ListWidget *listWidget)
{
    listWidget->setStyleSheet(SheetStyle::LISTWIDGET);
    listWidget->setFixedWidth(this->screenWidth());
    listWidget->setContentsMargins(this->screenWidth()*0.03,0,0,0);
}

void AccountSafePage::setGroupButton(GroupButton* gButton, const QString& title)
{
    gButton->setText(title);
    QPixmap pixmap(ImagePath::RIGHT_HOLLOW_ARROWS);
    gButton->setReserve(pixmap);
    gButton->setAttribute(Qt::WA_LayoutUsesWidgetRect);
    gButton->setContentsMargins(0,0,this->screenWidth()*0.02,0);
    gButton->setFixedHeight(this->screenHeight()*0.0775);
}

void AccountSafePage::on_btnMail_clicked()
{
    userMailModifyPage = new UserAlterMailPage(this);
    this->displayConcretePage(userMailModifyPage);
}

void AccountSafePage::on_btnMacPwd_clicked()
{
    userPwdVerifyPage = new UserVerifyPasswordPage(this, curAccountID);
    userPwdVerifyPage->show();
    connect(userPwdVerifyPage, SIGNAL(destroyed()), this, SLOT(on_deleteSubPages()));
    connect(userPwdVerifyPage, SIGNAL(readyRead(quint16,char*,quint16)), this, SLOT(on_recvDataFromOtherPage(quint16,char*,quint16)));
}

void AccountSafePage::on_deleteSubPages()
{
    userPwdVerifyPage = NULL;
    userMailModifyPage = NULL;
}

void AccountSafePage::on_recvDataFromOtherPage(quint16 cmdCode, char *data, quint16 dataLen)
{
    if(cmdCode == CommandCode::BACK_ACCOUNT_SAFE_PAGE){
        on_deleteSubPages();
        return;
    }
    emit readyRead(cmdCode, data, dataLen);
}

void AccountSafePage::on_btnPrePage_clicked()
{
    this->hide();
    this->deleteLater();
}

AccountSafePage::~AccountSafePage()
{
    SAFE_DELETE(userInfo);
}
