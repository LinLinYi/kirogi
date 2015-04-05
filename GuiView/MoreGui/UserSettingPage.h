#ifndef USERSETTINGPAGE_H
#define USERSETTINGPAGE_H

#include "BasePage.h"
#include "GroupButton.h"
#include "ListWidget.h"
#include "AccountSafePage.h"
#include "SoftwareFeedBackPage.h"
#include "SoftwareAboutPage.h"

class UserSettingPage : public BasePage
{
    Q_OBJECT
public:
    explicit UserSettingPage(QWidget *parent = 0, quint64 accountId = 0);
    virtual ~UserSettingPage();

public:
    void fetchData(InputPacket& inpack);

private:
    quint64 curAccountID;
    NavigationBar* navigationBar;
    GroupButton* btnAccountSafe;
    GroupButton* btnFeedback;
    GroupButton* btnAbout;
    QPushButton* btnExitLogin;

    ListWidget* generalListWidget;
    AccountSafePage* accountSafePage;
    SoftwareFeedBackPage* softwareFeedBackPage;
    SoftwareAboutPage* softwareAboutPage;

private:
    void initializeWidget();
    void setListWidget(ListWidget *listWidget);
    void setGroupButton(GroupButton* groupButton, const QString& title);
    void displayConcretePage(BasePage* page);
    void transferDataToAnotherPage(InputPacket& inpack, BasePage* basePage);

private slots:
    void on_btnPrePage_clicked();
    void on_btnAccountSafe_clicked();
    void on_btnFeedback_clicked();
    void on_btnAbout_clicked();
    void on_btnExitLogin_clicked();
    void on_deleteSubPages();
    void on_recvDataFromOtherPage(quint16 cmdCode, char* data, quint16 dataLen);
};

#endif // USERSETTINGPAGE_H
