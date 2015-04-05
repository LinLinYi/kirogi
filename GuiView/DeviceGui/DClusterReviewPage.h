#ifndef DCLUSTERREVIEWPAGE_H
#define DCLUSTERREVIEWPAGE_H

class QLabel;
class GroupButton;
class GlobalDefine;
class NavigationBar;
class GlobalDefine;
class QPushButton;
class DClusterJoinNotify;

#include "BasePage.h"
#include "GlobalDefine.hpp"
#include "DClusterJoinNotify.h"

class DClusterReviewPage : public BasePage
{
    Q_OBJECT
public:
    explicit DClusterReviewPage(QWidget *parent, DClusterJoinNotify& info);
    virtual ~DClusterReviewPage();

public:
    void fetchData(InputPacket& inpack);

private:
    Mi::ClusterJoin curCheckType;
    Mi::MsgStatus curCheckStatus;
    DClusterJoinNotify curNotifyInfo;

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

#endif // DCLUSTERREVIEWPAGE_H
