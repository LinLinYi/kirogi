#ifndef DCLUSTERCLAIMDEVICELISTPAGE_H
#define DCLUSTERCLAIMDEVICELISTPAGE_H

class CheckButton;
class NavigationBar;
class DClusterDevInfo;
class DClusterDevInfoList;
class DClusterClaimDevicePage;
class DClusterClaimDevicePwdPage;

#include "BasePage.h"

class DClusterClaimDeviceListPage : public BasePage
{
    Q_OBJECT
public:
    explicit DClusterClaimDeviceListPage(QWidget* parent = 0, quint64 clusID = 0);
    virtual ~DClusterClaimDeviceListPage();

public:
    void initializeWidget();
    void setClusterID(quint64 clusID);
    void fetchData(InputPacket& inpack);

protected:
    void timerEvent(QTimerEvent* event);

private:
    int timerID;
    quint64 curClusterID;
    quint8 curReviewStatus;
    CheckButton* curCheckButton;
    NavigationBar* navigationBar;
    QVBoxLayout* devClaimListLayout;
    DClusterDevInfoList* claimDeviceList;
    DClusterClaimDevicePage* clusterClaimDevicePage;
    DClusterClaimDevicePwdPage* clusterClaimDevicePwdPage;

private:
    void test();
    void setDeviceClaimWidget(DClusterDevInfo* info);
    void handleDeviceClaim(InputPacket& inpack);
    void handleDeviceClaimList(InputPacket& inpack);
    void displayConcretePage(BasePage* page);
    void transferDataToAnotherPage(InputPacket& inpack, BasePage* basePage);

private slots:
    void on_btnPrePage_clicked();
    void on_btnClaimPage_clicked();
    void on_btnClaim_clicked();
    void on_deleteSubPages();
    void on_recvDataFromOtherPage(quint16 cmdCode, char* data, quint16 dataLen);
};

#endif // DCLUSTERCLAIMDEVICELISTPAGE_H
