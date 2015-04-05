#include "GPushButton.h"

GPushButton::GPushButton(QWidget* parent) : QPushButton(parent)
{
    m_indexOne = 0;
    m_indexTwo = 0;
}

void GPushButton::setIndexOne(quint64 index)
{
    m_indexOne = index;
}

void GPushButton::setIndexTwo(quint64 index)
{
    m_indexTwo = index;
}

quint64 GPushButton::getIndexOne()
{
    return m_indexOne;
}

quint64 GPushButton::getIndexTwo()
{
    return m_indexTwo;
}

GPushButton::~GPushButton()
{

}
