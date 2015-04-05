#ifndef USERALTERMAILPAGE_H
#define USERALTERMAILPAGE_H

class QPushButton;
class QLineEdit;
class InputPacket;
class NavigationBar;
class LoadingPage;
class HintMsgPage;
class UserVerifyNewMailPage;
#include "BasePage.h"

class UserAlterMailPage : public BasePage
{
    Q_OBJECT
public:
    explicit UserAlterMailPage(QWidget* parent = 0);
    virtual ~UserAlterMailPage();

public:
    void fetchData(InputPacket& inpack);

private:
    NavigationBar* navigationBar;
    QLineEdit* editMail;
    QPushButton* btnApply;
    LoadingPage* loadingPage;
    HintMsgPage* hintMsgPage;
    UserVerifyNewMailPage* userNewMailVerifyPage;

private:
    void initializeWidget();
    void handleNetError();
    void handleHintInfo(QString& strText);
    void handleUserMailModifyInfo(InputPacket& inpack);
    void displayConcretePage(BasePage* page);
    void transferDataToAnotherPage(InputPacket& inpack, BasePage* basePage);

private slots:
    void on_btnPrePage_clicked();
    void on_btnApply_clicked();
    void on_hintMsgPage(bool);
    void on_deleteSubPages();
    void on_recvDataFromOtherPage(quint16 cmdCode, char* dataField, quint16 dataLen);
};

#endif // USERALTERMAILPAGE_H
