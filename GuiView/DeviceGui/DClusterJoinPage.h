#ifndef DCLUSTERJOINPAGE_H
#define DCLUSTERJOINPAGE_H

class GroupButton;
class NavigationBar;
class LoadingPage;
class HintMsgPage;
class QPushButton;

#include "BasePage.h"
#include "DClusterInfo.h"

class DClusterJoinPage : public BasePage
{ 
    Q_OBJECT
public:
    explicit DClusterJoinPage(QWidget* parent , DClusterInfo& clusInfo);
    virtual ~DClusterJoinPage();

public:
    void fetchData(InputPacket& inpack);

private:
    NavigationBar* navigationBar;
    GroupButton* btnTitleInfo;
    GroupButton* btnDescribe;
    DClusterInfo curClusterInfo;

    QPushButton* btnJoinDevCluster;
    LoadingPage* loadingPage;
    HintMsgPage* hintMsgPage;

private:
    void initializeWidget();
    bool hasJoinedCluster();
    void setClusterInfo(DClusterInfo& info);

    void handleNetError();
    void handleHintInfo(QString& text);
    void handleJoinDevCluster(InputPacket& inpack);

private slots:
    void on_btnPrePage_clicked();
    void on_hintMsgPage(bool isJump);
    void on_btnJoinDevCluster_clicked();
    void on_backClusterManagementPage_clicked();
};

#endif // DCLUSTERJOINPAGE_H
