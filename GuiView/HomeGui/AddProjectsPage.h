#ifndef ADDPROJECTSPAGE_H
#define ADDPROJECTSPAGE_H

class QLabel;
class QPushButton;
class QToolButton;
class InputPacket;
class GroupButton;
class NavigationBar;

#include "BasePage.h"

class AddProjectsPage : public BasePage
{
    Q_OBJECT
public:
    explicit AddProjectsPage(QWidget* parent = 0);
    virtual ~AddProjectsPage();

public:
    void fetchData(InputPacket& inpack);

private:
    NavigationBar* navigationBar;           /*! 导航栏*/
    QToolButton* btnSweepProject;           /*! 扫一扫按钮*/
    GroupButton* btnAddProject;             /*! 添加项目按钮*/
    GroupButton* btnCreateProject;          /*! 创建项目按钮*/

private:
    void initializeWidget();
    void setGroupButton(GroupButton* groupButton, QPixmap photo, QString title);

private slots:
    void on_btnPrePage_clicked();
    void on_btnSweepProject_clicked();
    void on_btnAddProject_clicked();
    void on_btnCreateProject_clicked();
};

#endif // ADDPROJECTSPAGE_H
