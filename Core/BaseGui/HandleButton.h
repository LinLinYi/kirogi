#ifndef HANDLEBUTTON_H
#define HANDLEBUTTON_H

class QPushButton;
class QToolButton;
class QPaintEvent;
class QLabel;
class QPixmap;
class QHBoxLayout;

#include "GPushButton.h"

class HandleButton : public GPushButton
{
    Q_OBJECT
public:
    HandleButton(QWidget* parent = 0);
    virtual ~HandleButton(void);

public:
    void setPixMap(const QPixmap& pixMap, int w = 0, int h = 0);
    void setText(const QString& name);
    void setUpReserve(const QString& reserve);
    void setDownReserve(const QString& reserve);
    void setStatus(const QString& status);
    void setDescribe(const QString& describe);
    void setContentsMargins(int left, int top, int right, int bottom);

private:
    QString m_name;            /*! 名称*/
    QPixmap m_pixMap;          /*! 标题图片*/
    QString m_status;          /*! 状态*/
    QString m_describe;        /*! 消息内容*/
    QString m_upReserve;       /*! 上预留*/
    QString m_downReserve;     /*! 下预留*/

    QLabel* m_pLblPhoto;       /*! 图片显示*/
    QLabel* m_pLblName;        /*! 名称显示*/
    QLabel* m_pLblStatus;      /*! 状态显示*/
    QLabel* m_pLblDescribe;    /*! 描述显示*/
    QLabel* m_pLblUpReserve;   /*! 上预留显示*/
    QLabel* m_pLblDownReserve; /*! 下预留显示*/
    QHBoxLayout* m_pHboxLayout;/*! 页面布局*/

private:
    void initializeWidget();    
};

#endif // HANDLEBUTTON_H
