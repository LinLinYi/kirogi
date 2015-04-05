#ifndef GPUSHBUTTON_H
#define GPUSHBUTTON_H

#include <QPushButton>

class GPushButton : public QPushButton
{
public:
    GPushButton(QWidget* parent = 0);
    ~GPushButton();

public:
    quint64 getIndexOne();//索引1：获取当前按钮标识或唯一索引
    quint64 getIndexTwo();//索引2：获取当前按钮标识或唯一索引
    void setIndexOne(quint64 index);//索引1：设置当前按钮标识或唯一索引
    void setIndexTwo(quint64 index);//索引2：设置当前按钮标识或唯一索引

private:
    quint64 m_indexOne;
    quint64 m_indexTwo;
};

#endif // GPUSHBUTTON_H
