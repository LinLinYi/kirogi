#ifndef UCLUSTERREVIEWPAGE_H
#define UCLUSTERREVIEWPAGE_H

class GroupButton;
class NavigationBar;
class QPushButton;
class QLabel;

#include "BasePage.h"
#include "GlobalDefine.hpp"
#include "UClusterJoinNotify.h"

class UClusterReviewPage : public BasePage
{
    Q_OBJECT
public:
    explicit UClusterReviewPage(QWidget *parent, UClusterJoinNotify& info);
    virtual ~UClusterReviewPage();

public:
    void fetchData(InputPacket& inpack);

private:
    Mi::ClusterJoin curCheckType;
    Mi::MsgStatus curCheckStatus;
    UClusterJoinNotify curNotifyInfo;

    NavigationBar* navigationBar;
    QLabel* lblCheckHint;
    GroupButton* btnSender;
    QPushButton* btnAgree;
    QPushButton* btnRefuse;

private:
    void initializeWidget();
    void sendCheckedStatus(quint8 ansCode);
    void hasCheckedDisplay(Mi::MsgStatus status, Mi::ClusterJoin type);

private slots:
    void on_btnPrePage_clicked();
    void on_btnSender_clicked();
    void on_btnAgree_clicked();
    void on_btnRefuse_clicked();
};

#endif // UCLUSTERREVIEWPAGE_H
