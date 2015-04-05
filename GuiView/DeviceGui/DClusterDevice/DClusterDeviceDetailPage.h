#ifndef DCLUSTERDEVICEDETAILPAGE_H
#define DCLUSTERDEVICEDETAILPAGE_H

class GroupButton;
class DClusterDevInfo;
class NavigationBar;
class DClusterDevAlterDescribePage;
class DClusterDevAlterRemarksPage;

#include "BasePage.h"

class DClusterDeviceDetailPage : public BasePage
{
    Q_OBJECT
public:
    explicit DClusterDeviceDetailPage(QWidget* parent, quint64 clusID, quint64 devID);
    virtual ~DClusterDeviceDetailPage();

public:
    void fetchData(InputPacket &inpack);

private:
    quint64 curClusterID;
    quint64 curDeviceID;

    NavigationBar* navigationBar;
    GroupButton* btnTitleInfo;
    GroupButton* btnMacAddr;
    GroupButton* btnDevRemarks;
    GroupButton* btnDevDescribe;
    DClusterDevInfo* curDeviceInfo;
    DClusterDevAlterRemarksPage* devAlterRemarksPage;
    DClusterDevAlterDescribePage* devAlterDescribePage;

private:
    void initializeWidget();
    void loadLocalData();
    void displayConcretePage(BasePage *page);
    void transferDataToAnotherPage(InputPacket& inpack, BasePage* page);

private slots:
    void on_btnPrePage_clicked();
    void on_btnRemarks_clicked();
    void on_btnDescribe_clicked();
    void on_deleteSubPages();
    void on_recvDataFromOtherPage(quint16 cmdCode, char* data, quint16 dataLen);
};

#endif // DCLUSTERDEVICEDETAILPAGE_H
