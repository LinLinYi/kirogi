#ifndef USERDETAILINFOPAGE_H
#define USERDETAILINFOPAGE_H

class QLabel;
class QPushButton;
class InputPacket;
class NavigationBar;

#include "BasePage.h"

class UserDetailInfoPage : public BasePage
{
    Q_OBJECT
public:
    explicit UserDetailInfoPage(QWidget* parent = 0);
    explicit UserDetailInfoPage(quint64 id, QString name, QWidget* parent = 0);
    virtual ~UserDetailInfoPage();

public:
    void fetchData(InputPacket& input);
    void setUserID(quint64 id) { userID = id; }
    void setUserName(QString name) { userName = name; }
    quint64 getUserID() { return userID; }
    QString getUserName() { return userName; }

private:
    void initializeWidget();

private:
    quint64 userID;
    QString userName;
    NavigationBar* navigationBar;

private slots:
    void on_btnPrePage_clicked();
};

#endif // USERDETAILINFOPAGE_H
