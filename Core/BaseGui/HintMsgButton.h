#ifndef HINTMSGBUTTON_H
#define HINTMSGBUTTON_H

class QLabel;
class QPixmap;
class QHBoxLayout;

#include "GPushButton.h"
#include "GlobalDefine.hpp"

class HintMsgButton : public GPushButton
{
    Q_OBJECT
public:
    HintMsgButton(QWidget* parent = 0);
    ~HintMsgButton();

public:
    bool event(QEvent *e);

public:
    void setMsgPixMap(const QPixmap& pixMap, int w = 0, int h = 0);
    void setMsgName(const QString& name);
    void setMsgTime(const QString& time);
    void setMsgDescribe(const QString& describe);
    void setUnreadMsgCount(const quint16 count);
    QString& getMsgTime() { return m_msgTime; }
    QPixmap& getMsgPixMap() { return m_msgPixMap; }

    void setMsgType(Mi::NotifyCategory type) { m_msgType = type; }
    Mi::NotifyCategory getMsgType() { return m_msgType; }
    void setContentsMargins(int left, int top, int right, int bottom);

private:
    Mi::NotifyCategory m_msgType;   /*! 消息类型ID*/
    QPixmap m_msgPixMap;            /*! 标题图片*/
    QString m_msgName;              /*! 名称*/
    QString m_msgTime;              /*! 消息日期*/
    QString m_msgDescribe;          /*! 消息内容*/
    quint16 m_unReadMsgCount;       /*! 未读消息数目*/

    QLabel* m_pLblPhoto;            /*! 图片显示*/
    QLabel* m_pLblName;             /*! 名称显示*/
    QLabel* m_pLblTime;             /*! 日期显示*/
    QLabel* m_pLblDescribe;         /*! 描述显示*/
    QLabel* m_pLblUnReadMsgCount;   /*! 未读消息显示*/
    QPushButton* m_btnDeleteWidget; /*! 删除当前widget*/
    QHBoxLayout* m_pHboxLayout;     /*! 页面布局*/

    quint16 screenHeight;           /*! 屏幕高度*/
    quint16 screenWidth;            /*! 屏幕宽度*/

    QPoint startPos;
    QPoint lastPos;

signals:
    void deleteClicked();

private:
    void initializeWidget();
    void paintUnreadHintPixmap(QPixmap& pixmap, QString& hintText);

private slots:
    void on_deleteWidget_clicked();
};

#endif // HINTMSGBUTTON_H
