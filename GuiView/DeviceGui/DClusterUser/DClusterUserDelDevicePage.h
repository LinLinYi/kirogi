#ifndef DCLUSTERUSERDELDEVICEPAGE_H
#define DCLUSTERUSERDELDEVICEPAGE_H

class GroupButton;
class NavigationBar;
class LoadingPage;
class HintMsgPage;
class DClusterAuthDevInfo;
class DClusterAuthDevInfoList;

#include "BasePage.h"

class DClusterUserDelDevicePage : public BasePage
{
    Q_OBJECT
public:
    explicit DClusterUserDelDevicePage(QWidget* parent = 0, quint64 clusterID = 0, quint64 userID = 0);
    virtual ~DClusterUserDelDevicePage();

public:
    void fetchData(InputPacket& inpack);
    void setUserID(quint64 usrID);
    QList<quint64>& getDelDevList() { return devIdList; }

protected:
    void timerEvent(QTimerEvent* event);

private:
    int timerID;
    quint64 curClusterID;
    quint64 curUserID;
    quint16 curFixedContentHeight;
    quint16 deviceDelCount;
    NavigationBar* navigationBar;
    QVBoxLayout* deviceListLayout;

    LoadingPage* loadingPage;
    HintMsgPage* hintMsgPage;
    DClusterAuthDevInfoList* deviceList;
    
    QList<quint64> devIdList;
    QHash<quint64, GroupButton*> devicesBtnList;

private:
    void initializeWidget();
    void handleNetError();
    void handleHintInfo(QString& text);
    void handleDelDeviceInfo(InputPacket& inpack);
    void handleAuthorizedDeviceList(InputPacket& inpack);
    void setDeviceButton(DClusterAuthDevInfo* devInfo);

private slots:
   void on_hintMsgPage(bool isJump);
   void on_btnPrePage_clicked();
   void on_btnCancel_clicked();
   void on_btnDelDevice_clicked();
   void on_btnDeviceButton_clicked();
};

#endif // DCLUSTERUSERDELDEVICEPAGE_H
