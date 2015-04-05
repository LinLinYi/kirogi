#ifndef DCLUSTERDELUSERPAGE_H
#define DCLUSTERDELUSERPAGE_H

class HintMsgPage;
class LoadingPage;
class NavigationBar;
class ListWidget;
class GroupButton;
class DClusterUserInfo;
class DClusterUserInfoList;

#include "BasePage.h"

class DClusterDelUserPage : public BasePage
{
    Q_OBJECT
public:
    explicit DClusterDelUserPage(QWidget* parent = 0, quint64 clusterID = 0);
    virtual ~DClusterDelUserPage();

public:
    void fetchData(InputPacket& inpack);
    QList<quint64> getDelUserList();

private:
    quint64 curClusterID;
    quint32 curFixedContentHeight;
    quint16 selectDelUserCount;
    QList<quint64> delUserIdList;

    HintMsgPage* hintMsgPage;
    LoadingPage* loadingPage;
    ListWidget* userListWidget;
    DClusterUserInfoList* userInfoList;
    NavigationBar* navigationBar;

private:
    void loadLocalData();
    void initialWidget();
    void handleNetError();
    void handleDelUserReply(InputPacket& inpack);
    void handleHintInfo(QString& text);
    void deployUserList(QHash<quint64, DClusterUserInfo*> list);
    void setUserButton(GroupButton* btn, DClusterUserInfo* info);

private slots:
    void on_hintMsgPage(bool isJump);
    void on_btnDeleteSubmit_clicked();
    void on_btnPrePage_clicked();
    void on_selectDelUser_clicked();
};

#endif // DCLUSTERDELUSERPAGE_H
