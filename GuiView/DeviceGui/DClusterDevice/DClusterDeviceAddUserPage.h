#ifndef DCLUSTERDEVICEADDUSERPAGE_H
#define DCLUSTERDEVICEADDUSERPAGE_H

class GroupButton;
class NavigationBar;
class LoadingPage;
class HintMsgPage;
class DClusterUserInfo;
class DClusterUserInfoList;

#include "BasePage.h"

class DClusterDeviceAddUserPage : public BasePage
{
    Q_OBJECT
public:
    explicit DClusterDeviceAddUserPage(QWidget* parent = 0, quint64 clusterID = 0, quint64 deviceID = 0);
    virtual ~DClusterDeviceAddUserPage();

public:
    void fetchData(InputPacket& inpack);
    QList<quint64>& getAddUsrList() { return usrIdList; }

protected:
    void timerEvent(QTimerEvent* event);

private:
    int timerID;
    quint64 curClusterID;
    quint64 curDeviceID;
    quint16 curFixedContentHeight;
    quint16 userAddCount;
    NavigationBar* navigationBar;
    QVBoxLayout* userListLayout;

    LoadingPage* loadingPage;
    HintMsgPage* hintMsgPage;

    QList<quint64> usrIdList;
    DClusterUserInfoList* userInfoList;
    QHash<quint64, GroupButton*> usersBtnList;

private:
    void initializeWidget();
    void handleNetError();
    void handleHintInfo(QString& text);
    void recvUserInfoFromServer(InputPacket& inpack);
    void handleDevClusterUserList(InputPacket& inpack);
    void handleAuthorizedUserList(InputPacket& inpack);
    void setUserButton(DClusterUserInfo* usrInfo);

private slots:
   void on_hintMsgPage(bool isJump);
   void on_btnPrePage_clicked();
   void on_btnCancel_clicked();
   void on_addUser_clicked();
   void on_btnUserButton_clicked();
};

#endif // DCLUSTERDEVICEADDUSERPAGE_H
