#ifndef LOADINGPAGE_H
#define LOADINGPAGE_H

class QLabel;
class QTimer;
class QProgressIndicator;

#include "BaseWidget.h"

class LoadingPage : public BaseWidget
{
    Q_OBJECT
public:
    explicit LoadingPage(QWidget *parent = 0);
    virtual ~LoadingPage();

public:
    void setTimeOut(int mesc);
    void start(int mesc = 0);
    void stop();

protected:
    void paintEvent(QPaintEvent *);

private:
    QTimer* timer;                  /*! 超时定时器*/
    QLabel* loadingGif;             /*! 加载gif*/
    QLabel* loadingText;            /*! 加载gif标签*/
    QProgressIndicator* proIndicator;/*! Loading图像*/

    int timeMesc;                   /*! 超时时间*/
    unsigned int m_screenWidth;     /*! 提示页面宽度*/
    unsigned int m_screenheight;    /*! 提示页面高度*/

private:
    void initializeWidget();

signals:
    void timeOut();

private slots:
    void updateHint();
};

#endif // LOADINGPAGE_H
