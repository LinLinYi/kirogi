#ifndef DCLUSTERDEVICEHASUSERLISTPAGE_H
#define DCLUSTERDEVICEHASUSERLISTPAGE_H

class NavigationMenuBar;
class GroupInfoList;
class GroupButton;
class GListWidget;
class GroupInfo;
class QAction;
class UserAuthorizedInfo;
class DClusterDeviceAddUserPage;
class DClusterDeviceDelUserPage;
class DClusterDeviceDetailPage;
class UserAuthorizedInfoList;

#include "BasePage.h"

class DClusterDeviceHasUserListPage : public BasePage
{
    Q_OBJECT
public:
    explicit DClusterDeviceHasUserListPage(QWidget* parent = 0, quint64 clusterID = 0, quint64 deviceID = 0);
    virtual ~DClusterDeviceHasUserListPage();

public:
    void fetchData(InputPacket& inpack);
    void loadLocalData();

protected:
    void timerEvent(QTimerEvent* event);

private:
    int timerID;
    quint16 groupCount;
    quint64 curDeviceID;
    quint64 curClusterID;
    int curFixedContentHeight;

    QAction* addUserAction;
    QAction* deleteUserAction;
    GroupButton* btnDeviceDetail;

    GListWidget* userListWidget;
    NavigationMenuBar* navigationMenuBar;
    UserAuthorizedInfoList* authorizedUserInfoList;

    DClusterDeviceAddUserPage* clusterDeviceAddUserPage;
    DClusterDeviceDelUserPage* clusterDeviceDelUserPage;
    DClusterDeviceDetailPage* clusterDeviceDetailPage;

private:
    void initializeWidget();
    void sendFetchAuthorizedUserCmd();
    void setUserWidget(UserAuthorizedInfo *userInfo, GListWidget* listWidget);
    void setGListWidget(GListWidget *uListWidget);
    void handleDeviceAuthorizedUserList(InputPacket& inpack);
    void handleClusterAddUserInfo(InputPacket& inpack);
    void handleClusterDelUserInfo(InputPacket& inpack);
    void delUserWidget(quint64 usrID);
    void delAuthorizedUserWidget(QList<quint64>& delList);
    void displayConcretePage(BasePage* page);
    void transferDataToAnotherPage(InputPacket& inpack, BasePage* basePage);

private slots:
    void on_addUserAction_clicked();
    void on_deleteUserAction_clicked();
    void on_btnPrepage_clicked();
    void on_btnDeviceInfo_clicked();
    void on_deleteSubPages();
    void on_btnListWidgetFlex_clicked(int listHeight);
    void on_recvDataFromOtherPage(quint16 cmdCode, char* data, quint16 dataLen);
};

#endif // DCLUSTERDEVICEHASUSERLISTPAGE_H
