#ifndef USERJOINNOTIFYLISTPAGE_H
#define USERJOINNOTIFYLISTPAGE_H

class CheckButton;
class ListWidget;
class UserReviewPage;
class NavigationBar;
class UserJoinNotify;
class UserJoinNotifyList;

#include "BasePage.h"

class UserJoinNotifyListPage : public BasePage
{
    Q_OBJECT
public:
    explicit UserJoinNotifyListPage(QWidget* parent = 0);
    virtual ~UserJoinNotifyListPage();

public:
    void fetchData(InputPacket& inpack);

private:
    quint32 curFixedContentHeight;
    CheckButton* curCheckButton;
    NavigationBar* navigationBar;
    ListWidget* notifyListWidget;

    UserReviewPage* userReviewPage;
    UserJoinNotifyList* notifyList;

private:
    void initializeWidget();
    void loadLocalDataSource();
//数据接收
    void recvUserCheckResultFromServer(InputPacket& inpack);
    void recvNotifyInfoFromServer(InputPacket& inpack);
//数据处理
    void handleUserApproval(InputPacket& inpack);
//显示配置
    void deployMsgNotifyList(QHash<quint64, UserJoinNotify*> list);
//信息加入到信息页面
    void appendNotifyToListWidget(ListWidget* listWidget, UserJoinNotify* info);
//控件参数及样式设置
    void setNotifyButton(CheckButton* btn, UserJoinNotify* info);
//列表信息排序
    void sortWithNotifyStatus(ListWidget* listWidget, CheckButton *htn);
//列表信息查找
    UserJoinNotify* findNotifyFromNotifyList(UserJoinNotifyList* infoList, quint64 userID);
    QWidget* findNotifyButtonFromListWidget(ListWidget *listwidget, quint64 userID);

//其他
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

#endif // USERJOINNOTIFYLISTPAGE_H
