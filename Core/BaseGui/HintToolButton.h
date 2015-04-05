#ifndef HINTTOOLBUTTON_H
#define HINTTOOLBUTTON_H

#include <QToolButton>

class HintToolButton : public QToolButton
{
    Q_OBJECT
public:
    HintToolButton(QWidget* parent = 0);
    virtual ~HintToolButton(void);

public:
    void setUnreadMsgCount(quint16 count);

protected:
    void paintEvent(QPaintEvent* event);
private:
    quint16 unreadMsgCount;
};

#endif // HINTTOOLBUTTON_H
