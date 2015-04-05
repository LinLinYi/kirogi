#ifndef FORGETPWDPAGE_H
#define FORGETPWDPAGE_H

#include <QPushButton>
#include <QLineEdit>
#include <qglobal.h>
#include "BasePage.h"
#include "InputPacket.h"
#include "ForgetPwdVerifyPage.h"

#if defined Q_OS_WIN32 || defined Q_OS_WIN64 || defined Q_OS_WIN
    using namespace std;
    # pragma execution_character_set("utf-8")
#endif

/*! \class ForgetPwdPage
*   \brief 忘记密码页面的类
*/
class ForgetPwdPage : public BasePage
{
    Q_OBJECT
public:
    explicit ForgetPwdPage(QWidget* parent = 0);
    virtual ~ForgetPwdPage();

public:
    void fetchData(InputPacket& inpack);

private:
    NavigationBar* navigationBar;
    QLineEdit* editMail;
    QPushButton* btnApply;
    LoadingPage* loadingPage;
    HintMsgPage* hintMsgPage;
    ForgetPwdVerifyPage* forgetPwdValidatePage;

private:
    void initializeWidget();
    void handleHintInfo(QString& strText);
    void handleNetError();
    void handleForgetPwdApply(InputPacket& inpack);
    void displayConcretePage(BasePage* page);
    void transferDataToAnotherPage(InputPacket& inpack, BasePage* basePage);

private slots:
    void on_btnPrePage_clicked();
    void on_btnApply_clicked();
    void on_hintMsgPage(bool);
    void on_deleteSubPages();
    void on_recvDataFromOtherPage(quint16 cmdCode, char* dataField, quint16 dataLen);
};

#endif // FORGETPWDPAGE_H
