#ifndef REGISTERVERIFYPAGE_H
#define REGISTERVERIFYPAGE_H

#include <QPushButton>
#include <QLabel>

#include "BasePage.h"
#include "RegAccountDeployPage.h"

class RegisterVerifyPage : public BasePage
{
    Q_OBJECT
public:
    explicit RegisterVerifyPage(QWidget* parent = 0);
    virtual ~RegisterVerifyPage();


public:
    void fetchData(InputPacket& inputData);

private:
    NavigationBar* navigationBar;//导航栏
    QLabel* lblMsgHint;          //验证码提示
    QLabel* lblAuthMail;         //验证邮箱
    QLineEdit* editAuthCode;     //验证码输入
    QPushButton* btnSubmit;      //提交按钮

    LoadingPage* loadingPage;    //加载页面
    HintMsgPage* hintMsgPage;    //消息提示页面
    RegAccountDeployPage* regAccountDeployPage;//账户设置页面

private:
    void initializeWidget();
    void handleHintInfo(QString& text);
    void handleNetError();
    void handleRegisterSecondStep(InputPacket& inpack);
    void displayConcretePage(BasePage* page);
    void transferDataToAnotherPage(InputPacket& inpack, BasePage* basePage);

private slots:
    void on_btnPrePage_clicked();
    void on_btnSubmit_clicked();
    void on_hintMsgPage(bool isJump);
    void on_deleteSubPages();
    void on_recvDataFromOtherPage(quint16 cmdCode, char* data, quint16 dataLen);
};

#endif // REGISTERVERIFYPAGE_H
