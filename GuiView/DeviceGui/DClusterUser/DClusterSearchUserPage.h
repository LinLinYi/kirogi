#ifndef DCLUSTERSEARCHUSERPAGE_H
#define DCLUSTERSEARCHUSERPAGE_H

class InputPacket;
class SearchBar;
class HintMsgPage;
class LoadingPage;
class NavigationBar;
class DClusterInviteUserPage;

#include "BasePage.h"

class DClusterSearchUserPage : public BasePage
{
    Q_OBJECT
public:
    explicit DClusterSearchUserPage(QWidget* parent = 0, quint64 clusterID = 0);
    virtual ~DClusterSearchUserPage();

public:
    void fetchData(InputPacket& inputpack);

private:
    quint64 curClusterID;               /*! 设备群ID*/
    SearchBar* searchBar;               /*! 查找设备群*/
    LoadingPage* loadingPage;           /*! 加载页面*/
    HintMsgPage* hintMsgpage;           /*! 信息提示页面*/
    NavigationBar* navigationBar;       /*! 导航栏*/
    DClusterInviteUserPage* inviteUserPage;/*! 邀请用户页面*/

private:
    void initializeWidget();
    void handleNetError();
    void handleHintInfo(QString& text);
    void handleSearchUserResult(InputPacket& inpack);
    void displayConcretePage(BasePage* page);
    void transferDataToAnotherPage(InputPacket& inpack, BasePage* basePage);

private slots:
    void on_btnPrePage_clicked();
    void on_searchBar(QString& text);
    void on_hintMsgpage(bool isJump);
    void on_deleteSubPages();
    void on_recvDataFromOtherPage(quint16 cmdCode, char* data, quint16 dataLen);
};

#endif // DCLUSTERSEARCHUSERPAGE_H
