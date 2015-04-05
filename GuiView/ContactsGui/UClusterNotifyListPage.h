#ifndef UCLUSTERNOTIFYLISTPAGE_H
#define UCLUSTERNOTIFYLISTPAGE_H

class CheckButton;
class ListWidget;
class NavigationBar;
class UClusterReviewPage;
class UClusterJoinNotify;
class UClusterJoinNotifyList;

#include "BasePage.h"
#include "GlobalDefine.hpp"

class UClusterNotifyListPage : public BasePage
{
    Q_OBJECT
public:
    explicit UClusterNotifyListPage(QWidget* parent = 0);
    virtual ~UClusterNotifyListPage();

public:
    void fetchData(InputPacket& inpack);

private:
    quint32 curFixedContentHeight;
    CheckButton* curCheckButton;
    NavigationBar* navigationBar;
    ListWidget* notifyListWidget;
    UClusterReviewPage* clusterReviewPage;
    UClusterJoinNotifyList* notifyList;

private:
    void initializeWidget();
    void loadLocalDataSource();
    void recvClusterCheckResultFromServer(InputPacket& inpack);
    void recvNotifyInfoFromServer(InputPacket& inpack);
    void handleClusterApproval(quint16 cmdCode, char* data, quint16 dataLen);
    void deployMsgNotifyList(QList<UClusterJoinNotify*> list);
    void appendNotifyToListWidget(ListWidget* listWidget, UClusterJoinNotify *info);
    void setNotifyButton(CheckButton* btn, UClusterJoinNotify* info);
    void sortWithNotifyStatus(ListWidget* listWidget, CheckButton *hBtn);
    QWidget* findNotifyButtonFromListWidget(ListWidget *listwidget, UClusterJoinNotify *info);
    UClusterJoinNotify* findNotifyFromNotifyList(Mi::ClusterJoin type, quint64 userID, quint64 clusterID);

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


#endif // UCLUSTERNOTIFYLISTPAGE_H
