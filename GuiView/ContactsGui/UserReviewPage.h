#ifndef USERREVIEWPAGE_H
#define USERREVIEWPAGE_H

class GroupButton;
class UserJoinNotify;
class NavigationBar;
class QLabel;
class QPushButton;

#include "BasePage.h"
#include "GlobalDefine.hpp"
#include "UserJoinNotify.h"

class UserReviewPage : public BasePage
{
    Q_OBJECT
public:
    explicit UserReviewPage(QWidget *parent, UserJoinNotify& info);
    virtual ~UserReviewPage();

public:
    void fetchData(InputPacket& inpack);

private:
    Mi::ClusterJoin curCheckType;
    Mi::MsgStatus curCheckStatus;
    UserJoinNotify curNotifyInfo;

    NavigationBar* navigationBar;
    QLabel* lblCheckHint;
    GroupButton* btnSender;
    QPushButton* btnAgree;
    QPushButton* btnRefuse;

private:
    void initializeWidget();
    void sendCheckedStatus(quint8 ansCode);
    void hasCheckedDisplay(Mi::MsgStatus status);

private slots:
    void on_btnPrePage_clicked();
    void on_btnSender_clicked();
    void on_btnAgree_clicked();
    void on_btnRefuse_clicked();
};


#endif // USERREVIEWPAGE_H
