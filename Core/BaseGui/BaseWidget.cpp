#include "BaseWidget.h"
#include "ScreenPara.h"
#include "StatusBarParam.h"

BaseWidget::BaseWidget(QWidget* parent) : QWidget(parent)
{
    m_height = ScreenPara::getInstance()->screenHeight();
    m_width = ScreenPara::getInstance()->screenWidth();
    m_statusBarHeight = StatusBarParam::StatusBarHeight();
}

quint32 BaseWidget::screenHeight()
{
    return m_height;
}

quint32 BaseWidget::screenWidth()
{
    return m_width;
}

quint32 BaseWidget::statusBarHeight()
{
    return m_statusBarHeight;
}

BaseWidget::~BaseWidget()
{

}
