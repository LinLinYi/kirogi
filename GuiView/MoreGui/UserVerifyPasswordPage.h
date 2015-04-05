#ifndef USERVERIFYPASSWORDPAGE_H
#define USERVERIFYPASSWORDPAGE_H

#include <QString>
#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include "UserAlterPasswordPage.h"

/*! \class UserVerifyPasswordPage
 *  \brief 密码验证页面
 */
class UserVerifyPasswordPage: public QWidget
{
    Q_OBJECT
public:
    UserVerifyPasswordPage(QWidget* parent = 0, quint64 accountID = 0);
     ~UserVerifyPasswordPage();

public:
    void fetchData(InputPacket& inpack);

protected:
    void paintEvent(QPaintEvent*);

private:
    quint32 screenWidth;
    quint32 screenHeight;
    quint64 curAccountID;

    QLabel* lblTitle;
    QLabel* lblHintInfo;
    QPushButton* btnSubmit;
    QPushButton* btnCancel;
    QLineEdit* editCurAccountPwd;
    UserAlterPasswordPage* userPwdModifyPage;

private:
    void initializeWidget();
    void displayConcretePage(BasePage* page);
    void transferDataToAnotherPage(InputPacket& inpack, BasePage* page);

signals:
    void readyRead(quint16 cmdCode, char* data, quint16 dataLen);

private slots:
    void on_btnSubmit_clicked();
    void on_btnCancel_clicked();
    void on_deleteSubPages();
    void on_recvDataFromOtherPage(quint16 cmdCode, char* data, quint16 dataLen);
};

#endif // USERVERIFYPASSWORDPAGE_H
