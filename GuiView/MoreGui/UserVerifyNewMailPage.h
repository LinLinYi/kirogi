#ifndef USERVERIFYNEWMAILPAGE_H
#define USERVERIFYNEWMAILPAGE_H

class QLineEdit;
class QPushButton;
class InputPacket;
class NavigationBar;
class LoadingPage;
class HintMsgPage;
#include "BasePage.h"

class UserVerifyNewMailPage : public BasePage
{
    Q_OBJECT
public:
    explicit UserVerifyNewMailPage(QWidget* parent, QString& newMail);
    virtual ~UserVerifyNewMailPage();

public:
    void fetchData(InputPacket& inpack);

private:
    NavigationBar* navigationBar;
    QLineEdit* editValidate;
    QPushButton* btnSubmit;
    LoadingPage* loadingPage;
    HintMsgPage* hintMsgPage;
    QString newMailAddr;

private:
    void initializeWidget();
    void handleNetError();
    void handleHintInfo(QString &strText);
    void handleUserNewMailVerify(InputPacket& inpack);
    void transferDataToAnotherPage(InputPacket& inpack, BasePage* basePage);

private slots:
    void on_btnSubmit_clicked();
    void on_btnPrePage_clicked();
    void on_hintMsgPage(bool isJump);
};


#endif // USERVERIFYNEWMAILPAGE_H
