#ifndef USERALTERSIGNATUREPAGE_H
#define USERALTERSIGNATUREPAGE_H

class QTextEdit;
class HintMsgPage;
class LoadingPage;
class NavigationBar;

#include "BasePage.h"
#include "UserInfo.h"

class UserAlterSignaturePage : public BasePage
{
    Q_OBJECT
public:
    explicit UserAlterSignaturePage(QWidget* parent = 0, quint64 accountID = 0);
    virtual ~UserAlterSignaturePage();

public:
    void fetchData(InputPacket &inpack);

private:
    quint64 curAccountID;
    UserInfo curUserInfo;

    HintMsgPage* hintMsgPage;
    LoadingPage* loadingPage;
    NavigationBar* navigationBar;
    QTextEdit* editSignature;

private:
    void initializeWidget();
    void handleNetError();
    void loadLocalData();
    void handleSignatureInfo(InputPacket& inpack);
    void handleHintInfo(QString& strText);

private slots:
    void on_btnPrePage_clicked();
    void on_btnSubmitPage_clicked();
    void on_hintMsgPage(bool isJump);
};

#endif // USERALTERSIGNATUREPAGE_H
