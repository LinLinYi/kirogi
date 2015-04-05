#ifndef TOUCHSCROLLER_H
#define TOUCHSCROLLER_H

#include <QLayout>
#include <QScroller>
#include <QScrollArea>

/*! \class TouchScroller
 *  \brief 安装触摸区域滚动的类
 */
class TouchScroller
{
public:
    TouchScroller(){}
    ~TouchScroller(){}

public:
    /*! 安装触摸滚动*/
    static QScroller* installKineticScroller(QAbstractScrollArea *area);

private:
    /*! 触摸滚动条*/
    static QScroller* m_scroller;
};

#endif // TOUCHSCROLLER_H
