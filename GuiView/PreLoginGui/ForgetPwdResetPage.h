#ifndef FORGETPWDRESETPAGE_H
#define FORGETPWDRESETPAGE_H

class QLineEdit;
class QPushButton;
class NavigationBar;
class LoadingPage;
class HintMsgPage;
class QValidator;

#include "BasePage.h"

class ForgetPwdResetPage : public BasePage
{
    Q_OBJECT
public:
    explicit ForgetPwdResetPage(QWidget* parnet = 0);
    virtual ~ForgetPwdResetPage();

public:
    void fetchData(InputPacket& inpack);

private:
    NavigationBar* navigationBar;
    QLineEdit* editPassword;
    QLineEdit* editSubmitPassword;
    QPushButton* btnSubmit;
    QValidator* validator;

    LoadingPage* loadingPage;
    HintMsgPage* hintMsgPage;
    quint32 verifyCode;

private:
    void initializeWidget();
    void handleHintInfo(QString& strText);
    void handleNetError();
    void handleForgetPwdReset(InputPacket& inpack);
    void setInputMode(QLineEdit* line);

private slots:
    void on_btnPrePage_clicked();
    void on_btnSubmit_clicked();
    void on_hintMsgPage(bool isJump);
};

#endif // FORGETPWDRESETPAGE_H
