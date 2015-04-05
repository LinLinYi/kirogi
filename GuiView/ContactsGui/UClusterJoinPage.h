#ifndef UCLUSTERJOINPAGE_H
#define UCLUSTERJOINPAGE_H

class GroupButton;
class NavigationBar;
class QPushButton;
class LoadingPage;
class HintMsgPage;

#include "BasePage.h"
#include "UClusterInfo.h"

class UClusterJoinPage : public BasePage
{
    Q_OBJECT
public:
    explicit UClusterJoinPage(QWidget* parent , UClusterInfo& clusInfo);
    virtual ~UClusterJoinPage();

public:
    void fetchData(InputPacket& inpack);

private:
    NavigationBar* navigationBar;
    GroupButton* btnTitleInfo;
    GroupButton* btnDescribe;
    UClusterInfo curClusterInfo;

    QPushButton* btnJoinDevCluster;
    LoadingPage* loadingPage;
    HintMsgPage* hintMsgPage;

private:
    void initializeWidget();
    bool hasJoinedCluster();
    void setClusterInfo(UClusterInfo& info);

    void handleNetError();
    void handleHintInfo(QString& text);
    void handleJoinDevCluster(InputPacket& inpack);

private slots:
    void on_btnPrePage_clicked();
    void on_hintMsgPage(bool isJump);
    void on_btnJoinDevCluster_clicked();
    void on_backClusterManagementPage_clicked();
};


#endif // UCLUSTERJOINPAGE_H
