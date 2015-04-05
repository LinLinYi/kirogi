#ifndef DCLUSTERTRANSFERDEVICEPAGE_H
#define DCLUSTERTRANSFERDEVICEPAGE_H

class QLabel;
class GroupButton;
class NavigationBar;
class LoadingPage;
class HintMsgPage;
class DClusterDevInfo;
class DClusterDevInfoList;

#include "BasePage.h"

class DClusterTransferDevicePage : public BasePage
{
    Q_OBJECT
public:
    explicit DClusterTransferDevicePage(QWidget* parent = 0, quint64 clusterID = 0);
    virtual ~DClusterTransferDevicePage();

public:
    void fetchData(InputPacket& inpack);
    void setTranClusterID(quint64 id);

protected:
    void timerEvent(QTimerEvent* event);

private:
    int timerID;
    quint64 curClusterID;
    quint64 tranClusterID;
    quint16 curFixedContentHeight;
    quint16 deviceTransferCount;
    NavigationBar* navigationBar;
    QVBoxLayout* deviceListLayout;

    QLabel* lblHint;
    LoadingPage* loadingPage;
    HintMsgPage* hintMsgPage;
    DClusterDevInfoList* deviceInfoList;
    QHash<quint64, GroupButton*> devicesBtnList;

private:
    void initializeWidget();
    void handleNetError();
    void handleHintInfo(QString& text);
    void handleTransferDeviceList(InputPacket& inpack);
    void handleTransferDeviceInfo(InputPacket& inpack);
    void setDeviceButton(DClusterDevInfo* devInfo);

private slots:
   void on_hintMsgPage(bool isJump);
   void on_btnPrePage_clicked();
   void on_btnCancel_clicked();
   void on_transferDevice_clicked();
   void on_btnDeviceButton_clicked();
};

#endif // DCLUSTERTRANSFERDEVICEPAGE_H
