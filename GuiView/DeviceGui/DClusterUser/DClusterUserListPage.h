#ifndef DEVCLUSTERUSERLISTPAGE_H
#define DEVCLUSTERUSERLISTPAGE_H

class QVBoxLayout;
class NavigationMenuBar;
class ListWidget;
class QPushButton;
class DClusterInfo;
class LoadingPage;
class HintMsgPage;
class GroupButton;
class DClusterUserInfo;
class OnlineStatusInfoList;
class DClusterUserInfoList;
class DClusterSearchUserPage;
class DClusterDelUserPage;
class DClusterUserHasDevicesListPage;

#include "BasePage.h"

class DClusterUserListPage : public BasePage
{
    Q_OBJECT
public:
    explicit DClusterUserListPage(QWidget* parent = 0, quint64 clusterID = 0);
    virtual ~DClusterUserListPage();

public:
    void fetchData(InputPacket& inpack);

protected:
    void timerEvent(QTimerEvent* event);

private:
    int timerID;
    QVBoxLayout* vbTopLayout;
    QPushButton* btnSearch;
    ListWidget* userListWidget;
    NavigationMenuBar* navigationMenuBar;

    QAction* addUserAction;
    QAction* deleteUserAction;

    quint64 curClusterID;
    quint16 deleteUserCount;
    quint32 curFixedContentHeight;
    DClusterInfo* clusterInfo;
    DClusterUserInfoList* userInfoList;

    LoadingPage* loadingPage;
    HintMsgPage* hintMsgPage;
    DClusterDelUserPage* clusterdelUserPage;
    DClusterSearchUserPage* clusterSearchUserPage;
    DClusterUserHasDevicesListPage* clusterUserHasDevicesListPage;

private:
    void initializeWidget();
    void loadLocalData();
    void handleHintInfo(QString& text);
    void handleNetError();
    void setUserButton(GroupButton* btn, DClusterUserInfo* info);
    void appendUserToListWidget(DClusterUserInfo* info);
    void deployUserList(QHash<quint64, DClusterUserInfo*> list);
    void handleClusterDelUserListInfo(InputPacket& inpack);
    void handleClusterUserListInfo(InputPacket& inpack);
    void displayConcretePage(BasePage* page);
    void sortWithUserRole(ListWidget* listWidget, GroupButton* gBtn);
    void listWidgetLastWidgetStyle(ListWidget* listWidget);
    void transferDataToOhterPage(InputPacket& inpack, BasePage* basePage);

private slots:
    void on_btnSearch_clicked();
    void on_btnPrepage_clicked();
    void on_hintMsgPage(bool isJump);
    void on_addUserAction_clicked();
    void on_delUserAction_clicked();
    void on_deleteSubPages();
    void on_btnDevClusterUserHasDevicesListPage_clicked();
    void on_recvDataFromOtherPage(quint16 cmdCode, char* data, quint16 dataLen);
};

#endif // DCLUSTERUSERLISTPAGE_H
