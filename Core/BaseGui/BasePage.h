#ifndef BASEPAGE_H
#define BASEPAGE_H

class QLineEdit;
class QFrame;
class QScrollArea;
class QScroller;
class QEvent;
class QValidator;
class QVBoxLayout;
class InputPacket;
class TouchScroller;

#include <QtGlobal>
#include "BaseWidget.h"

#if defined Q_OS_WIN32 || defined Q_OS_WIN64 || defined Q_OS_WIN
    using namespace std;
    # pragma execution_character_set("utf-8")
#endif

enum PageType{
    MainPage = 0x01,
    SubPage = 0x02
};

class BasePage: public BaseWidget
{
    Q_OBJECT
public:
    BasePage(QWidget* parent, PageType type = SubPage);
    virtual ~BasePage();

public:
    bool event(QEvent *event);
    virtual void fetchData(InputPacket&) = 0;

public:
    void installScrollViewportArea(quint32 height = 0);
    void setBodyRealHeight(quint32 height);
    void setBodyScreenHeight(quint32 height);
    void setTopbarLayout(QLayout* layout);
    void setBodyPartLayout(QLayout* layout);
    void setBottombarLayout(QLayout* layout);

    quint32 topBarHeight();
    quint32 bodyPartHeight();
    quint32 bottomBarHeight();
    quint32 scrollAreaHasBottomBarHeight();
    quint32 scrollAreaNoBottomBarHeight();

signals:
    void readyRead(quint16 cmdCode, char* data, quint16 dataLen);

private:
    PageType pageType;              /*! 页面类型*/
    bool flipOver;                  /*! 翻页标志*/
    QFrame* topBar;                 /*! 顶部工具条*/
    QFrame* bodyPart;               /*! 页面主体*/
    QFrame* bottomBar;              /*! 底部任务栏*/
    QScrollArea* touchScrollArea;   /*! 触摸滚动条*/
    QVBoxLayout* totalWidgetLayouyt;/*! 整体页面布局*/

    bool m_moveFlag;                /*! 鼠标移动标志*/
    QPoint m_preMovePos;            /*! 前一个鼠标移动位置*/
    QPoint m_pressedPos;            /*! 触摸或鼠标按下位置*/
    QPoint m_releasePos;            /*! 触摸或鼠标抬起位置*/
    quint32 m_winWidth;             /*! 当前窗口宽度*/
    quint32 m_winHeight;            /*! 当前窗口高度*/
    quint32 m_statusBarScrnHeight;  /*! 状态栏屏幕高度*/
    quint32 m_topBarScrnHeight;     /*! 工具栏屏幕高度*/
    quint32 m_bodyScrnHeight;       /*! 主体栏屏幕高度*/
    quint32 m_bottomBarScrnHeight;  /*! 任务栏屏幕高度*/
    Qt::WindowFlags m_flags;

private:
    QScroller *touchScroller;

private:
    void initBaseWidget();
};
#endif // BASETPAGE_H
