#ifndef USERLOGINPAGE_H
#define USERLOGINPAGE_H

#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QCheckBox>
#include <QValidator>

#include "BasePage.h"
#include "InputPacket.h"
#include "ForgetPwdPage.h"
#include "MainWindow.h"
#include "UserRegisterPage.h"

class UserLoginPage : public BasePage
{
    Q_OBJECT
public:
    explicit UserLoginPage(QWidget* parent = 0, PageType type = MainPage);
    ~UserLoginPage();

public:
    void fetchData(InputPacket& inpack);

private:
    NavigationBar* m_navigationBar;/*! 导航栏*/
    QLineEdit* m_pEditUserName;    /*! 用户名输入框*/
    QLineEdit* m_pEditUserPwd;     /*! 用户密码输入框*/
    QPushButton* m_pBtnLogin;      /*! 登录按钮*/
    QCheckBox* m_pcheckRemPwd;     /*! 记住密码*/
    QPushButton* m_pBtnRegister;   /*! 注册按钮*/
    QPushButton* m_pBtnForgetPwd;  /*! 忘记密码按钮*/

    QValidator* validator;
    LoadingPage* loadingPage;      /*! 加载页面*/
    HintMsgPage* hintMsgPage;      /*! 消息提示页面*/
    ForgetPwdPage* forgetPwdPage;  /*! 忘记密码页面*/
    MainWindow* mainWindow;        /*! 主页面*/
    UserRegisterPage* userRegisterPage;/*! 用户登录页面*/

private:
    void initializeWidget();
    void readLoginInfo();
    void writeLoginInfo(bool judge);
    void handleHintInfo(QString& text);
    void handleNetError();
    void handleLoginInfo(InputPacket& inpack);
    void handleLoginSuccessInfo(InputPacket& inpack);
    void displayConcretePage(BasePage* page);
    void setInputMode(QLineEdit* line);
    void transferDataToAnotherPage(InputPacket& inpack, BasePage* basePage);
    void transferDataToAnotherPage(InputPacket& inpack, MainWindow* mainWindow);

private slots:
    void on_btnLogin_clicked();
    void on_btnRegister_clicked();
    void on_btnForgetPwd_clicked();
    void on_hintMsgPage(bool isJump);
    void on_deleteSubPages();
    void on_recvDataFromOtherPage(quint16 cmdCode, char* dataField, quint16 dataLen);
    void on_mainWindow(quint16 cmdCode, char* dataField, quint16 dataLen);
};

#endif // USERLOGINPAGE_H
