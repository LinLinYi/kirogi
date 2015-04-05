#ifndef DCLUSTERCREATESUCCESSEDPAGE_H
#define DCLUSTERCREATESUCCESSEDPAGE_H

class GroupButton;
class InputPacket;
class DClusterInfo;
class NavigationBar;
class QPushButton;

#include "BasePage.h"

class DClusterCreateSuccessedPage : public BasePage
{
    Q_OBJECT
public:
    explicit DClusterCreateSuccessedPage(QWidget* parent = 0, quint64 clusID = 0);
    virtual ~DClusterCreateSuccessedPage();

public:
    void fetchData(InputPacket& inpack);

private:
    quint64 curClusterID;
    NavigationBar* navigationBar;       /*! 导航栏*/
    GroupButton* btnTitleInfo;          /*! 群标题信息*/
    GroupButton* btnDescribe;           /*! 群描述*/
    QPushButton* btnComplete;           /*! 完成群创建*/
    DClusterInfo* curClusterInfo;       /*! 群信息*/
private:
    void initializeWidget();
    void handleClusterInfo(InputPacket& inpack);
    void setGroupButton(GroupButton* btn, QPixmap photo, QString title);

private slots:
    void on_btnPerfactClusterInfo_clicked();
    void on_btnInviationDevice_clicked();
    void on_btnCompleted_clicked();
};

#endif // DCLUSTERCREATESUCCESSEDPAGE_H
