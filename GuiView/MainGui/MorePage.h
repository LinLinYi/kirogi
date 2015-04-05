#ifndef MOREPAGE_H
#define MOREPAGE_H

class QLabel;
class QLineEdit;
class QPushButton;
class InputPacket;
class GroupButton;
class UserSettingPage;
class PersonalInfoPage;
class ListWidget;
class NavigationBar;
class QToolButton;

#include "BasePage.h"
#include "UserInfo.h"

class MorePage : public BasePage
{
    Q_OBJECT
public:
    explicit MorePage(QWidget *parent = 0, PageType type = MainPage);
    ~MorePage();

public:
    void fetchData(InputPacket& inpack);

private:
    UserInfo curUserInfo;
    quint64 curAccountID;
    QString curAccountName;

    NavigationBar* navigationBar;
    GroupButton* btnSetting;
    QToolButton* btnPersonalInfo;

    ListWidget* settingListWidget;
    UserSettingPage* userSettingPage;
    PersonalInfoPage* personalInfoPage;

private:
    void initializeWidget();
    void loadLocalData(quint64 userID);
    void setListWidget(ListWidget *listWidget);
    void transferDataToAnotherPage(InputPacket& inpack, BasePage* basePage);
    void setGroupButton(GroupButton* btn, const QPixmap& photo, const QString &title);
    void displayConcretePage(BasePage* page);

private slots:
    void on_btnPersonal_clicked();
    void on_btnSetting_clicked();
    void on_deleteSubPages();
    void on_recvDataFromOtherPage(quint16 cmdCode, char* data, quint16 dataLen);
};
#endif // MOREPAGE_H
