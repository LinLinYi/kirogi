#ifndef USERALTERNICKNAMEPAGE_H
#define USERALTERNICKNAMEPAGE_H

class QLineEdit;
class LoadingPage;
class HintMsgPage;
class NavigationBar;

#include "BasePage.h"
#include "UserInfo.h"

class UserAlterNicknamePage : public BasePage
{
    Q_OBJECT
public:
    explicit UserAlterNicknamePage(QWidget* parent = 0, quint64 accountID = 0);
    virtual ~UserAlterNicknamePage();

public:
    void fetchData(InputPacket &inpack);

private:
    quint64 curAccountID;
    UserInfo curUserInfo;
    HintMsgPage* hintMsgPage;
    LoadingPage* loadingPage;
    NavigationBar* navigationBar;
    QLineEdit* editNickname;

private:
    void initializeWidget();
    void handleNetError();
    void loadLocalData(quint64 userID);
    void handleNickNameInfo(InputPacket& inpack);
    void handleHintInfo(QString& strText);

private slots:
    void on_btnPrePage_clicked();
    void on_btnSubmitPage_clicked();
    void on_hintMsgPage(bool isJump);
};


#endif // USERALTERNICKNAMEPAGE_H
