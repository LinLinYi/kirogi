#ifndef DCLUSTERDEVICEDELUSERPAGE_H
#define DCLUSTERDEVICEDELUSERPAGE_H

class NavigationBar;
class LoadingPage;
class HintMsgPage;
class GroupButton;
class UserAuthorizedInfo;
class UserAuthorizedInfoList;

#include "BasePage.h"

class DClusterDeviceDelUserPage : public BasePage
{
    Q_OBJECT
public:
    explicit DClusterDeviceDelUserPage(QWidget* parent = 0, quint64 clusterID = 0, quint64 deviceID = 0);
    virtual ~DClusterDeviceDelUserPage();

public:
    void fetchData(InputPacket& inpack);
    QList<quint64>& getDelUserList() { return userIdList; }

protected:
    void timerEvent(QTimerEvent* event);

private:
    int timerID;
    quint64 curClusterID;
    quint64 curDeviceID;
    quint16 curFixedContentHeight;
    quint16 userDelCount;
    NavigationBar* navigationBar;
    QVBoxLayout* userListLayout;

    LoadingPage* loadingPage;
    HintMsgPage* hintMsgPage;
    UserAuthorizedInfoList* userList;
    
    QList<quint64> userIdList;
    QHash<quint64, GroupButton*> usersBtnList;

private:
    void initializeWidget();
    void handleNetError();
    void handleHintInfo(QString& text);
    void handleDelUserList(InputPacket& inpack);
    void handleDelUserInfo(InputPacket& inpack);
    void handleAuthorizedUserList(InputPacket& inpack);
    void setUserButton(UserAuthorizedInfo* devInfo);

private slots:
   void on_hintMsgPage(bool isJump);
   void on_btnPrePage_clicked();
   void on_btnCancel_clicked();
   void on_btnDelUser_clicked();
   void on_btnUserButton_clicked();
};

#endif // DCLUSTERDEVICEDELUSERPAGE_H
