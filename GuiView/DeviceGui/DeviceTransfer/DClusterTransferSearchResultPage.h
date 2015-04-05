#ifndef DEVCLUSTERTRANSFERSEARCHRESULTPAGE_H
#define DEVCLUSTERTRANSFERSEARCHRESULTPAGE_H

class QPushButton;
class DClusterInfoList;
class GroupButton;
class NavigationBar;
class DClusterTransferDevicePage;

#include "BasePage.h"

class DClusterTransferSearchResultPage : public BasePage
{
    Q_OBJECT
public:
    explicit DClusterTransferSearchResultPage(QWidget* parent = 0, quint64 clusterID = 0);
    virtual ~DClusterTransferSearchResultPage();

public:
    void fetchData(InputPacket &inpack);

private:
    NavigationBar* navigationBar;   /*! 导航栏*/
    GroupButton* btnTitleInfo;      /*! 标题信息*/
    GroupButton* btnRemaks;         /*! 群备注*/
    GroupButton* btnDescribe;       /*! 群描述*/
    QPushButton* btnDeviceTransfer; /*! 设备移交*/
    DClusterTransferDevicePage* clusterTransferDevicePage;

    quint64 curClusterID;
    quint64 tranClusterID;
    DClusterInfoList* clusterInfoList;

private:
    void initializeWidget();
    void handleSearchClusterInfo(InputPacket& inpack);
    void displayConcretePage(BasePage* page);
    void transferDataToAnotherPage(InputPacket& inpack, BasePage* basePage);

private slots:
    void on_btnPrePage_clicked();
    void on_btnDeviceTransfer_clicked();
    void on_deleteSubPages();
    void on_recvDataFromOtherPage(quint16 cmdCode, char* data, quint16 dataLen);
};
#endif // DCLUSTERTRANSFERSEARCHRESULTPAGE_H
