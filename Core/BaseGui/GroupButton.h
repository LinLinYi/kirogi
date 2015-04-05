#ifndef GROUPINGBUTTON_H
#define GROUPINGBUTTON_H

class QLbael;
class QPixmap;
class QHBoxLayout;
class QEvent;
class QLabel;

#include "GPushButton.h"

class GroupButton : public GPushButton
{
public:
    GroupButton(QWidget* parent = 0);
    ~GroupButton();

public:
    void setPixMap(const QPixmap& pixMap, int w = 0, int h = 0);    /*! 设置标题头图片*/
    void setText(const QString& text);                              /*! 设置标题*/
    void setRemarks(const QString& remarks);                        /*! 标题备注*/
    void setDescribe(const QString& describe);                      /*! 标题描述*/
    void setDescribe(const QPixmap &describe, int w = 0, int h = 0);/*! 标题描述*/
    void setReserve(const QString& reserve);                        /*! 设置预留内容*/
    void setReserve(QPixmap reserve);                               /*! 设置预留内容*/
    void setSelectFlag(const bool isPressed = false);               /*! 按钮按下标志*/
    bool getSelectFlag() { return selectFlag; }                     /*! 获取按钮按下标志*/
    void setPortraitPath(const QString& strPath);                   /*! 设置头像路径*/
    QString& getPortraitPath() { return pathPortrait; }             /*! 获取头像路径*/
    void setContentsMargins(int left, int top, int right, int bottom);

protected:
    bool event(QEvent *e);

private:
    QPoint pressedPos;
    QPoint releasedPos;

    QLabel* lblHead;
    QLabel* lblText;
    QLabel* lblDescribe;
    QLabel* lblRemarks;
    QLabel* lblReserve;

    QPixmap pixPortrait;
    QString pathPortrait;
    QString strTitle;
    QString strDescribe;
    QPixmap pixDescribe;
    QString strRemarks;
    QString strReserve;
    QPixmap pixReserve;
    QHBoxLayout* hblTotal;
    bool selectFlag;

private:
    void initializeWidget();
};


#endif // GROUPBUTTON_H

