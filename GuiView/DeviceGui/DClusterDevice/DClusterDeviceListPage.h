#ifndef DCLUSTERDEVICELISTPAGE_H
#define DCLUSTERDEVICELISTPAGE_H

class QVBoxLayout;
class ListWidget;
class DClusterInfo;
class GroupButton;
class DClusterDevInfo;
class OnlineStatusInfoList;
class DClusterDevInfoList;
class NavigationBar;
class QPushButton;
class OnlineStatusInfo;
class DClusterDeviceHasUserListPage;

#include "BasePage.h"

class DClusterDeviceListPage : public BasePage
{
    Q_OBJECT
public:
    explicit DClusterDeviceListPage(QWidget* parent = 0, quint64 clusterID = 0);
    virtual ~DClusterDeviceListPage();

public:
    void fetchData(InputPacket& inpack);

protected:
    void timerEvent(QTimerEvent* event);

private:
    NavigationBar* navigationBar;
    QPushButton* btnSearch;
    ListWidget* devListWidget;

    int timerID;
    quint32 curFixedContentHeight;
    quint64 curClusterID;

    DClusterDevInfoList* deviceInfoList;
    DClusterDeviceHasUserListPage* clusterDeviceHasUserListPage;

private:
    void initializeWidget();
    void setDeviceButton(GroupButton* btn, DClusterDevInfo* info);
    void handleDevClusterDeviceListInfo(InputPacket& inpack);
    void handleDevClusterDeviceOnlineList(InputPacket& inpack);
    void deployDeviceList(QHash<quint64, DClusterDevInfo*> list);
    void appendDeviceToListWidget(DClusterDevInfo* info);
    void changeDeviceOnlineStatus(OnlineStatusInfo *onlineInfo);
    void displayConcretePage(BasePage* page);
    void listWidgetLastWidgetStyle(ListWidget *listWidget);
    void transferDataToAnotherPage(InputPacket& inpack, BasePage* basePage);

private slots:
    void on_btnPrepage_clicked();
    void on_btnSearch_clicked();
    void on_btnDevClusterDeviceInfo_clicked();
    void on_deleteSubPages();
    void on_recvDataFromOtherPage(quint16 cmdCode, char* data, quint16 dataLen);
};

#endif // DCLUSTERDEVICELISTPAGE_H
