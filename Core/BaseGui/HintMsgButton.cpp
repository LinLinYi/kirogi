#include <QRect>
#include <QPainter>
#include <QPixmap>
#include <QVBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QHBoxLayout>
#include <QGestureEvent>

#include "ImageConfig.hpp"
#include "HintMsgButton.h"

#define MAX_LEFT_MOVE_DIST (-10)
#define MAX_RIGHT_MOVE_DIST (10)

HintMsgButton::HintMsgButton(QWidget *parent) : GPushButton(parent)
{
    this->initializeWidget();
    this->setAttribute(Qt::WA_AcceptTouchEvents);
    this->grabGesture(Qt::PanGesture);
    this->grabGesture(Qt::PinchGesture);
    this->grabGesture(Qt::SwipeGesture);
}

void HintMsgButton::initializeWidget()
{
    //按钮图片
    m_pLblPhoto = new QLabel(this);
    m_pLblPhoto->hide();
    m_pLblPhoto->setMargin(0);
    m_pLblPhoto->setStyleSheet("font:bold 15px; color:black; border-width: 0px;");

    //按钮标题名称
    m_pLblName = new QLabel(this);
    m_pLblName->hide();
    m_pLblName->setAlignment(Qt::AlignVCenter);
    m_pLblName->setStyleSheet("font:bold 15px; color:black; border-width: 0px;");

    //时间显示
    m_pLblTime = new QLabel(this);
    m_pLblTime->hide();
    m_pLblTime->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_pLblTime->setStyleSheet("color:gray; padding-top: 5px; font: 10px; border-width: 0px;");

    QHBoxLayout* pHUpLayout = new QHBoxLayout();
    pHUpLayout->addWidget(m_pLblName);
    pHUpLayout->addWidget(m_pLblTime);
    pHUpLayout->setMargin(0);

    //消息描述
    m_pLblDescribe = new QLabel(this);
    m_pLblDescribe->hide();
    m_pLblDescribe->setAlignment(Qt::AlignVCenter);
    m_pLblDescribe->setStyleSheet("color:#8c8c8c; font: 13px; border-width: 0px;");

    //最新消息内容
    m_pLblUnReadMsgCount = new QLabel(this);
    m_pLblUnReadMsgCount->hide();
    m_pLblUnReadMsgCount->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_pLblUnReadMsgCount->setStyleSheet("color:#8c8c8c; font:13px; border-width: 0px;");

    QHBoxLayout* pHDownLayout = new QHBoxLayout();
    pHDownLayout->addWidget(m_pLblDescribe);
    pHDownLayout->addWidget(m_pLblUnReadMsgCount);
    pHDownLayout->setMargin(0);

    QVBoxLayout* pVboxLayout = new QVBoxLayout();
    pVboxLayout->addStretch(0);
    pVboxLayout->addLayout(pHUpLayout);
    pVboxLayout->addLayout(pHDownLayout);
    pVboxLayout->addStretch(0);
    pVboxLayout->setMargin(0);
    pVboxLayout->setSpacing(0);

    //删除当前widget
    m_btnDeleteWidget = new QPushButton(this);
    m_btnDeleteWidget->hide();
    m_btnDeleteWidget->setText(QString(tr("删除")));
    m_btnDeleteWidget->setStyleSheet("color:white; font:20px; background-color:rgb(255,59,48); border-width: 0px;");
    connect(m_btnDeleteWidget, SIGNAL(clicked()), this, SLOT(on_deleteWidget_clicked()));

    m_pHboxLayout = new QHBoxLayout(this);
    m_pHboxLayout->addWidget(m_pLblPhoto);
    m_pHboxLayout->addLayout(pVboxLayout);
    m_pHboxLayout->addWidget(m_btnDeleteWidget);
    this->setLayout(m_pHboxLayout);
}

bool HintMsgButton::event(QEvent *e)
{
#ifdef Q_OS_IOS || Q_OS_ANDROID//触摸事件处理
    switch(e->type()){
        case QEvent::TouchBegin:{
            e->accept();
            return true;
        }
        case QEvent::TouchUpdate:{
            e->accept();
            return true;
        }
        case QEvent::TouchCancel:{
            e->accept();
            return true;
        }
        case QEvent::TouchEnd:{
            e->accept();
            QTouchEvent* evType = (QTouchEvent*)(e);
            const QList<QTouchEvent::TouchPoint>& points = evType->touchPoints();

            for(int i = 0; i<points.size(); i++){
                const QTouchEvent::TouchPoint &point = points.at(i);

                int moveX = point.lastPos().x() - point.startPos().x();
                int moveY = point.lastPos().y() - point.startPos().y();
                if( moveX < MAX_LEFT_MOVE_DIST && moveY > -(this->height()*0.9) && moveY < this->height()*0.9)
                {//左滑动显示删除按钮
                    m_pLblPhoto->hide();
                    m_btnDeleteWidget->show();
                    m_btnDeleteWidget->setFixedHeight(this->height());
                    m_btnDeleteWidget->setFixedWidth(this->width()*0.25);
                }
                else if(moveX > MAX_LEFT_MOVE_DIST && moveX < MAX_RIGHT_MOVE_DIST && !m_btnDeleteWidget->isVisible())
                {//删除按钮，点击widget
                    emit clicked();
                }
                else if((moveX > MAX_LEFT_MOVE_DIST && moveX < MAX_RIGHT_MOVE_DIST
                         && m_btnDeleteWidget->isVisible() && point.lastPos().x() > (this->width() - this->width()*0.25)))
                {//点击删除按钮
                    emit deleteClicked();
                }
                else if(moveX > MAX_RIGHT_MOVE_DIST || (moveX > MAX_LEFT_MOVE_DIST && moveX < MAX_RIGHT_MOVE_DIST
                                       && m_btnDeleteWidget->isVisible() && point.lastPos().x() < (this->width() - this->width()*0.25)))
                {//隐藏删除按钮
                    m_pLblPhoto->show();
                    m_btnDeleteWidget->hide();
                }
            }
            return true;
        }
        default:
            break;
    }
#else//鼠标事件处理
    QMouseEvent* mEvent = (QMouseEvent*)e;
    switch(e->type()){
        case QEvent::MouseButtonPress:{
            e->accept();
            startPos = mEvent->pos();
            return true;
        }
        case QEvent::MouseMove:{
            e->accept();
            return true;
        }
        case QEvent::MouseButtonRelease:{
            e->accept();
            lastPos = mEvent->pos();
            int moveX = lastPos.x() - startPos.x();

            if( moveX < MAX_LEFT_MOVE_DIST)
            {//左滑动显示删除按钮
                m_pLblPhoto->hide();
                m_btnDeleteWidget->show();
                m_btnDeleteWidget->setFixedHeight(this->height());
                m_btnDeleteWidget->setFixedWidth(this->width()*0.25);
            }
            else if(moveX > MAX_LEFT_MOVE_DIST && moveX < MAX_RIGHT_MOVE_DIST && !m_btnDeleteWidget->isVisible())
            {//删除按钮隐藏，点击widget
                emit clicked();
            }
            else if(moveX > MAX_RIGHT_MOVE_DIST || (moveX > MAX_LEFT_MOVE_DIST && moveX < MAX_RIGHT_MOVE_DIST
                                                    && m_btnDeleteWidget->isVisible() && lastPos.x() < (this->width() - this->width()*0.25)))
            {//隐藏删除按钮
                m_pLblPhoto->show();
                m_btnDeleteWidget->hide();
            }
            lastPos = startPos;//完成后，表示鼠标不移动
            return true;
        }
    }
#endif
    return QWidget::event(e);
}

void HintMsgButton::paintUnreadHintPixmap(QPixmap &pixmap, QString& hintText)
{
    QFont font;
    font.setFamily("Times");
    font.setPixelSize(12);
    font.setBold(true);

    QPainter painter;
    painter.begin(&pixmap);
    painter.setPen(Qt::white);
    painter.setFont(font);
    painter.drawText(0,0,pixmap.width()/2,pixmap.height()/2, Qt::AlignCenter, hintText);
    painter.end();
}

void HintMsgButton::setMsgPixMap(const QPixmap &pixMap, int w, int h)
{
    m_pLblPhoto->show();
    m_msgPixMap = pixMap;

    if(w == 0 && h == 0){
         m_msgPixMap.setDevicePixelRatio(2.0);
         m_pLblPhoto->setFixedHeight(m_msgPixMap.height()/2);
         m_pLblPhoto->setFixedWidth(m_msgPixMap.width()/2);
    }else{
        m_pLblPhoto->setFixedHeight(h);
        m_pLblPhoto->setFixedWidth(w);
        m_pLblPhoto->setAutoFillBackground(true);
    }
    m_pLblPhoto->setPixmap(m_msgPixMap);
}

void HintMsgButton::setMsgName(const QString &name)
{
    m_msgName = name;
    m_pLblName->show();
    m_pLblName->setText(m_msgName);
}

void HintMsgButton::setMsgTime(const QString &time)
{
    m_msgTime = time;
    m_pLblTime->show();
    m_pLblTime->setText(time);
    m_pLblTime->setContentsMargins(0,0,this->width()*0.03,0);
}

void HintMsgButton::setMsgDescribe(const QString &describe)
{
    m_msgDescribe = describe;
    m_pLblDescribe->show();
    m_pLblDescribe->setText(m_msgDescribe);
}

void HintMsgButton::setUnreadMsgCount(const quint16 count)
{
    m_unReadMsgCount = count;
    if(count <= 0){
        m_pLblUnReadMsgCount->hide();
        return;
    }

    QPixmap pixmap;
    QString textTip = QString::number(m_unReadMsgCount);
    if(m_unReadMsgCount < 10){//未读消息为个位数
        QPixmap pixmapUnit(ImagePath::HINT_UNIT);
        pixmap = pixmapUnit;
    }
    else if(m_unReadMsgCount >= 10 && m_unReadMsgCount <= 99){//未读消息为十位数
        QPixmap pixmapDecade(ImagePath::HINT_DECADE);
        pixmap = pixmapDecade;
    }else{//未读消息为百位数
        QPixmap pixmapDecade(ImagePath::HINT_HUNDRED);
        pixmap = pixmapDecade;
        textTip = QString("99+");
    }

    pixmap.setDevicePixelRatio(2);
    this->paintUnreadHintPixmap(pixmap, textTip);
    m_pLblUnReadMsgCount->setContentsMargins(0,0,this->width()*0.04,0);
    m_pLblUnReadMsgCount->setPixmap(pixmap);
    m_pLblUnReadMsgCount->show();
}

void HintMsgButton::setContentsMargins(int left, int top, int right, int bottom)
{
    m_pHboxLayout->setMargin(0);
    QWidget::setContentsMargins(left, top, right, bottom);
}

void HintMsgButton::on_deleteWidget_clicked()
{
    emit deleteClicked();
}

HintMsgButton::~HintMsgButton()
{

}



