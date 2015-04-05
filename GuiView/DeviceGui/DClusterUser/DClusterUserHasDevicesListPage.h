#ifndef DCLUSTERUSERHASDEVICESLISTPAGE_H
#define DCLUSTERUSERHASDEVICESLISTPAGE_H

class GroupButton;
class GroupInfoList;
class GListWidget;
class GroupInfo;
class NavigationMenuBar;
class DClusterUserDetailPage;
class DClusterDevInfoList;
class DClusterUserAddDevicePage;
class DClusterUserDelDevicePage;
class DClusterAuthDevInfoList;
class DClusterAuthDevInfo;

#include "BasePage.h"

class DClusterUserHasDevicesListPage : public BasePage
{
    Q_OBJECT
public:
    explicit DClusterUserHasDevicesListPage(QWidget* parent = 0, quint64 clusterID = 0, quint64 userID = 0);
    virtual ~DClusterUserHasDevicesListPage();

public:
    void fetchData(InputPacket& inpack);

protected:
    void timerEvent(QTimerEvent* event);

private:
    int timerID;
    quint16 groupCount;
    quint64 curUserID;
    QString curUserName;
    quint64 curClusterID;
    int curFixedContentHeight;

    QAction* addDeviceAction;
    QAction* deleteDeviceAction;
    GroupButton* btnUserDetail;

    GListWidget* devListWidget;
    NavigationMenuBar* navigationMenuBar;
    DClusterAuthDevInfoList* authorizedDeviceInfoList;

    DClusterUserAddDevicePage* clusterUserAddDevicePage;
    DClusterUserDelDevicePage* clusterUserDelDevicePage;
    DClusterUserDetailPage* clusterUserDetailPage;

private:
    void initializeWidget();
    void loadLocalData();
    void fetchAuthorizedDeviceCmd();
    void setDeviceWidget(DClusterAuthDevInfo *devInfo, GListWidget* listWidget);
    void setGListWidget(GListWidget *dListWidget);
    void handleUserAuthorizedDeviceList(InputPacket& inpack);
    void recvDeviceInfoFromServer(InputPacket& inpack);
    void handleDelDeviceInfo(InputPacket& inpack);
    void delDeviceWidget(quint64 usrID);
    void delAuthorizedDeviceWidget(QList<quint64>& delList);
    void displayConcretePage(BasePage* page);
    void transferDataToAnotherPage(InputPacket& inpack, BasePage* basePage);

private slots:
    void on_addDeviceAction_clicked();
    void on_deleteDeviceAction_clicked();
    void on_btnPrepage_clicked();
    void on_btnUserDetail_clicked();
    void on_btnListWidgetFlex_clicked(int listHeight);
    void on_deleteSubPages();
    void on_recvDataFromOtherPage(quint16 cmdCode, char* data, quint16 dataLen);
};

#endif // DCLUSTERUSERHASDEVICESLISTPAGE_H
