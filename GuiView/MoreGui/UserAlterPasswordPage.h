#ifndef USERALTERPASSWORDPAGE_H
#define USERALTERPASSWORDPAGE_H

class QLabel;
class QLineEdit;
class HintMsgPage;
class LoadingPage;
class UserInfo;
class NavigationBar;
class QValidator;

#include "BasePage.h"

class UserAlterPasswordPage : public BasePage
{
    Q_OBJECT
public:
    explicit UserAlterPasswordPage(QWidget* parent = 0, quint64 accountID = 0);
    virtual ~UserAlterPasswordPage();

public:
    void fetchData(InputPacket &inpack);

private:
    quint64 curAccountID;
    UserInfo* userInfo;
    HintMsgPage* hintMsgPage;
    LoadingPage* loadingPage;

    NavigationBar* navigationBar;
    QLineEdit* editMacNum;
    QLineEdit* editMacPwd;
    QLineEdit* editAffirmPwd;
    QValidator *validator;

private:
    void initializeWidget();
    void handleNetError();
    void handleHintInfo(QString& strText);
    void handleModifyInfo(InputPacket& inpack);
    void setInputMode(QLineEdit* line);

private slots:
    void on_btnPrePage_clicked();
    void on_btnSubmit_clicked();
    void on_hintMsgPage(bool isJump);
};

#endif // USERALTERPASSWORDPAGE_H
