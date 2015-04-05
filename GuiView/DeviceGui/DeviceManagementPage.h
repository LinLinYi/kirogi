#ifndef DEVICEMANAGEMENTPAGE_H
#define DEVICEMANAGEMENTPAGE_H

class QPushButton;
class QLineEdit;
class QLabel;
class InputPacket;
class GroupButton;
class SearchBar;
class NavigationBar;
class DClusterSearchPage;
class DClusterSearchResultPage;
class DClusterCreatePage;

#include "BasePage.h"

class DeviceManagementPage : public BasePage
{
    Q_OBJECT

public:
    explicit DeviceManagementPage(QWidget *parent = 0);
    virtual ~DeviceManagementPage();

public:
    void fetchData(InputPacket& inpack);

private:
    NavigationBar* navigationBar;
    SearchBar* searchBar;
    GroupButton* btnSweep;
    GroupButton* btnSearchCluster;
    GroupButton* btnCreateCluster;

    DClusterSearchPage* clusterSearchPage;
    DClusterCreatePage* clusterCreatePage;

private:
    void initializeWidget();
    void transferDataToAnotherPage(InputPacket& inpack, BasePage* basePage);
    void setGroupButton(GroupButton* groupButton, QPixmap photo, QString title);
    void displayConcretePage(BasePage* page);

private slots:
    void on_btnPrePage_clicked();
    void on_searchBar(QString& searchValue);
    void on_btnSweep_clicked();
    void on_searchCluster_clicked();
    void on_createCluster_clicked();
    void on_deleteSubPages();
    void on_recvDataFromOtherPage(quint16 cmdCode, char* data, quint16 dataLen);
};

#endif // DEVICEMANAGEMENTPAGE_H
