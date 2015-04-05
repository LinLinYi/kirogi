#ifndef BASEWIDGET_H
#define BASEWIDGET_H

#include <QWidget>

class BaseWidget : public QWidget
{
public:
    explicit BaseWidget(QWidget* parent = 0);
    virtual ~BaseWidget();

public:
    quint32 screenHeight();
    quint32 screenWidth();
    quint32 statusBarHeight();

private:
    quint32 m_height;
    quint32 m_width;
    quint32 m_statusBarHeight;
};

#endif // BASEWIDGET_H
