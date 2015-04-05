#include <QLabel>
#include <QFrame>
#include <QFile>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QRegExp>

#include "Keys.h"
#include "ToolKit.h"
#include "FilePath.h"
#include "HintMsgPage.h"
#include "LoadingPage.h"
#include "AssemblyData.h"
#include "UserLoginPage.h"
#include "NavigationBar.h"
#include "UserProtocol.hpp"
#include "ImageConfig.hpp"
#include "GlobalDefine.hpp"
#include "SheetStyleConfig.hpp"

UserLoginPage::UserLoginPage(QWidget* parent, PageType type)
    : BasePage(parent, type)
    , loadingPage(NULL)
    , forgetPwdPage(NULL)
    , mainWindow(NULL)
    , userRegisterPage(NULL)
{
    this->initializeWidget();
    connect(m_pBtnLogin, SIGNAL(clicked()), this, SLOT(on_btnLogin_clicked()));
}

void UserLoginPage::initializeWidget()
{
    //提示信息页面操作
    hintMsgPage = new HintMsgPage(this);
    hintMsgPage->hide();
    connect(hintMsgPage, SIGNAL(submitClicked(bool)), this, SLOT(on_hintMsgPage(bool)));

    //导航栏
    QString strTitle = tr("登 录");
    m_navigationBar = new NavigationBar(this);
    m_navigationBar->setTitleText(strTitle);

    QHBoxLayout* pHLTop = new QHBoxLayout();
    pHLTop->addWidget(m_navigationBar);
    pHLTop->setSpacing(0);
    pHLTop->setMargin(0);
    this->setTopbarLayout(pHLTop);

    //登录顶部背景
    QLabel* topImg = new QLabel(this);
    QPixmap pixmap(ImagePath::LOGIN_TOP);
    pixmap.setDevicePixelRatio(2);
    topImg = new QLabel(this);
    topImg->setPixmap(pixmap);
    topImg->setScaledContents(true);
    topImg->setFixedHeight(this->screenHeight()*0.2);
    topImg->setFixedWidth(this->screenWidth());
    topImg->setMargin(0);

    //账号标签
    QLabel* pLblUserName = new QLabel(this);
    QPixmap namePixmap(ImagePath::LOGIN_USERNAME);
    namePixmap.setDevicePixelRatio(2.0);
    pLblUserName->setPixmap(namePixmap);
    pLblUserName->setAlignment(Qt::AlignCenter);
    pLblUserName->setFixedHeight(this->screenHeight()*0.077);
    pLblUserName->setMaximumWidth(this->screenWidth()*0.15);
    pLblUserName->setStyleSheet(SheetStyle::LOGINNAME_LABEL);

    //密码标签
    QLabel* pLblPwd = new QLabel(this);
    QPixmap pwdPixmap(ImagePath::LOGIN_PASSCODE);
    pwdPixmap.setDevicePixelRatio(2.0);
    pLblPwd->setPixmap(pwdPixmap);
    pLblPwd->setAlignment(Qt::AlignCenter);
    pLblPwd->setFixedHeight(this->screenHeight()*0.077);
    pLblPwd->setMaximumWidth(this->screenWidth()*0.15);
    pLblPwd->setStyleSheet(SheetStyle::LOGINPWD_LABEL);

    //请输入用户名
    m_pEditUserName = new QLineEdit(this);
    m_pEditUserName->setMaxLength(20);
    m_pEditUserName->setFixedHeight(this->screenHeight()*0.077);
    this->setInputMode(m_pEditUserName);
    m_pEditUserName->setStyleSheet(SheetStyle::LOGINNAME_LINEEDIT);
    m_pEditUserName->setPlaceholderText(tr("请输入用户名"));

    //请输入密码
    m_pEditUserPwd = new QLineEdit(this);
    m_pEditUserPwd->setMaxLength(20);
    m_pEditUserPwd->setFixedHeight(this->screenHeight()*0.077);
    this->setInputMode(m_pEditUserPwd);
    m_pEditUserPwd->setEchoMode(QLineEdit::Password);
    m_pEditUserPwd->setStyleSheet(SheetStyle::LOGIPWD_LINEEDIT);
    m_pEditUserPwd->setPlaceholderText(tr("请输入密码"));

    QGridLayout* pGLUserInfo = new QGridLayout();
    pGLUserInfo->addWidget(pLblUserName, 0, 0, 1, 1);
    pGLUserInfo->addWidget(m_pEditUserName, 0, 1, 1, 3);
    pGLUserInfo->addWidget(pLblPwd, 1, 0, 1, 1);
    pGLUserInfo->addWidget(m_pEditUserPwd, 1, 1, 1, 3);
    pGLUserInfo->setSpacing(0);
    pGLUserInfo->setMargin(0);

    QFrame* userInfoFrame = new QFrame(this);
    userInfoFrame->setLayout(pGLUserInfo);
    userInfoFrame->setFixedWidth(this->screenWidth());
    userInfoFrame->setStyleSheet("background-color:white;border-color:rgb(208,208,208);\
                                  border-width:0px; border-top-width:1px; border-bottom-width:1px;");

    //登录按钮
    m_pBtnLogin = new QPushButton(tr("登   录"), this);
    m_pBtnLogin->setFixedHeight(this->screenHeight()*0.074);
    m_pBtnLogin->setStyleSheet(SheetStyle::LOGIN_BUTTON);

    QVBoxLayout* btnLoginLayout = new QVBoxLayout;
    btnLoginLayout->addWidget(m_pBtnLogin);
    btnLoginLayout->setContentsMargins(this->screenWidth()*0.05, this->screenWidth()*0.05, this->screenWidth()*0.05,0);

    //记住密码
    m_pcheckRemPwd = new QCheckBox(tr("记住密码"),this);
    m_pcheckRemPwd->setStyleSheet(tr("color:gray"));
    m_pcheckRemPwd->setFixedHeight(this->screenHeight()*0.075);

    QHBoxLayout* hblRemmberPwd = new QHBoxLayout();
    hblRemmberPwd->addWidget(m_pcheckRemPwd);
    hblRemmberPwd->setAlignment(Qt::AlignRight);
    hblRemmberPwd->setContentsMargins(this->screenWidth()*0.05, 0, this->screenWidth()*0.05,this->screenWidth()*0.05);

    //注册账号
    m_pBtnRegister = new QPushButton(tr("注册账号"), this);
    m_pBtnRegister->setFixedHeight(this->screenHeight()*0.075);
    m_pBtnRegister->setAttribute(Qt::WA_LayoutUsesWidgetRect);
    m_pBtnRegister->setStyleSheet("color:#8c8c8c; font:15px; border-radius:3px; background-color:rgb(0xec,0xec,0xec);");
    connect(m_pBtnRegister, SIGNAL(clicked()), this, SLOT(on_btnRegister_clicked()));

    QLabel* lblLine = new QLabel(tr(" | "),this);
    lblLine->setStyleSheet("color:#8c8c8c;");
    lblLine->setContentsMargins(this->screenWidth()*0.01, 0, this->screenWidth()*0.01, 0);
    m_pBtnForgetPwd = new QPushButton(tr("忘记密码"), this);
    m_pBtnForgetPwd->setFixedHeight(this->screenHeight()*0.075);
    m_pBtnForgetPwd->setAttribute(Qt::WA_LayoutUsesWidgetRect);
    m_pBtnForgetPwd->setStyleSheet("color:#8c8c8c; font:15px; border-radius:3px; background-color:rgb(0xec,0xec,0xec);");
    connect(m_pBtnForgetPwd, SIGNAL(clicked()), this, SLOT(on_btnForgetPwd_clicked()));

    QHBoxLayout* hblRegister = new QHBoxLayout();
    hblRegister->addWidget(m_pBtnRegister);
    hblRegister->addWidget(lblLine);
    hblRegister->addWidget(m_pBtnForgetPwd);
    hblRegister->setAlignment(Qt::AlignCenter);
    hblRegister->setSpacing(0);
    hblRegister->setMargin(0);

    QVBoxLayout* pVLBody = new QVBoxLayout();
    pVLBody->addWidget(topImg);
    pVLBody->addSpacing(this->screenHeight()*0.026);
    pVLBody->addWidget(userInfoFrame);
    pVLBody->addSpacing(this->screenHeight()*0.01);
    pVLBody->addLayout(btnLoginLayout);
    pVLBody->addLayout(hblRemmberPwd);
    pVLBody->addSpacing(this->screenHeight()*0.45);
    pVLBody->addLayout(hblRegister);
    pVLBody->setSpacing(0);
    pVLBody->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    pVLBody->setMargin(0);
    this->setBodyPartLayout(pVLBody);

    //读取登录过的用户名
    this->readLoginInfo();
}

void UserLoginPage::fetchData(InputPacket& inpack)
{
    quint16 cmdCode = inpack.getCmdCode();
    DPRINT("-->UserLoginPage receive data, the cmdCode is:0x%x", cmdCode);

    switch(cmdCode)
    {
        case CommandCode::NET_ERROR:
            this->handleNetError();
            break;
        case CommandCode::LOGIN_FIRST_STEP:
        case CommandCode::LOGIN_SECOND_STEP:
            this->handleLoginInfo(inpack);
            break;
    }

    this->transferDataToAnotherPage(inpack, forgetPwdPage);
    this->transferDataToAnotherPage(inpack, userRegisterPage);
    this->transferDataToAnotherPage(inpack, mainWindow);
}

void UserLoginPage::readLoginInfo()
{
    QFile file(FilePath::userLoginInfoPath());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QString name;
    QString pwd;
    bool state;

    QDataStream in(&file);
    in>>state>>name>>pwd;
    m_pcheckRemPwd->setChecked(state);
    if(!state){
        return;
    }
    else{
        m_pEditUserName->setText(name);
        m_pEditUserPwd->setText(pwd);
    }
}

void UserLoginPage::writeLoginInfo(bool judge)
{
    QFile file(FilePath::userLoginInfoPath());
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QFile::Truncate))
        return;

    QDataStream out(&file);
    file.resize(0);
    out<<judge;

    if(!judge)
       return;
    else{
        out<<m_pEditUserName->text().trimmed();
        out<<m_pEditUserPwd->text().trimmed();
    }

    file.close();
}

void UserLoginPage::handleHintInfo(QString& text)
{
    hintMsgPage->setText(text);
    hintMsgPage->show();
    SAFE_DELETE(loadingPage);
}

void UserLoginPage::handleNetError()
{
    if(loadingPage == NULL) return;
    QString lblText = tr("网络连接错误,请重试");
    this->handleHintInfo(lblText);
}

void UserLoginPage::handleLoginInfo(InputPacket &inpack)
{
    quint16 cmdCode = inpack.getCmdCode();
    quint8 answerCode = inpack.getAnswerCode();

    if(cmdCode == CommandCode::LOGIN_FIRST_STEP && answerCode == Mi::Absent){
        QString lblText = tr("用户名不存在,请重试.");
        handleHintInfo(lblText);
    }
    else if(cmdCode == CommandCode::LOGIN_FIRST_STEP && answerCode == Mi::Success){
        QString strUserPwd = m_pEditUserPwd->text().trimmed();
        AssemblyData assemblyData;
        assemblyData.append(strUserPwd);
        char* data = assemblyData.getAssemblyData();
        emit readyRead(CommandCode::LOGIN_SECOND_STEP, data, strlen(data));
    }
    else if(cmdCode == CommandCode::LOGIN_SECOND_STEP && answerCode == Mi::Error){
        QString lblText = tr("密码错误,请重试.");
        this->handleHintInfo(lblText);
    }
    else if(cmdCode == CommandCode::LOGIN_SECOND_STEP && answerCode == Mi::Success){
        SAFE_DELETE(loadingPage);
        this->handleLoginSuccessInfo(inpack);
    }
}

void UserLoginPage::handleLoginSuccessInfo(InputPacket &inpack)
{
    //创建并显示主页面
    quint64 accountID = inpack.getUserID();

    if(mainWindow == NULL){
        mainWindow = new MainWindow(accountID);
    }

    this->hide();
    mainWindow->show();
    connect(mainWindow, SIGNAL(readyRead(quint16,char*,quint16)), this, SLOT(on_mainWindow(quint16,char*,quint16)));
}

void UserLoginPage::displayConcretePage(BasePage *page)
{
    if(page == NULL) return;
    page->show();
    connect(page, SIGNAL(destroyed()), this, SLOT(on_deleteSubPages()));
    connect(page, SIGNAL(readyRead(quint16,char*,quint16)), this, SLOT(on_recvDataFromOtherPage(quint16,char*,quint16)));
}

void UserLoginPage::setInputMode(QLineEdit* line)
{
    QRegExp regx("[a-zA-Z0-9]+$");
    validator = new QRegExpValidator(regx, line);
    line->setValidator(validator);
}

void UserLoginPage::transferDataToAnotherPage(InputPacket &inpack, BasePage *basePage)
{
    if(basePage == NULL) return;
    basePage->fetchData(inpack);
}

void UserLoginPage::transferDataToAnotherPage(InputPacket &inpack, MainWindow *mainWindow)
{
    if(mainWindow == NULL)  return;
    mainWindow->fetchData(inpack);
}

void UserLoginPage::on_hintMsgPage(bool isJump)
{
    if(!isJump)
        hintMsgPage->hide();
}

void UserLoginPage::on_btnLogin_clicked()
{
    if(m_pEditUserName->text().isEmpty()){
        QString lblText = tr("用户名不能为空,请输入");
        handleHintInfo(lblText);
        return;
    }

    if(m_pEditUserPwd->text().isEmpty()){
        QString lblText = tr("密码不能为空,请输入");
        handleHintInfo(lblText);
        return;
    }

    if(m_pEditUserPwd->text().length() < 6){
        QString lblText = tr("密码错误,请重新填写");
        handleHintInfo(lblText);
        return;
    }

    QString strUserName = m_pEditUserName->text().trimmed();
    AssemblyData assemblyData;
    assemblyData.append(strUserName);
    char* data = assemblyData.getAssemblyData();
    quint16 dataLen = assemblyData.getAssemblyDataLen();
    emit readyRead(CommandCode::LOGIN_FIRST_STEP, data, dataLen);

    //是否记住密码
    if(m_pcheckRemPwd->checkState())
        writeLoginInfo(true);
    else
        writeLoginInfo(false);

    //页面加载窗口
    if(loadingPage == NULL){
        loadingPage = new LoadingPage(this);
        loadingPage->show();
    }
}

void UserLoginPage::on_btnRegister_clicked()
{
    userRegisterPage = new UserRegisterPage(this);
    this->displayConcretePage(userRegisterPage);
}

void UserLoginPage::on_btnForgetPwd_clicked()
{
    forgetPwdPage = new ForgetPwdPage(this);
    this->displayConcretePage(forgetPwdPage);
}

void UserLoginPage::on_deleteSubPages()
{
    forgetPwdPage = NULL;
    userRegisterPage = NULL;
}

void UserLoginPage::on_recvDataFromOtherPage(quint16 cmdCode, char *dataField, quint16 dataLen)
{
    if(cmdCode == CommandCode::BACK_LOGIN_PAGE){
        m_pEditUserName->clear();
        m_pEditUserPwd->clear();
        SAFE_DELETE(userRegisterPage);
        SAFE_DELETE(forgetPwdPage);
        return;
    }
    emit readyRead(cmdCode, dataField, dataLen);
}

void UserLoginPage::on_mainWindow(quint16 cmdCode, char *dataField, quint16 dataLen)
{
    switch(cmdCode){
    case CommandCode::ACCOUNT_LOGOUT:
        SAFE_DELETE(mainWindow);
        this->show();
        break;
    default:
        emit readyRead(cmdCode, dataField, dataLen);
        break;
    }
}

UserLoginPage::~UserLoginPage()
{

}
