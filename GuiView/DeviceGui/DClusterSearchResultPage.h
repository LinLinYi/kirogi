#ifndef DCLUSTERSEARCHRESULTPAGE_H
#define DCLUSTERSEARCHRESULTPAGE_H

class QVBoxLayout;
class DClusterInfo;
class DClusterInfoList;
class GroupButton;
class NavigationBar;
class DClusterJoinPage;

#include "BasePage.h"

class DClusterSearchResultPage : public BasePage
{
    Q_OBJECT
public:
    explicit DClusterSearchResultPage(QWidget* parent = 0);
    virtual ~DClusterSearchResultPage();

public:
    void fetchData(InputPacket& inpack);

private:
    NavigationBar* navigationBar;
    QVBoxLayout* clusterListLayOut;
    quint16 curFixedContentHeight;
    DClusterInfoList* clusterInfoList;
    DClusterJoinPage* clusterJoinPage;

private:
    void initializeWidget();
    void setGroupButton(GroupButton* btn, DClusterInfo* baseInfo);
    void handleSearchClusterResult(InputPacket& inpack);
    DClusterInfo* findClusterInfoFromList(quint64 clusID);
    void displayConcretePage(BasePage* page);
    void transferDataToAnotherPage(InputPacket& inpack, BasePage* basePage);

private slots:
    void on_btnPrePage_clicked();
    void on_toolBtnCluster_clicked();
    void on_deleteSubPages();
    void on_recvDataFromOtherPage(quint16 cmdCode, char* data, quint16 dataLen);
};

#endif // DCLUSTERSEARCHRESULTPAGE_H
