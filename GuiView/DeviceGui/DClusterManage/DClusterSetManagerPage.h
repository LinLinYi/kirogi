#ifndef DCLUSTERSETMANAGERPAGE_H
#define DCLUSTERSETMANAGERPAGE_H

class QPushButton;
class CheckButton;
class NavigationBar;
class HintMsgPage;
class LoadingPage;
class DClusterUserInfo;
class DClusterUserInfoList;
class DClusterAddManagerPage;

#include "BasePage.h"

class DClusterSetManagerPage : public BasePage
{
    Q_OBJECT
public:
    explicit DClusterSetManagerPage(QWidget* parent = 0, quint64 clusterID = 0);
    virtual ~DClusterSetManagerPage();

public:
    void fetchData(InputPacket &inpack);

protected:
    void timerEvent(QTimerEvent*);

private:
    int timerID;
    quint64 curClusterID;
    quint64 curCancelManagerID;
    NavigationBar* navigationBar;
    QVBoxLayout* managerLayout;
    QPushButton* btnAddManager;
    DClusterUserInfoList* userInfoList;
    QHash<quint64, CheckButton*> btnCheckList;

    LoadingPage* loadingPage;
    HintMsgPage* hintMsgPage;
    DClusterAddManagerPage* clusterAddManagerPage;

private:
    void initializeWidget();
    void handleHintInfo(QString& text);
    void handleNetError();
    void appendManagerToList(DClusterUserInfo* info);
    void handleDevClusterUserListInfo(InputPacket& inpack);
    void handleDevClusterAuthorizedInfo(InputPacket& inpack);
    void displayConcretePage(BasePage* page);

private slots:
    void on_hintMsgPage(bool isJump);
    void on_btnComplete_clicked();
    void on_btnAddManager_clicked();
    void on_btnCancelManager_clicked();
    void on_deleteSubPages();
    void on_recvDataFromOtherPage(quint16 cmdCode, char* data, quint16 dataLen);
};

#endif // DCLUSTERSETMANAGERPAGE_H
