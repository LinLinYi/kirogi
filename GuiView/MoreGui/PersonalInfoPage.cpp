#include <QHBoxLayout>
#include <QVBoxLayout>

#include "PersonalInfoPage.h"
#include "UserProtocol.hpp"
#include "ImageConfig.hpp"
#include "NavigationBar.h"
#include "ImageConfig.hpp"
#include "SheetStyleConfig.hpp"

PersonalInfoPage::PersonalInfoPage(QWidget* parent, quint64 accountID)
    :BasePage(parent)
    , curAccountID(accountID)
    , curAccountInfo(NULL)
    , alterHeadPortraitPage(NULL)
    , alterNicknamePage(NULL)
    , alterSignaturePage(NULL)
{
    this->initializeWidget();
}

void PersonalInfoPage::initializeWidget()
{   
    //导航栏
    QString strTitle = tr("个人信息");
    QString strLeftBtn = tr("更多");
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

    //头像
    btnHeadPortrait = new GroupButton(this);
    QPixmap headPixmap(ImagePath::USER_PORTRAIT);
    btnHeadPortrait->setDescribe(headPixmap);
    btnHeadPortrait->setStyleSheet(SheetStyle::GROUPBUTTON_NOBORDER);
    this->setGroupButton(btnHeadPortrait, QString("头像"), this->screenHeight()*0.123);
    connect(btnHeadPortrait, SIGNAL(clicked()), this, SLOT(on_btnAlterHeadPortrait_clicked()));

    //账号
    QPixmap pixmap;
    btnMacNum = new GroupButton(this);
    this->setGroupButton(btnMacNum, tr("账号"));
    btnMacNum->setReserve(pixmap);
    btnMacNum->setStyleSheet(SheetStyle::GROUPBUTTON_UPANDDOWNBORDER);

    //昵称
    btnUserName = new GroupButton(this);
    this->setGroupButton(btnUserName, tr("昵称"));
    btnUserName->setStyleSheet(SheetStyle::GROUPBUTTON_BOTTOMBORDER);
    connect(btnUserName, SIGNAL(clicked()), this, SLOT(on_btnAlterNicknamePage_clicked()));

    //个性签名
    btnSignatrue = new GroupButton(this);
    this->setGroupButton(btnSignatrue, tr("个性签名"));
    btnSignatrue->setStyleSheet(SheetStyle::GROUPBUTTON_UPANDDOWNBORDER);
    connect(btnSignatrue, SIGNAL(clicked()), this, SLOT(on_btnAlterSignaturePage_clicked()));

    QVBoxLayout* vbLayout = new QVBoxLayout;
    vbLayout->addWidget(btnHeadPortrait);
    vbLayout->addWidget(btnMacNum);
    vbLayout->addWidget(btnUserName);
    vbLayout->addSpacing(this->screenHeight()*0.037);
    vbLayout->addWidget(btnSignatrue);
    vbLayout->setSpacing(0);
    vbLayout->setMargin(0);
    vbLayout->addStretch(0);
    this->setBodyPartLayout(vbLayout);

    //屏幕触摸滚动设置
    this->installScrollViewportArea();
    this->loadLocalData();
}

void PersonalInfoPage::loadLocalData()
{
    curAccountInfo.loadDataSource(curAccountID);

    QString macNum = cutOutString(curAccountInfo.getUserName());
    btnMacNum->setDescribe(macNum);

    QString remarks = cutOutString(curAccountInfo.getRemarks());
    btnUserName->setDescribe(remarks);

    QString signature = cutOutString(curAccountInfo.getDescribe());
    btnSignatrue->setDescribe(signature);

    QString headPhoto = curAccountInfo.getUserProtrait();
    if(headPhoto.length() > 0){
        QPixmap pixmap(headPhoto);
        btnHeadPortrait->setDescribe(pixmap, pixmap.size().width(), pixmap.size().height());
    }
}

void PersonalInfoPage::fetchData(InputPacket& inpack)
{
    this->transferDataToAnotherPage(inpack, alterNicknamePage);
    this->transferDataToAnotherPage(inpack, alterSignaturePage);
}

void PersonalInfoPage::setGroupButton(GroupButton* btn, const QString& title, const quint16 height)
{
    QPixmap pixmap(ImagePath::RIGHT_HOLLOW_ARROWS);
    btn->setReserve(pixmap);
    btn->setText(title);
    btn->setFixedWidth(this->screenWidth());
    btn->setAttribute(Qt::WA_LayoutUsesWidgetRect);

    if(height == 0)
        btn->setFixedHeight(this->screenHeight()*0.0775);
    else
        btn->setFixedHeight(height);
}

void PersonalInfoPage::displayConcretePage(BasePage *page)
{
    if(page == NULL) return;
    page->show();
    connect(page, SIGNAL(destroyed()), this, SLOT(on_deleteSubPages()));
    connect(page, SIGNAL(readyRead(quint16,char*,quint16)), this, SLOT(on_recvDataFromOtherPage(quint16,char*,quint16)));
}

void PersonalInfoPage::transferDataToAnotherPage(InputPacket& inpack, BasePage *basePage)
{
    if(basePage == NULL) return;
    basePage->fetchData(inpack);
}

void PersonalInfoPage::on_btnPrePage_clicked()
{
    this->hide();
    this->deleteLater();
}

void PersonalInfoPage::on_btnAlterHeadPortrait_clicked()
{
    alterHeadPortraitPage = new UserAlterHeadPortraitPage(this, curAccountID);
    this->displayConcretePage(alterHeadPortraitPage);
}

void PersonalInfoPage::on_btnAlterNicknamePage_clicked()
{
    alterNicknamePage = new UserAlterNicknamePage(this, curAccountID);
    this->displayConcretePage(alterNicknamePage);
}

void PersonalInfoPage::on_btnAlterSignaturePage_clicked()
{
    alterSignaturePage = new UserAlterSignaturePage(this, curAccountID);
    this->displayConcretePage(alterSignaturePage);
}

void PersonalInfoPage::on_deleteSubPages()
{
    alterNicknamePage = NULL;
    alterSignaturePage = NULL;
    alterHeadPortraitPage = NULL;
    this->loadLocalData();
}

void PersonalInfoPage::on_recvDataFromOtherPage(quint16 cmdCode, char *data, quint16 dataLen)
{
    emit readyRead(cmdCode, data, dataLen);
}

QString PersonalInfoPage::cutOutString(const QString &str)
{
    QString cutStr = str;
    if(cutStr.length() > 12)
        cutStr = str.leftJustified(12, '.', true) + QString("...");
    else if(cutStr.length() == 0)
        cutStr = QString("");

    return cutStr;
}

PersonalInfoPage::~PersonalInfoPage()
{

}
