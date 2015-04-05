#ifndef DCLUSTERINVITEUSERPAGE_H
#define DCLUSTERINVITEUSERPAGE_H

class QPushButton;
class NavigationBar;
class LoadingPage;
class HintMsgPage;
class GroupButton;
class UserInfo;
class InputPacket;

#include "BasePage.h"

class DClusterInviteUserPage : public BasePage
{
    Q_OBJECT
public:
    explicit DClusterInviteUserPage(QWidget* parent, quint64 clusterID);
    virtual ~DClusterInviteUserPage();

public:
    void fetchData(InputPacket& inpack);

private:
    quint64 curUserID;
    quint64 curClusterID;
    bool isExistUserInfo;
    UserInfo* curUserInfo;

    NavigationBar* navigationBar;   /*! 导航栏*/
    GroupButton* btnTitleInfo;      /*! 标题信息*/
    GroupButton* btnSignatrue;      /*! 用户签名*/
    QPushButton* btnInviteMember;   /*! 邀请成员按钮*/
    LoadingPage* loadingPage;       /*! 信息加载页面*/
    HintMsgPage* hintMsgPage;       /*! 信息提示页面*/

private slots:
    void on_hintMsgPage(bool isJump);
    void on_btnPrePage_clicked();
    void on_btnInviteMember_clicked();

private:
    void initializeWidget();
    void handleNetError();
    void handleHintInfo(QString& text);
    void handleInviteUserInfo(InputPacket& inpack);
    void handleSearchUserinfo(InputPacket& inpack);
};

#endif // DCLUSTERINVITEUSERPAGE_H
