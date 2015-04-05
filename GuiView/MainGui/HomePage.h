#ifndef HOMEPAGE_H
#define HOMEPAGE_H

class QLabel;
class QLineEdit;
class QPushButton;
class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class InputPacket;
class AddProjectsPage;
class ListWidget;
class GroupButton;
class NavigationBar;

#include "BasePage.h"

class HomePage : public BasePage
{
    Q_OBJECT
public:
    explicit HomePage(QWidget *parent = 0, PageType type = MainPage);
    ~HomePage();

public:
    void fetchData(InputPacket& inpack);

private:
    NavigationBar* navigationBar;       /*! 导航栏*/
    quint16 curFixedContentHeight;       /*! 当前页面固定高度*/
    QLabel* lblHomeGraphics;            /*! 主页图片预览*/
    QLabel* lblProjectHint;             /*! 项目组标签显示栏*/

    ListWidget* sceneListWidget;        /*! 场景列表*/
    AddProjectsPage* addProjectsPage;   /*! 添加项目页面*/

private:
    void initializeWidget();
    void displayConcretePage(BasePage* page);
    void setGroupButton(GroupButton* btn);
    void setListWidget(ListWidget *listWidget);
    void test();

private slots:
    void on_btnSearch_clicked();
    void on_btnAddProject_clicked();
    void on_deleteSubPages();
    void on_recvDataFromOtherPage(quint16 cmdCode, char* packData, quint16 dataLen);
};

#endif // HOMEPAGE_H
