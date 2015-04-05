#ifndef DCLUSTERUSERADDDEVICEPAGE_H
#define DCLUSTERUSERADDDEVICEPAGE_H

class NavigationBar;
class LoadingPage;
class HintMsgPage;
class GroupButton;
class DClusterDevInfo;
class DClusterDevInfoList;

#include "BasePage.h"


class DClusterUserAddDevicePage : public BasePage
{
    Q_OBJECT
public:
    explicit DClusterUserAddDevicePage(QWidget* parent = 0, quint64 clusterID = 0, quint64 userID = 0);
    virtual ~DClusterUserAddDevicePage();

public:
    void fetchData(InputPacket& inpack);
    QList<quint64>& getAddDevList() { return devIdList; }

protected:
    void timerEvent(QTimerEvent* event);

private:
    int timerID;
    quint64 curClusterID;
    quint64 curUserID;
    quint16 curFixedContentHeight;
    quint16 deviceAddCount;
    NavigationBar* navigationBar;
    QVBoxLayout* deviceListLayout;

    LoadingPage* loadingPage;
    HintMsgPage* hintMsgPage;

    QList<quint64> devIdList;
    DClusterDevInfoList* deviceList;
    QHash<quint64, GroupButton*> devicesBtnList;

private:
    void initializeWidget();
    void handleHintInfo(QString& text);
    void handleNetError();
    void recvDeviceInfoFromServer(InputPacket& inpack);
    void handleDevClusterDeviceList(InputPacket& inpack);
    void handleAuthorizedDeviceList(InputPacket& inpack);
    void setDeviceButton(DClusterDevInfo* devInfo);

private slots:
   void on_hintMsgPage(bool isJump);
   void on_btnPrePage_clicked();
   void on_btnCancel_clicked();
   void on_addDevice_clicked();
   void on_btnDeviceButton_clicked();
};
#endif // DCLUSTERUSERADDDEVICEPAGE_H
