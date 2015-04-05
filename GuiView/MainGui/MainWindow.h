#ifndef MAINWINDOW_H
#define MAINWINDOW_H

class QFrame;
class QVBoxLayout;
class QHBoxLayout;
class InputPacket;
class HomePage;
class ContactPage;
class DevicePage;
class MorePage;
class QToolButton;
class BasePage;

#include <QVBoxLayout>
#include "BaseWidget.h"

class MainWindow : public BaseWidget
{
    Q_OBJECT

public:
    MainWindow(quint64 accountID = 0);
    ~MainWindow();
    
public:
    void fetchData(InputPacket& inpack);

private:
    quint64 curAccountID;                 /*! 当前账户ID*/
    HomePage* homePage;                   /*! "首页"页*/
    DevicePage* devicePage;               /*! "设备"页*/
    ContactPage* contactPage;            /*! "通讯"页*/
    MorePage* morePage;                   /*! "更多"页*/

    QToolButton* m_btnHomePage;           /*! "首页"按钮*/
    QToolButton* m_btnDevicePage;         /*! "设备"按钮*/
    QToolButton* m_btnContactsPage;       /*! "通讯"按钮*/
    QToolButton* m_btnMorePage;           /*! "更多"按钮*/

    QFrame* m_bottomBar;                  /*! 底部任务栏*/
    BasePage* m_curDestPage;              /*! 当前显示页面*/
    quint32 m_screenCurWidth;             /*! 当前窗口宽度*/
    quint32 m_screenCurHeight;            /*! 当前窗口高度*/

    quint32 m_statusHeight;               /*! 状态栏高度*/
    quint32 m_topBarHeight;               /*! 底部导航栏高度*/
    quint32 m_bottomBarHeight;            /*! 底部任务栏高度*/
    QVBoxLayout m_mainLayout;             /*! 主窗口布局*/

private:
    void initializeWidget();
    void initLocalDataBase();
    void labelHintInfoDisplay();
    void setToolButton(QToolButton *btn, QString imagePath);
    void adjustPages(BasePage* page);
    void transferDataToAnotherPage(InputPacket& inack, BasePage* basePage);

signals:
    void readyRead(quint16 cmdCode, char* data, quint16 dataLen);

private slots:
    void on_btnHomePage_Clicked();
    void on_btnContactsPage_Clicked();
    void on_btnDevicePage_Clicked();
    void on_btnMorePage_Clicked();
    void on_subPage(quint16 cmdCode, char* data, quint16 dataLen);
};

#endif
