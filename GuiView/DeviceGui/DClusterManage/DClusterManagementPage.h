#ifndef DCLUSTERMANAGEMENTPAGE_H
#define DCLUSTERMANAGEMENTPAGE_H

class QToolButton;
class GroupButton;
class DeviceInfoList;
class ListWidget;
class QPushButton;
class NavigationBar;
class LoadingPage;
class HintMsgPage;
class DClusterUserListPage;
class DClusterDeviceListPage;
class DClusterDeleteVerifyPage;
class DClusterSetManagerPage;
class DClusterClaimDeviceListPage;
class DClusterTransferSearchPage;
class DClusterAlterDescribePage;
class DClusterAlterRemarksPage;
class DClusterDetailInfoPage;

#include "BasePage.h"
#include "DClusterInfo.h"

class DClusterManagementPage : public BasePage
{
    Q_OBJECT
public:
    explicit DClusterManagementPage(QWidget* parent = 0, quint64 clusterID = 0);
    virtual ~DClusterManagementPage();

public:
    void fetchData(InputPacket& inpack);

private:
    NavigationBar* navigationBar;
    GroupButton* btnClusterInfo;
    GroupButton* btnClusterRemarks;
    GroupButton* btnClusterDescribe;
    GroupButton* btnClusterStatus;
    GroupButton* btnClusterSize;
    ListWidget* clusterDetailListWidget;

    GroupButton* btnDeviceClaim;
    GroupButton* btnDeviceTransfer;
    //ListWidget* clusterDevHandleListWidget;

    GroupButton* btnDevicesList;
    GroupButton* btnUsersList;
    ListWidget* clusterMemberListWidget;

    GroupButton* btnClusterAuthorize;
    QPushButton* btnExitCluster;

    int timerID;
    quint64 curClusterID;
    QString curClusterName;
    Mi::AuthLevel curUserPermission;
    DClusterInfo curClusterInfo;
    quint16 curSolidBodyHeight;

    LoadingPage* loadingPage;
    HintMsgPage* hintMsgPage;
    HintMsgPage* hintMsgExitCluster;
    DClusterUserListPage* clusterUserListPage;
    DClusterSetManagerPage* clusterSetManagerPage;
    DClusterDeleteVerifyPage* clusterDeleteVerifyPage;
    DClusterDeviceListPage* clusterDeviceListPage;
    DClusterClaimDeviceListPage* clusterClaimDeviceListPage;
    DClusterTransferSearchPage* clusterTransferSearchPage;
    DClusterAlterDescribePage* clusterAlterDescribePage;
    DClusterAlterRemarksPage* clusterAlterRemarksPage;
    DClusterDetailInfoPage* clusterDetailInfoPage;

private:
    void initializeWidget();
    void handleNetError();
    void loadLocalData();
    void handleHintInfo(QString& text);
    void handleExitDevCluster(InputPacket& inpack);
    void handleDeleteDevCluster(InputPacket& inpack);
    void setListWidget(ListWidget* listWidget);
    void setGroupButton(GroupButton* gButton, QString title, QString titleDetail = "");
    void transferDataToAnotherPage(InputPacket& inpack, BasePage* basePage);

    void revDataFromAlterDescribe();
    void revDataFromAlterRemarks();

    bool isPermission();
    QString cutOutString(const QString &str);
    void displayConcretePage(BasePage* page);

private slots:
    void on_hintMsgPage(bool isJump);
    void on_btnPrePage_clicked();
    void on_btnClusterDetail_clicked();
    void on_btnClusterRemarks_clicked();
    void on_btnClusterDescribe_clicked();
    void on_btnClusterStatus_clicked();
    void on_btnClusterSize_clicked();
    void on_btnDeviceClaim_clicked();
    void on_btnDeviceTransfer_clicked();
    void on_btnDevicesList_clicked();
    void on_btnUsersList_clicked();
    void on_btnClusterAuthorize_clicked();
    void on_btnExitCluster_clicked();
    void on_hintMsgExitCluster_Submit_clicked(bool isJump);
    void on_hintMsgExitCluster_Cancel_clicked();
    void on_deleteSubPages();
    void on_recvDataFromOtherPage(quint16 cmdCode, char* data, quint16 dataLen);
};

#endif // DEVCLUSTERMANAGEMENTPAGE_H
