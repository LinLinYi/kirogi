#ifndef ACCOUNTSAFEPAGE_H
#define ACCOUNTSAFEPAGE_H

#include "BasePage.h"
#include "GroupButton.h"
#include "ListWidget.h"
#include "UserInfo.h"
#include "UserVerifyPasswordPage.h"
#include "UserAlterMailPage.h"

class AccountSafePage : public BasePage
{
    Q_OBJECT
public:
    explicit AccountSafePage(QWidget *parent = 0, quint64 accountID = 0);
    virtual ~AccountSafePage();

public:
    void fetchData(InputPacket& inpack);

private:
    quint64 curAccountID;
    NavigationBar* navigationBar;
    GroupButton* btnMacNum;
    GroupButton* btnMail;
    GroupButton* btnMacPwd;
    ListWidget* modifyListWidget;
    UserVerifyPasswordPage* userPwdVerifyPage;
    UserAlterMailPage* userMailModifyPage;
    UserInfo* userInfo;

private:
    void initializeWidget();
    void setListWidget(ListWidget *listWidget);
    void setGroupButton(GroupButton* groupButton, const QString& title);
    void displayConcretePage(BasePage* page);
    void transferDataToAnotherPage(InputPacket& inpack, BasePage* basePage);
    void transferDataToAnotherPage(InputPacket &inpack, UserVerifyPasswordPage* page);

private slots:
    void on_btnPrePage_clicked();
    void on_btnMail_clicked();
    void on_btnMacPwd_clicked();
    void on_deleteSubPages();
    void on_recvDataFromOtherPage(quint16 cmdCode, char* data, quint16 dataLen);
};

#endif // ACCOUNTSAFEPAGE_H
