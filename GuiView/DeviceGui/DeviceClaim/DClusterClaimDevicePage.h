#ifndef DCLUSTERCLAIMDEVICEPAGE_H
#define DCLUSTERCLAIMDEVICEPAGE_H

class DClusterDevInfo;
class GroupButton;
class NavigationBar;
class QPushButton;
class DClusterClaimDevicePwdPage;

#include "BasePage.h"

class DClusterClaimDevicePage : public BasePage
{
    Q_OBJECT
public:
    explicit DClusterClaimDevicePage(QWidget* parent = 0, quint64 clusID = 0, quint64 devID = 0);
    virtual ~DClusterClaimDevicePage();

public:
    void fetchData(InputPacket& inpack);
    void setClusterID(quint64 clusID);
    void setDeviceID(quint64 devID);

protected:
    void timerEvent(QTimerEvent* event);

private:
    int timerID;
    quint64 curClusterID;
    quint64 curDeviceID;

    NavigationBar* navigationBar;   /*! 导航栏*/
    GroupButton* btnTitleInfo;      /*! 设备标题信息*/

    GroupButton* btnMacAddr;        /*! 设备MAC地址*/
    GroupButton* btnDescribe;       /*! 设备描述*/
    QPushButton* btnClaimDevice;    /*! 设备认领*/
    DClusterDevInfo* deviceInfo;    /*! 设备信息*/
    DClusterClaimDevicePwdPage* clusterClaimDevicePwdPage;

private:
    void initializeWidget();
    void handleDeviceInfo(InputPacket& inpack);
    void displayConcretePage(BasePage* page);
    void transferDataToAnotherPage(InputPacket& inpack, BasePage* basePage);

private slots:
    void on_btnPrePage_clicked();
    void on_btnClaimDevice_clicked();
    void on_deleteSubPages();
    void on_recvDataFromOtherPage(quint16 cmdCode, char* data, quint16 dataLen);
};

#endif // DCLUSTERCLAIMDEVICEPAGE_H
