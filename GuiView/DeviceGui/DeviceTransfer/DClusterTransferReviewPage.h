#ifndef DCLUSTERTRANSFERREVIEWPAGE_H
#define DCLUSTERTRANSFERREVIEWPAGE_H

class QVBoxLayout;
class ListWidget;
class DClusterInfo;
class GroupButton;
class NavigationBar;
class QPushButton;
class HintMsgPage;
class LoadingPage;
class DeviceTransferInfo;
class DeviceTransferInfoList;

#include "BasePage.h"
#include "GlobalDefine.hpp"

class DClusterTransferReviewPage : public BasePage
{
    Q_OBJECT
public:
    explicit DClusterTransferReviewPage(QWidget* parent = 0, quint64 userID = 0, quint64 srcID = 0, quint64 destID = 0);
    virtual ~DClusterTransferReviewPage();

public:
    void fetchData(InputPacket& inpack);
    void setTransUserID(quint64 userID) { srcUserID = userID; }
    void setSrcClusterID(quint64 srcID) { srcClusterID = srcID; }
    void setDestClusterID(quint64 destID) { destClusterID = destID; }

private:
    quint64 srcUserID;
    quint64 srcClusterID;
    quint64 destClusterID;
    NavigationBar* navigationBar;
    QPushButton* btnSearch;
    ListWidget* devListWidget;

    quint64 curClusterID;
    Mi::MsgStatus curReviewStatus;
    quint32 curFixedContentHeight;
    DeviceTransferInfoList* deviceInfoList;

    HintMsgPage* hintMsgPage;
    LoadingPage* loadingPage;

private:
    void initializeWidget();
    void handleNetError();
    void handleHintInfo(QString& str);
    void handleApprovalResult(InputPacket& inpack);
    void deployDeviceList(QHash<quint64, DeviceTransferInfo*> list);
    void appendDeviceToListWidget(DeviceTransferInfo* info);
    void setDeviceButton(GroupButton* btn, DeviceTransferInfo* info);
    void displayConcretePage(BasePage* page);
    void listWidgetLastWidgetStyle(ListWidget *listWidget);
    void transferDataToAnotherPage(InputPacket& inpack, BasePage* basePage);

private slots:
    void on_btnAgree_clicked();
    void on_btnRefuse_clicked();
    void on_hintMsgPage(bool isJump);
    void on_recvDataFromOtherPage(quint16 cmdCode, char* data, quint16 dataLen);
};

#endif // DCLUSTERTRANSFERREVIEWPAGE_H
