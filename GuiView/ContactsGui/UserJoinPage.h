#ifndef USERJOINPAGE_H
#define USERJOINPAGE_H

class QWidget;
class QPushButton;
class UserInfo;
class InputPacket;
class HintMsgPage;
class LoadingPage;
class GroupButton;
class NavigationBar;

#include "BasePage.h"

class UserJoinPage : public BasePage
{
    Q_OBJECT
public:
    explicit UserJoinPage(QWidget* parent = 0);
    virtual ~UserJoinPage();

public:
    void fetchData(InputPacket& inpack);

private:
    quint64 curUserID;
    UserInfo* curUserInfo;

    NavigationBar* navigationBar;
    GroupButton* btnTitleInfo;
    GroupButton* btnSignatrue;
    QPushButton* btnAddBuddy;
    
    HintMsgPage* hintMsgPage;
    LoadingPage* loadingPage;
    
private:
    void setPageData(UserInfo* info);
    void handleNetError();
    void handleHintInfo(QString& text);
    void handleSearchedUserInfo(InputPacket& inpack);
    void recvUserJoinApprovalResult(InputPacket& inpack);

private slots:
    void on_btnPrePage_clicked();
    void on_btnAddBuddy_clicked();
    void on_hintMsgPage(bool isJump);

private:
    void initializeWidget();
};

#endif // USERJOINPAGE_H
