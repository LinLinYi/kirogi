#ifndef FORGETPWDVERIFYPAGE_H
#define FORGETPWDVERIFYPAGE_H

#include <QLineEdit>
#include <QPushButton>
#include "InputPacket.h"
#include "BasePage.h"
#include "ForgetPwdResetPage.h"

/*! \class ForgetPwdVerifyPage
*   \brief 忘记密码:服务器申请通过后，输入验证码的页面
*/
class ForgetPwdVerifyPage : public BasePage
{
    Q_OBJECT
public:
    explicit ForgetPwdVerifyPage(QWidget* parent = 0);
    virtual ~ForgetPwdVerifyPage();

public:
    void fetchData(InputPacket& inpack);

private:
    NavigationBar* navigationBar;
    QLineEdit* editValidate;
    QPushButton* btnNextStep;
    quint32 verifyCode;
    LoadingPage* loadingPage;
    HintMsgPage* hintMsgPage;
    ForgetPwdResetPage* forgetPwdResetPage;

private:
    void initializeWidget();
    void handleNetError();
    void handleHintInfo(QString &strText);
    void handleForgetPwdVerify(InputPacket& inpack);
    void displayConcretePage(BasePage* page);
    void transferDataToAnotherPage(InputPacket& inpack, BasePage* basePage);

private slots:
    void on_btnNextStep_clicked();
    void on_btnPrePage_clicked();
    void on_hintMsgPage(bool isJump);
    void on_deleteSubpage();
    void on_recvDataFromOtherPage(quint16 cmdCode, char* dataField, quint16 dataLen);
};

#endif // FORGETPWDVERIFYPAGE_H
