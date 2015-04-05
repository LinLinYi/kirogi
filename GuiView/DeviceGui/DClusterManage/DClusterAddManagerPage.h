#ifndef DCLUSTERADDMANAGERPAGE_H
#define DCLUSTERADDMANAGERPAGE_H

class GroupButton;
class NavigationBar;
class LoadingPage;
class HintMsgPage;

#include "BasePage.h"
#include "DClusterUserInfoLIst.h"

class DClusterAddManagerPage : public BasePage
{
    Q_OBJECT
public:
    explicit DClusterAddManagerPage(QWidget* parent = 0, quint64 clusterID = 0);
    virtual ~DClusterAddManagerPage();

public:
    void fetchData(InputPacket& inpack);
    quint64 getManagerID() { return curManagerID; }

protected:
    void timerEvent(QTimerEvent* event);

private:
    int timerID;
    quint64 curClusterID;
    quint64 curManagerID;
    NavigationBar* navigationBar;
    QVBoxLayout* selectManagerLayout;
    DClusterUserInfoList userInfoList;
    QHash<quint64, GroupButton*> btnUserList;

    LoadingPage* loadingPage;
    HintMsgPage* hintMsgPage;

private:
    void initializeWidget();
    void handleNetError();
    void handleHintInfo(QString& text);
    void handleDevClusterUserList(InputPacket& inpack);
    void handleDevClusterSettingManager(InputPacket& inpack);

private slots:
    void on_hintMsgPage(bool isJump);
    void on_btnClosed_clicked();
    void on_btnSelectManager_clicked();
};

#endif // DCLUSTERADDMANAGERPAGE_H
