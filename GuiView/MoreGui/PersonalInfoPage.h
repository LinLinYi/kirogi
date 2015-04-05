#ifndef PERSONALINFOPAGE_H
#define PERSONALINFOPAGE_H

#include "BasePage.h"
#include "UserInfo.h"
#include "GroupButton.h"
#include "UserAlterHeadPortraitPage.h"
#include "UserAlterNicknamePage.h"
#include "UserAlterSignaturePage.h"

class PersonalInfoPage : public BasePage
{
    Q_OBJECT
public:
    explicit PersonalInfoPage(QWidget* parent = 0, quint64 accountID = 0);
    virtual ~PersonalInfoPage();

public:
    void fetchData(InputPacket &inpack);

private:
    quint64 curAccountID;
    UserInfo curAccountInfo;
    NavigationBar* navigationBar;
    GroupButton* btnHeadPortrait;
    GroupButton* btnUserName;
    GroupButton* btnMacNum;
    GroupButton* btnSignatrue;

    UserAlterHeadPortraitPage* alterHeadPortraitPage;
    UserAlterNicknamePage* alterNicknamePage;
    UserAlterSignaturePage* alterSignaturePage;

private:
    void initializeWidget();
    void loadLocalData();
    QString cutOutString(const QString& str);
    void setGroupButton(GroupButton* btn, const QString& title, const quint16 height = 0);
    void displayConcretePage(BasePage* page);
    void transferDataToAnotherPage(InputPacket& inpack, BasePage* basePage);

private slots:
    void on_btnPrePage_clicked();
    void on_btnAlterHeadPortrait_clicked();
    void on_btnAlterNicknamePage_clicked();
    void on_btnAlterSignaturePage_clicked();
    void on_deleteSubPages();
    void on_recvDataFromOtherPage(quint16 cmdCode, char* data, quint16 dataLen);
};

#endif // PERSONALINFOPAGE_H
