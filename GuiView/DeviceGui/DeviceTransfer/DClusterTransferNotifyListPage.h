#ifndef DCLUSTERTRANSFERNOTIFYLISTPAGE_H
#define DCLUSTERTRANSFERNOTIFYLISTPAGE_H

class NavigationBar;
class LoadingPage;
class HintMsgPage;
class CheckButton;
class ListWidget;
class DeviceTransNotify;
class DeviceTransNotifyList;
class DeviceTransferNotifyInfo;
class DClusterTransferReviewPage;

#include "BasePage.h"
#include "GlobalDefine.hpp"

class DClusterTransferNotifyListPage : public BasePage
{
    Q_OBJECT
public:
    explicit DClusterTransferNotifyListPage(QWidget* parent = 0);
    virtual ~DClusterTransferNotifyListPage();

public:
    void fetchData(InputPacket& inpack);

private:
    quint32 curFixedContentHeight;
    CheckButton* curCheckButton;
    NavigationBar* navigationBar;
    ListWidget* notifyListWidget;
    DClusterTransferReviewPage* transferReviewPage;
    DeviceTransNotifyList* notifyList;

private:
    void initializeWidget();
    void loadLocalDataSource();
    void recvNotifyInfoFromServer(InputPacket& inpack);
    void recvClusterCheckResultFromServer(InputPacket& inpack);
    void deployMsgNotifyList(QList<DeviceTransNotify*> list);
    void appendNotifyToListWidget(ListWidget* listWidget, DeviceTransNotify* info);
    void setNotifyButton(CheckButton* btn, DeviceTransNotify* info);
    void sortWithNotifyStatus(ListWidget* listWidget, CheckButton *hBtn);
    QWidget* findNotifyButtonFromListWidget(ListWidget* listwidget, DeviceTransNotify* info);
    DeviceTransNotify* findNotifyFromNotifyList(quint64 srcClusterID, quint64 destClusterID);

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

#endif // DCLUSTERTRANSFERNOTIFYLISTPAGE_H
