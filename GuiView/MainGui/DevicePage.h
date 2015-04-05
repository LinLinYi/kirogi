#ifndef DEVICEPAGE_H
#define DEVICEPAGE_H

class QLineEdit;
class QPushButton;
class QVBoxLayout;
class InputPacket;
class GListWidget;
class HintMsgButton;
class HintToolButton;
class DNotifySummary;
class NavigationBar;
class QToolButton;
class QLabel;
class ListWidget;
class MyDeviceInfo;
class DClusterInfo;
class GroupButton;
class MyDeviceInfoList;
class DClusterInfoList;
class DNotifySummaryList;
class OnlineStatusInfoList;
class DClusterJoinNotify;
class DeviceManagementPage;
class DClusterNotifyListPage;
class DClusterManagementPage;
class DClusterTransferNotifyListPage;

#include "BasePage.h"
#include "GlobalDefine.hpp"

using namespace std;
# pragma execution_character_set("utf-8")

class DevicePage : public BasePage
{
    Q_OBJECT
public:
    explicit DevicePage(QWidget *parent = 0, PageType type = MainPage);
    ~DevicePage();

public:
    void fetchData(InputPacket& inpack);

protected:
    void timerEvent( QTimerEvent *event );

private:
    int timerID;
    NavigationBar* navigationBar;
    int curFixedContentHeight;

    QLabel* lblInfoHint;
    HintToolButton* btnMessage;
    QToolButton* btnCluster;
    QToolButton* btnDevice;
    HintMsgButton* curHintMsgButton;

    ListWidget* deviceListWidget;
    ListWidget* messageListWidget;
    ListWidget* clusterListWidget;
    ListWidget* curListWidget;
    QVBoxLayout* widgetsLayout;

    DClusterTransferNotifyListPage* transferDeviceNotifyListPage;
    DClusterNotifyListPage* clusterNotifyListPage;
    DClusterManagementPage* clusterManagementPage;
    DeviceManagementPage* deviceManagementPage;

    DClusterInfoList* clusterInfoList;               /*! 设备群信息列表*/
    MyDeviceInfoList* myDeviceInfoList;              /*! 我的设备信息列表*/
    OnlineStatusInfoList* myDeviceOnlineList;        /*! 我的设备在线列表*/
    DNotifySummaryList* notifySummaryList;           /*! 通知信息列表*/

private:
    void initializeWidget();
    void loadLocalData();

//数据接收
    void recvMyDeviceListFromServer(InputPacket& inpack);
    void recvMyDeviceOnlineListFromServer(InputPacket& inpack);
    void recvClusterListFromServer(InputPacket& inpack);
    void recvCreatClusterResultFromServer(InputPacket& inpack);
    void recvClusterInfoFromServer(InputPacket& inpack);
    void recvDeviceInfoFromServer(InputPacket& inpack);
    void recvNotifyInfoFromServer(InputPacket inpack);
    void recvClusterJoinApprovalResult(InputPacket& inpack);
    void recvDeleteClusterResult(InputPacket& input);

//数据操作
    void handleClusterNotify(InputPacket& inpack);
    void handleClusterTransferDevicesNotify(InputPacket& inpack);
    void handleClusterApprovalNotify(InputPacket& inpack);
    void handleAddDeviceNotify(InputPacket& inpack);
    void handleDeleteDeviceNotify(InputPacket& inpack);
    void handleExitClusterNotify(InputPacket& inpack);
    void handleDeviceClaimNotify(InputPacket& inpack);
    void handleMyDeviceNetStatusNotify();

//数据配置
    void deployMsgNotifyList(QList<DNotifySummary*> list);
    void deployClusterInfoList(QHash<quint64, DClusterInfo*> list);
    void deployMyDeviceList(QHash<quint64, MyDeviceInfo*> list);

//信息加入到信息页面
    void appendNotifyToListWidget(ListWidget* listWidget, DNotifySummary* info);
    void appendClusterToListWidget(ListWidget* listWidget, DClusterInfo* info);
    void appendDeviceToListWidget(ListWidget* listWidget, MyDeviceInfo* info);

//控件参数及样式设置
    void setDeviceButton(GroupButton* btn, MyDeviceInfo* info);
    void setClusterButton(GroupButton* btn, DClusterInfo* info);
    void setToolButton(QToolButton *btn, QString imagePath);
    void setNotifyButton(HintMsgButton* btn, DNotifySummary* info);
    void setListWidget(ListWidget *listWidget);

//控件动态删除
    void deleteDeviceListWidgetItem(quint64 clusID);
    void deleteClusterListWidgetItem(quint64 clusID);

//列表信息查找
    DClusterInfo* findClusterInfo(quint64 clusID);
    DNotifySummary* findNotifyFromNotifyList(Mi::NotifyCategory type, quint64 notifyID);
    QWidget* findWidgetFromListWidget(ListWidget* listWidget, quint64 id);
    QWidget* findWidgetFromListWidget(ListWidget* listWidget, DNotifySummary* info);
    DNotifySummary* findNotifyInfo(Mi::NotifyCategory type, quint64 id);

//列表信息排序
    void sortDeviceNetStatus(ListWidget* listWidget, quint64 id, Mi::NetStatus status);
    void sortWithClusterRole(ListWidget* listWidget, GroupButton* gBtn);
    void sortWithNotifyTime(ListWidget* listWidget, HintMsgButton* hBtn);

//服务器信息获取
    void fetchDeviceInfoFromServer(quint64 devID);
    void fetchClusterInfoReq(quint64 clusID);

//其他
    void labelHintInfoDisplay();
    void createClusterManagementPage(quint64 clusID = 0);
    void listWidgetLastWidgetStyle(ListWidget *listWidget);
    void displayConcretePage(BasePage* page);
    void deleteMyDeviceFromListWidget(quint64 devID);
    void adjustListWidgets(ListWidget* listwidget);
    void adjustUnreadNotifyMessageCount(HintMsgButton *btn);
    void transferDataToAnotherPage(InputPacket& inpack, BasePage* basePage);

private slots:
    void on_toolBtnMessage_clicked();
    void on_toolBtnCluster_clicked();
    void on_toolBtnDevice_clicked();

    void on_btnAddDevices_clicked();
    void on_deleteSubPages();
    void on_btnClusterManagementPage_clicked();
    void on_btnClusterNotifyManagementPage_clicked();
    void on_btnClusterNotifyPage_clicked();
    void on_btnMyDeviceGroup_clicked();
    void on_btnTransferNotifyListPage_clicked();
    void on_btnDeleteNotify_clicked();
    void on_recvDataFromOtherPage(quint16 cmdCode, char* data, quint16 dataLen);
};

#endif // DEVICEPAGE_H
