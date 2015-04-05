#ifndef DCLUSTERNOTIFYLISTPAGE_H
#define DCLUSTERNOTIFYLISTPAGE_H

class CheckButton;
class LIstWidget;
class NavigationBar;
class ListWidget;
class DClusterJoinNotify;
class DClusterReviewPage;
class DClusterJoinNotifyList;

#include "BasePage.h"
#include "GlobalDefine.hpp"

class DClusterNotifyListPage : public BasePage
{
    Q_OBJECT
public:
    explicit DClusterNotifyListPage(QWidget* parent = 0);
    virtual ~DClusterNotifyListPage();

public:
    void fetchData(InputPacket& inpack);

private:
    quint32 curFixedContentHeight;
    CheckButton* curCheckButton;
    NavigationBar* navigationBar;
    ListWidget* notifyListWidget;
    DClusterReviewPage* clusterReviewPage;
    DClusterJoinNotifyList* notifyList;

private:
    void initializeWidget();
    void loadLocalDataSource();
    void recvNotifyInfoFromServer(InputPacket& inpack);
    void recvClusterCheckResultFromServer(InputPacket& inpack);
    void deployMsgNotifyList(QList<DClusterJoinNotify*> list);
    void appendNotifyToListWidget(ListWidget* listWidget, DClusterJoinNotify* info);
    void setNotifyButton(CheckButton* btn, DClusterJoinNotify* info);
    void sortWithNotifyStatus(ListWidget* listWidget, CheckButton *hBtn);
    QWidget* findNotifyButtonFromListWidget(ListWidget* listwidget, DClusterJoinNotify* info);
    DClusterJoinNotify* findNotifyFromNotifyList(Mi::ClusterJoin type, quint64 userID, quint64 clusterID);

    void displayConcretePage(BasePage* page);
    void listWidgetLastWidgetStyle(ListWidget *listWidget);
    void transferDataToAnotherPage(InputPacket& inpack, BasePage* page);

private slots:
    void on_btnPrePage_clicked();
    void on_btnReviewPage_clicked();
    void on_btnReview_clicked();
    void on_deleteSubPages();
    void on_recvDataFromOtherPage(quint16 cmdCode, char* data, quint16 dataLen);
};

#endif // DCLUSTERNOTIFYPAGE_H
