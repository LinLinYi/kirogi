#ifndef CONTACTPAGE_H
#define CONTACTPAGE_H

class QLabel;
class QLineedit;
class QPushButton;
class QVBoxLayout;
class QToolButton;
class HintToolButton;
class InputPacket;
class ListWidget;
class UserInfo;
class UserInfoList;
class SearchBar;
class GroupButton;
class UNotifySummary;
class UserInfoList;
class UClusterInfo;
class NavigationBar;
class UClusterInfoList;
class HintMsgButton;
class UNotifySummaryList;
class UserManagementPage;
class UserDetailInfoPage;
class UserJoinNotifyListPage;
class UClusterNotifyListPage;

#include <QList>
#include "BasePage.h"
#include "GlobalDefine.hpp"

class ContactPage : public BasePage
{
    Q_OBJECT
public:
    explicit ContactPage(QWidget *parent = 0, PageType type = MainPage);
    ~ContactPage();

public:
    void fetchData(InputPacket& inpack);

protected:
    void timerEvent( QTimerEvent *event );

private:
    int timerID;
    int curFixedContentHeight;

    NavigationBar* navigationBar;
    HintToolButton* btnMessage;
    HintMsgButton* curHintMsgButton;
    QToolButton* btnCluster;
    QToolButton* btnBuddy;
    QLabel* lblInfoHint;

    ListWidget* buddyListWidget;
    ListWidget* messageListWidget;
    ListWidget* clusterListWidget;
    ListWidget* curListWidget;
    QToolButton* curToolButton;
    QVBoxLayout* widgetsLayout;

    UserManagementPage* userManagementPage;
    UserDetailInfoPage* userDetailInfoPage;
    UserJoinNotifyListPage* userJoinNotifyListPage;
    UClusterNotifyListPage* clusterNotifyListPage;

    UserInfoList* userInfoList;
    UClusterInfoList* clusterInfoList;
    UNotifySummaryList* notifySummaryList;

private:
    void initializeWidget();
    void loadLocalData();

//数据接收
    void recvUserInfoListFromServer(InputPacket& inpack);
    void recvUserInfoFromServer(InputPacket& inpack);
    void recvClusterInfoFromServer(InputPacket& inpack);
    void recvNotifyInfoFromServer(InputPacket inpack);
    void recvClusterListFromServer(InputPacket& inpack);
    void recvUserJoinApprovalResult(InputPacket& inpack);
    void recvClusterJoinApprovalResult(InputPacket& inpack);
    void recvCreatClusterResultFromServer(InputPacket &inpack);
//数据操作
    void handleUserJoinNotify(InputPacket& inpack);
    void handleClusterNotify(InputPacket& inpack);

//数据配置
    void deployMsgNotifyList(QList<UNotifySummary *> list);
    void deployClusterInfoList(QHash<quint64, UClusterInfo *> list);
    void deployMyBuddyList(QHash<quint64, UserInfo*> list);

//信息加入到信息页面
    void appendUserToListWidget(ListWidget* listwidget, UserInfo *info);
    void appendClusterToListWidget(ListWidget* listwidget, UClusterInfo* info);
    void appendNotifyToListWidget(ListWidget* listwidget, UNotifySummary *info);

//控件参数及样式设置
    void setUserButton(GroupButton* btn, UserInfo* info);
    void setClusterButton(GroupButton* btn, UClusterInfo* info);
    void setToolButton(QToolButton* btn, QString imagePath);
    void setNotifyButton(HintMsgButton* btn, UNotifySummary* info);
    void setListWidget(ListWidget *listWidget);

//列表信息查找
    QWidget* findWidgetFromListWidget(ListWidget *listWidget, quint64 id);
    UClusterInfo* findClusterInfo(quint64 clusID);
    UNotifySummary* findNotifyInfo(Mi::NotifyCategory type, quint64 id);
    QWidget* findNotifyButtonFromListWidget(ListWidget* listwidget, UNotifySummary* info);

//列表信息排序
    void sortWithNotifyTime(ListWidget *listWidget, HintMsgButton *hBtn);
    void sortWithClusterRole(ListWidget* listWidget, GroupButton* gBtn);

//服务器信息获取
    void fetchUserInfoReq(quint64 userID);
    void fetchClusterInfoReq(quint64 clusID);

//其他
    void labelHintInfoDisplay();
    void displayConcretePage(BasePage* page);
    void listWidgetLastWidgetStyle(ListWidget *listWidget);
    void adjustListWidgets(ListWidget* listwidget);
    void adjustUnreadNotifyMessageCount(HintMsgButton* btn);
    void transferDataToAnotherPage(InputPacket& inpack, BasePage* basePage);

private slots:
    void on_btnAddContactsPage_clicked();
    void on_toolBtnMessage_clicked();
    void on_toolBtnCluster_clicked();
    void on_toolBtnBuddy_clicked();
    void on_deleteSubPages();
    void on_btnDeleteNotify_clicked();
    void on_btnUserJoinNotify_clicked();
    void on_btnClusterJoinNotify_clicked();
    void on_btnClusterManagmentPage_clicked();
    void on_recvDataFromOtherPage(quint16 cmdCode, char* data, quint16 dataLen);
};

#endif // CONTACTPAGE_H
