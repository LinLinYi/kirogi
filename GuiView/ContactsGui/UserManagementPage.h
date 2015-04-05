#ifndef USERMANAGEMENTPAGE_H
#define USERMANAGEMENTPAGE_H

#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
class QPushButton;
class QLineEdit;
class QLabel;
class InputPacket;
class GroupButton;
class UserJoinPage;
class SearchBar;
class UClusterSearchPage;
class UClusterCreatePage;
class LoadingPage;
class HintMsgPage;
class NavigationBar;

#include "BasePage.h"

class UserManagementPage : public BasePage
{
    Q_OBJECT

public:
    explicit UserManagementPage(QWidget *parent = 0);
    ~UserManagementPage();

public:
    void fetchData(InputPacket& inpack);

private:
    NavigationBar* navigationBar;       /*! 导航栏*/
    SearchBar* searchBar;               /*! 搜索栏*/
    GroupButton* btnSweep;              /*! 扫一扫按钮*/
    GroupButton* btnSearchCluster;      /*! 查找群组按钮*/
    GroupButton* btnCreateCluster;      /*! 创建群组按钮*/

    LoadingPage* loadingPage;
    HintMsgPage* hintMsgPage;
    UserJoinPage* userJoinPage;         /*! 添加好友页面*/
    UClusterSearchPage* clusterSearchPage;/*! 查找群页面*/
    UClusterCreatePage* clusterCreatePage;/*! 创建群页面*/


private:
    void initializeWidget();
    void handleNetError();
    void handleHintInfo(QString& text);
    void handleUserSearchResult(InputPacket& inpack);
    void displayConcretePage(BasePage* page);
    void transferDataToAnotherPage(InputPacket& inpack, BasePage* basePage);
    void setGroupButton(GroupButton* groupButton, QPixmap photo, QString title);

private slots:
    void on_btnPrePage_clicked();
    void on_searchBar(QString& searchValue);
    void on_btnSweep_clicked();
    void on_btnSearchCluster_clicked();
    void on_btnCreateCluster_clicked();
    void on_deleteSubPages();
    void on_hintMsgPage(bool isJump);
    void on_recvDataFromOtherPage(quint16 cmdCode, char* packData, quint16 dataLen);
};

#endif // USERMANAGEMENTPAGE_H
