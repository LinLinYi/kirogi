#ifndef USERREGISTERPAGE_H
#define USERREGISTERPAGE_H

#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QRegExpValidator>
#include "RegisterVerifyPage.h"

#include "InputPacket.h"
#include "BasePage.h"

//using namespace std;
//# pragma execution_character_set("utf-8")

/*! \class UserRegisterPage
 *  \brief 用户注册账号的类
 */
class UserRegisterPage : public BasePage
{
    Q_OBJECT
public:
    UserRegisterPage(QWidget* parent = 0);
    ~UserRegisterPage();

public:
    void fetchData(InputPacket& inpack);

private:
    NavigationBar* m_navigationBar;
    QLineEdit* m_pEditEmail;

    QPushButton* m_pBtnRegister;
    LoadingPage* loadingPage;
    HintMsgPage* hintMsgPage;
    RegisterVerifyPage* regValidationPage;

private:
    void initializeWidget();
    void handleHintInfo(QString& text);
    void handleNetError();
    void handleRegisterFirstStep(InputPacket& inpack);
    void displayConcretePage(BasePage* page);
    void transferDataToAnotherPage(InputPacket& inpack, BasePage* basePage);

private slots:
    void on_btnRegister_Clicked();
    void on_btnCancel_Clicked();
    void on_hintMsgPage(bool isJump);
    void on_deleteSubPages();
    void on_recvDataFromOtherPage(quint16 cmdCode, char* dataField, quint16 dataLen);
};

#endif // USERREGISTERPAGE_H

