#include <QVBoxLayout>
#include <QtGlobal>
#include <QLineEdit>
#include <QFrame>
#include <QScrollArea>
#include <QScroller>
#include <QEvent>
#include <QMouseEvent>

#include "BasePage.h"
#include "ScreenPara.h"
#include "StatusBarParam.h"
#include "InputPacket.h"
#include "TouchScroller.h"
#include "BaseWidget.h"
#include "GlobalDefine.hpp"
#include "SheetStyleConfig.hpp"

BasePage::BasePage(QWidget* parent, PageType type)
    : BaseWidget(parent)
    , pageType(type)
    , flipOver(true)
    , totalWidgetLayouyt(NULL)
    , m_moveFlag(false)
    , m_preMovePos(QPoint(0,0))
    , touchScroller(NULL)
{
    this->setAutoFillBackground(true);
    QPalette palette;
    palette.setColor(QPalette::Background, QColor(0xec,0xec,0xec));
    this->setPalette(palette);
    this->setAttribute(Qt::WA_AcceptTouchEvents);

    this->initBaseWidget();
}

void BasePage::initBaseWidget()
{
    //屏幕参数设置
    m_winWidth = this->screenWidth();
    m_winHeight = this->screenHeight();

    if(ScreenPara::is_iPhone()){
        m_statusBarScrnHeight = Screen::PhoneStatusBarHeight;
        m_topBarScrnHeight = Screen::PhoneNavigationBarHeight;
        m_bottomBarScrnHeight = Screen::PhoneToolBarHeight;
    }else{
        m_statusBarScrnHeight = Screen::PadStatusBarHeight;
        m_topBarScrnHeight = Screen::PadNavigationBarHeight;
        m_bottomBarScrnHeight = Screen::PadToolBarHeight;
    }
    
    this->setFixedHeight(m_winHeight);
    this->setFixedWidth(m_winWidth);
    m_bodyScrnHeight = m_winHeight - m_topBarScrnHeight;

    //主窗口顶部
    topBar = new QFrame;
    topBar->setFixedHeight(m_topBarScrnHeight);
    topBar->setFixedWidth(m_winWidth);
    topBar->setStyleSheet(SheetStyle::TOPBAR);

    //主窗口主体
    bodyPart = new QFrame;
    bodyPart->setMinimumHeight(m_bodyScrnHeight);
    bodyPart->setFixedWidth(m_winWidth);
    bodyPart->setStyleSheet(SheetStyle::BODY);
    bodyPart->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed );

    //底部任务栏
    bottomBar = new QFrame;
    bottomBar->setMinimumHeight(m_bottomBarScrnHeight);
    bottomBar->setFixedWidth(m_winWidth);
    bottomBar->setStyleSheet(SheetStyle::TASKBAR);
    bottomBar->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed );
    bottomBar->hide();

    //触摸滚动设置
    touchScrollArea = new QScrollArea;
    touchScrollArea->setWidget(bodyPart);
    touchScrollArea->setWidgetResizable( true );
    touchScrollArea->setAttribute(Qt::WA_AcceptTouchEvents);
    touchScrollArea->setStyleSheet(SheetStyle::QSCROLLAREA);
    touchScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    touchScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    totalWidgetLayouyt = new QVBoxLayout;
    totalWidgetLayouyt->addWidget(topBar);
    totalWidgetLayouyt->addWidget(touchScrollArea);
    totalWidgetLayouyt->addWidget(bottomBar);
    totalWidgetLayouyt->setAlignment(Qt::AlignTop);
    totalWidgetLayouyt->setSpacing(0);
    totalWidgetLayouyt->setContentsMargins(0, 0, 0, 0);
    this->setLayout(totalWidgetLayouyt);
}

bool BasePage::event(QEvent *event)
{
    if(pageType == MainPage || flipOver == false){
        QWidget::event(event);
        return true;
    }

    //页面翻页功能
    QMouseEvent* mEvent = (QMouseEvent*)event;
    switch(event->type()){
    case QEvent::MouseButtonPress:
        mEvent->accept();
        m_pressedPos = mEvent->pos() + this->pos();
        break;
    case QEvent::MouseMove:
        if(m_pressedPos.x() > 10)
            return true;

        mEvent->accept();
        if(this->pos().x() >= 0){
            this->setGeometry(QRect(mEvent->x() + this->pos().x(), 0, m_winWidth, m_winHeight));
        }

        m_preMovePos = mEvent->pos() + this->pos();
        m_moveFlag = true;
        return true;
    case QEvent::MouseButtonRelease:
        if(m_pressedPos.x() > 10)
            return true;

        mEvent->accept();
        m_releasePos = mEvent->pos() + this->pos();
        m_releasePos -= m_pressedPos;

        if(m_moveFlag){
            if(m_releasePos.x() >= Screen::PhoneMinPagingPixel){
                this->hide();
                this->deleteLater();
            }else{
                this->setGeometry(QRect(0, 0, m_winWidth, m_winHeight));
            }
            m_moveFlag = false;
            m_preMovePos = QPoint(0,0);
        }
        break;
    }
    return QWidget::event(event);
}

void BasePage::installScrollViewportArea(quint32 height)
{
    this->setBodyRealHeight(height);
    touchScroller = TouchScroller::installKineticScroller(touchScrollArea);
}

void BasePage::setBodyRealHeight(quint32 height)
{
    if(height <= m_bodyScrnHeight){
        bodyPart->setFixedHeight(m_bodyScrnHeight);
        touchScrollArea->setFixedHeight(m_bodyScrnHeight - 1);
    }
    else{
        bodyPart->setFixedHeight(height);
        touchScrollArea->setFixedHeight(m_bodyScrnHeight -1);
    }
}

void BasePage::setBodyScreenHeight(quint32 height)
{
    m_bodyScrnHeight = height;
    bodyPart->setFixedHeight(height);
}

void BasePage::setTopbarLayout(QLayout *layout)
{
    topBar->setLayout(layout);
}

void BasePage::setBodyPartLayout(QLayout *layout)
{
    bodyPart->setLayout(layout);
}

void BasePage::setBottombarLayout(QLayout *layout)
{
    bottomBar->show();
    bottomBar->setLayout(layout);
}

quint32 BasePage::scrollAreaNoBottomBarHeight()
{
    return m_bodyScrnHeight;
}

quint32 BasePage::scrollAreaHasBottomBarHeight()
{
    return (m_winHeight - m_topBarScrnHeight - m_bottomBarScrnHeight);
}

quint32 BasePage::topBarHeight()
{
    return m_topBarScrnHeight;
}

quint32 BasePage::bodyPartHeight()
{
    return bodyPart->height();
}

quint32 BasePage::bottomBarHeight()
{
    return m_bottomBarScrnHeight;
}

BasePage::~BasePage()
{

}
