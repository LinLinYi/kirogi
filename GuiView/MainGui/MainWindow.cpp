#include <QHBoxLayout>
#include <QGridLayout>
#include <QtGlobal>
#include <QFrame>
#include <QVBoxLayout>
#include <QToolButton>

#include "BasePage.h"
#include "InputPacket.h"
#include "HomePage.h"
#include "ContactPage.h"
#include "DevicePage.h"
#include "MorePage.h"
#include "MainWindow.h"
#include "ToolKit.h"
#include "ScreenPara.h"
#include "UserProtocol.hpp"
#include "ImageConfig.hpp"
#include "SheetStyleConfig.hpp"
#include "CreateSqlitetables.hpp"

#if defined Q_OS_WIN32 || defined Q_OS_WIN64 || defined Q_OS_WIN
    using namespace std;
    # pragma execution_character_set("utf-8")
#endif

MainWindow::MainWindow(quint64 accountID)
    :curAccountID(accountID)
    , homePage(NULL)
    , devicePage(NULL)
    , contactPage(NULL)
    , morePage(NULL)
{
    m_screenCurWidth = this->screenWidth();
    m_screenCurHeight = this->screenHeight();
    m_statusHeight = this->statusBarHeight();

#ifdef Q_OS_IOS
    switch(m_screenCurHeight)
    {
        case 568:
            m_statusHeight = 20;
            m_topBarHeight = 44;
            m_bottomBarHeight = 49 + 1;
            break;
        default:
            m_topBarHeight = m_screenCurHeight*0.078;
            m_bottomBarHeight = m_screenCurHeight*0.087 + 1;
            break;
    }
#else
    m_topBarHeight = m_screenCurHeight*0.078;
    m_bottomBarHeight = m_screenCurHeight*0.087;
#endif

    this->initLocalDataBase();
    this->initializeWidget();
}

void MainWindow::initializeWidget()
{
    //登录成功后，初始化相关页面参数
    homePage = new HomePage(this); homePage->show();
    contactPage = new ContactPage(this); contactPage->hide();
    devicePage = new DevicePage(this); devicePage->hide();
    morePage = new MorePage(this); morePage->hide();

    m_curDestPage = homePage;
    this->setFixedWidth(m_screenCurWidth);
    this->setFixedHeight(m_screenCurHeight);

    //主窗口底部
    m_bottomBar = new QFrame(this);
    m_bottomBar->setFixedHeight(m_bottomBarHeight);
    m_bottomBar->setFixedWidth(m_screenCurWidth);
    m_bottomBar->setStyleSheet(SheetStyle::TASKBAR);

    //主窗口底部切换按钮
    m_btnHomePage = new QToolButton(this);
    m_btnContactsPage = new QToolButton(this);
    m_btnDevicePage =  new QToolButton(this);
    m_btnMorePage = new QToolButton(this);

    //"首页"按钮
    m_btnHomePage->setText(tr("首页"));
    this->setToolButton(m_btnHomePage, ImagePath::HOMEPAGE_SELECTED);
    m_btnHomePage->setStyleSheet(SheetStyle::TOOLBUTTON_BLUE_13);
    connect(m_btnHomePage, SIGNAL(clicked()), this, SLOT(on_btnHomePage_Clicked()));

    //"设备"按钮
    m_btnDevicePage->setText(tr("设备"));
    this->setToolButton(m_btnDevicePage, ImagePath::DEVICEPAGE_CANCELED);
    connect(m_btnDevicePage, SIGNAL(clicked()), this, SLOT(on_btnDevicePage_Clicked()));

    //"联系人"按钮
    m_btnContactsPage->setText(tr("联系人"));
    this->setToolButton(m_btnContactsPage, ImagePath::CONTACTPAGE_CANCELED);
    connect(m_btnContactsPage, SIGNAL(clicked()), this, SLOT(on_btnContactsPage_Clicked()));

    //"更多"按钮
    m_btnMorePage->setText(tr("更多"));
    this->setToolButton(m_btnMorePage, ImagePath::MOREPAGE_CANCELED);
    connect(m_btnMorePage, SIGNAL(clicked()), this, SLOT(on_btnMorePage_Clicked()));

    //按钮布局
    QHBoxLayout* pHbBottomTaskBar = new QHBoxLayout(this);
    pHbBottomTaskBar->addWidget(m_btnHomePage);
    pHbBottomTaskBar->addWidget(m_btnContactsPage);
    pHbBottomTaskBar->addWidget(m_btnDevicePage);
    pHbBottomTaskBar->addWidget(m_btnMorePage);
    pHbBottomTaskBar->setContentsMargins(0,0,0,0);
    pHbBottomTaskBar->setSpacing(m_screenCurWidth*0.05);
    m_bottomBar->setLayout(pHbBottomTaskBar);

    /*页面整体布局*/
    m_mainLayout.addWidget(m_curDestPage, 0);
    m_mainLayout.addWidget(m_bottomBar, 1);
    m_mainLayout.setContentsMargins(0,0,0,0);
    m_mainLayout.setAlignment(Qt::AlignTop);
    m_mainLayout.setSpacing(0);
    this->setLayout(&m_mainLayout);

    //相关信号与槽的处理
    connect(homePage, SIGNAL(readyRead(quint16,char*,quint16)), this, SLOT(on_subPage(quint16,char*,quint16)));
    connect(contactPage, SIGNAL(readyRead(quint16,char*,quint16)), this, SLOT(on_subPage(quint16,char*,quint16)));
    connect(devicePage, SIGNAL(readyRead(quint16,char*,quint16)), this, SLOT(on_subPage(quint16,char*,quint16)));
    connect(morePage, SIGNAL(readyRead(quint16,char*,quint16)), this, SLOT(on_subPage(quint16,char*,quint16)));
}

void MainWindow::initLocalDataBase()
{
    CreateSqliteTables::CreateTables(curAccountID);
}

void MainWindow::setToolButton(QToolButton *btn, QString imagePath)
{
    QIcon icon = QIcon(imagePath);
    btn->setIcon(icon);
    btn->setIconSize(QSize(btn->height()*0.9, btn->height()*0.9));
    btn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    btn->setStyleSheet(SheetStyle::TOOLBUTTON_GREY_13);
}

void MainWindow::adjustPages(BasePage *page)
{
    m_curDestPage->hide();
    m_mainLayout.removeWidget(m_curDestPage);
    m_mainLayout.insertWidget(0, page);
    m_curDestPage = page;
    m_curDestPage->show();//列表切换
}

void MainWindow::labelHintInfoDisplay()
{
    if(m_curDestPage == homePage){
        m_btnHomePage->setStyleSheet(SheetStyle::TOOLBUTTON_BLUE_13);
        m_btnContactsPage->setStyleSheet(SheetStyle::TOOLBUTTON_GREY_13);
        m_btnDevicePage->setStyleSheet(SheetStyle::TOOLBUTTON_GREY_13);
        m_btnMorePage->setStyleSheet(SheetStyle::TOOLBUTTON_GREY_13);

        m_btnHomePage->setIcon(QPixmap(ImagePath::HOMEPAGE_SELECTED));
        m_btnContactsPage->setIcon(QPixmap(ImagePath::CONTACTPAGE_CANCELED));
        m_btnDevicePage->setIcon(QPixmap(ImagePath::DEVICEPAGE_CANCELED));
        m_btnMorePage->setIcon(QPixmap(ImagePath::MOREPAGE_CANCELED));

    }else if(m_curDestPage == contactPage){
        m_btnHomePage->setStyleSheet(SheetStyle::TOOLBUTTON_GREY_13);
        m_btnContactsPage->setStyleSheet(SheetStyle::TOOLBUTTON_BLUE_13);
        m_btnDevicePage->setStyleSheet(SheetStyle::TOOLBUTTON_GREY_13);
        m_btnMorePage->setStyleSheet(SheetStyle::TOOLBUTTON_GREY_13);

        m_btnHomePage->setIcon(QPixmap(ImagePath::HOMEPAGE_CANCELED));
        m_btnContactsPage->setIcon(QPixmap(ImagePath::CONTACTPAGE_SELECTED));
        m_btnDevicePage->setIcon(QPixmap(ImagePath::DEVICEPAGE_CANCELED));
        m_btnMorePage->setIcon(QPixmap(ImagePath::MOREPAGE_CANCELED));

    }else if(m_curDestPage == devicePage){
        m_btnHomePage->setStyleSheet(SheetStyle::TOOLBUTTON_GREY_13);
        m_btnContactsPage->setStyleSheet(SheetStyle::TOOLBUTTON_GREY_13);
        m_btnDevicePage->setStyleSheet(SheetStyle::TOOLBUTTON_BLUE_13);
        m_btnMorePage->setStyleSheet(SheetStyle::TOOLBUTTON_GREY_13);

        m_btnHomePage->setIcon(QPixmap(ImagePath::HOMEPAGE_CANCELED));
        m_btnContactsPage->setIcon(QPixmap(ImagePath::CONTACTPAGE_CANCELED));
        m_btnDevicePage->setIcon(QPixmap(ImagePath::DEVICEPAGE_SELECTED));
        m_btnMorePage->setIcon(QPixmap(ImagePath::MOREPAGE_CANCELED));
    }else if(m_curDestPage == morePage){
        m_btnHomePage->setStyleSheet(SheetStyle::TOOLBUTTON_GREY_13);
        m_btnContactsPage->setStyleSheet(SheetStyle::TOOLBUTTON_GREY_13);
        m_btnDevicePage->setStyleSheet(SheetStyle::TOOLBUTTON_GREY_13);
        m_btnMorePage->setStyleSheet(SheetStyle::TOOLBUTTON_BLUE_13);

        m_btnMorePage->setStyleSheet(SheetStyle::TOOLBUTTON_BLUE_13);
        m_btnHomePage->setIcon(QPixmap(ImagePath::HOMEPAGE_CANCELED));
        m_btnContactsPage->setIcon(QPixmap(ImagePath::CONTACTPAGE_CANCELED));
        m_btnDevicePage->setIcon(QPixmap(ImagePath::DEVICEPAGE_CANCELED));
        m_btnMorePage->setIcon(QPixmap(ImagePath::MOREPAGE_SELECTED));
    }
}

void MainWindow::fetchData(InputPacket &inpack)
{
    quint16 cmdCode = inpack.getCmdCode();
    DPRINT("-->MainWindow receive data, the cmdCode is:0x%x", cmdCode);

    this->transferDataToAnotherPage(inpack, homePage);
    this->transferDataToAnotherPage(inpack, devicePage);
    this->transferDataToAnotherPage(inpack, contactPage);
    this->transferDataToAnotherPage(inpack, morePage);
}

void MainWindow::transferDataToAnotherPage(InputPacket &inpack, BasePage *basePage)
{
    if(basePage == NULL) return;
    basePage->fetchData(inpack);
}

void MainWindow::on_btnHomePage_Clicked()
{
    this->adjustPages(homePage);
    this->labelHintInfoDisplay();
}

void MainWindow::on_btnContactsPage_Clicked()
{
    this->adjustPages(contactPage);
    this->labelHintInfoDisplay();
}

void MainWindow::on_btnDevicePage_Clicked()
{
    this->adjustPages(devicePage);
    this->labelHintInfoDisplay();
}

void MainWindow::on_btnMorePage_Clicked()
{
    this->adjustPages(morePage);
    this->labelHintInfoDisplay();
}

void MainWindow::on_subPage(quint16 cmdCode, char *packData, quint16 dataLen)
{
    switch(cmdCode){
        case CommandCode::BOTTOM_BAR_UNDISPLAY:
            m_bottomBar->hide();
            break;
        case CommandCode::BOTTOM_BAR_DISPLAY:
            m_bottomBar->show();
            break;
        default:
            if(cmdCode > CommandCode::MIN_VALUE && cmdCode <= CommandCode::MAX_VALUE){//检测发送到网络的功能码
                emit readyRead(cmdCode, packData, dataLen);
            }
    }
}

MainWindow::~MainWindow(){}





