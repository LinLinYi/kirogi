#ifndef UCLUSTERSEARCHRESULTPAGE_H
#define UCLUSTERSEARCHRESULTPAGE_H

class QVBoxLayout;
class UClusterInfoList;
class GroupButton;
class UClusterJoinPage;
class NavigationBar;
class UClusterInfo;

#include "BasePage.h"

class UClusterSearchResultPage : public BasePage
{
    Q_OBJECT
public:
    explicit UClusterSearchResultPage(QWidget* parent = 0);
    virtual ~UClusterSearchResultPage();

public:
    void fetchData(InputPacket& inpack);

private:
    NavigationBar* navigationBar;
    QVBoxLayout* clusterListLayOut;
    quint16 curFixedContentHeight;
    UClusterInfoList* clusterInfoList;
    UClusterJoinPage* clusterJoinPage;

private:
    void initializeWidget();
    void setGroupButton(GroupButton* btn, UClusterInfo* baseInfo);
    void handleSearchClusterResult(InputPacket& inpack);
    UClusterInfo* findClusterInfoFromList(quint64 clusID);
    void displayConcretePage(BasePage* page);
    void transferDataToAnotherPage(InputPacket& inpack, BasePage* basePage);

private slots:
    void on_btnPrePage_clicked();
    void on_toolBtnCluster_clicked();
    void on_deleteSubPages();
    void on_recvDataFromOtherPage(quint16 cmdCode, char* data, quint16 dataLen);
};


#endif // UCLUSTERSEARCHRESULTPAGE_H
