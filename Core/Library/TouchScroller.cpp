#include <QAbstractScrollArea>
#include <QEasingCurve>
#include <QtGlobal>
#include "TouchScroller.h"

QScroller* TouchScroller::installKineticScroller(QAbstractScrollArea *area)
{
    if (area != NULL){
        QScrollerProperties prop = QScroller::scroller(area)->scrollerProperties();
        prop.setScrollMetric(QScrollerProperties::AxisLockThreshold, 1.0);
        prop.setScrollMetric(QScrollerProperties::MousePressEventDelay, 0.0);
        prop.setScrollMetric(QScrollerProperties::SnapPositionRatio, 0.33);
        prop.setScrollMetric(QScrollerProperties::SnapTime, 0.1);
        prop.setScrollMetric(QScrollerProperties::FrameRate, QScrollerProperties::Fps60);
        prop.setScrollMetric(QScrollerProperties::OvershootDragDistanceFactor, 0.2);
        prop.setScrollMetric(QScrollerProperties::DragVelocitySmoothingFactor, 0.0);    //平滑拖动因子
        prop.setScrollMetric(QScrollerProperties::MaximumClickThroughVelocity, 0.0);    //点击widgets后最大速率
        prop.setScrollMetric(QScrollerProperties::AcceleratingFlickMaximumTime, 1.0);   //加速最大时间
        prop.setScrollMetric(QScrollerProperties::AcceleratingFlickSpeedupFactor, 1.2); //加速因子
        prop.setScrollMetric(QScrollerProperties::DecelerationFactor, 0.1);             //减速因子，并依赖于ScrollingCurve
        prop.setScrollMetric(QScrollerProperties::MinimumVelocity, 0.0);                //滑动最小速率
        prop.setScrollMetric(QScrollerProperties::MaximumVelocity, 0.12);               //触摸后滚动的最大速率
        prop.setScrollMetric(QScrollerProperties::OvershootDragResistanceFactor,0.33);  //触摸移动后偏移屏幕的最大距离比
        prop.setScrollMetric(QScrollerProperties::OvershootScrollDistanceFactor, 0.1);  //触摸移动后滚动距离因子
        prop.setScrollMetric(QScrollerProperties::DragStartDistance, 0.001);            //手势滑动的最小距离（m）

        area->setAttribute(Qt::WA_AcceptTouchEvents);
        QScroller::grabGesture(area, QScroller::LeftMouseButtonGesture);
        QScroller::scroller(area)->setScrollerProperties(prop);
        return QScroller::scroller(area);
    }
    return QScroller::scroller(area);
}
