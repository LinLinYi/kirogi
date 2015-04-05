#ifndef DCLUSTERUSERDETAILPAGE_H
#define DCLUSTERUSERDETAILPAGE_H

class NavigationBar;
class GroupButton;
class DClusterUserInfo;

#include "BasePage.h"

class DClusterUserDetailPage : public BasePage
{
    Q_OBJECT
public:
    explicit DClusterUserDetailPage(QWidget* parent , quint64 clusID, quint64 userID);
    virtual ~DClusterUserDetailPage();

public:
    void fetchData(InputPacket &inpack);

private:
    quint64 curUserID;
    quint64 curClusterID;
    GroupButton* btnTitleInfo;      /*! 标题信息*/
    GroupButton* btnDescribe;       /*! 用户描述*/
    NavigationBar* navigationBar;   /*! 导航栏*/
    DClusterUserInfo* userInfo;     /*! 用户信息*/

private:
    void initializeWidget();
    void loadLocalData();

private slots:
    void on_btnPrePage_clicked();
};

#endif // DCLUSTERUSERDETAILPAGE_H
