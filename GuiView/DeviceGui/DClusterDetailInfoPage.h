#ifndef DCLUSTERDETAILINFOPAGE_H
#define DCLUSTERDETAILINFOPAGE_H

class GroupButton;
class NavigationBar;

#include "BasePage.h"
#include "DClusterInfo.h"

class DClusterDetailInfoPage : public BasePage
{
    Q_OBJECT
public:
    explicit DClusterDetailInfoPage(QWidget* parent = 0, quint64 clusID = 0);
    virtual ~DClusterDetailInfoPage();

public:
    void fetchData(InputPacket &inpack);

private:
    int timerID;
    quint64 curClusterID;

    GroupButton* btnTitleInfo;
    GroupButton* btnRemaks;
    GroupButton* btnDescribe;
    DClusterInfo* clusterInfo;
    NavigationBar* navigationBar;

private:
    void initializeWidget();
    void handleNetError();
    void loadLocalData();

private slots:
    void on_btnPrePage_clicked();
};

#endif // DCLUSTERDETAILINFOPAGE_H
