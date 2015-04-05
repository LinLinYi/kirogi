#include <QDesktopWidget>
#include <QApplication>
#include <QtGlobal>

#include "ScreenPara.h"
#include "ToolKit.h"

#ifdef Q_OS_IOS
#import <UIKit/UIKit.h>
#endif

ScreenPara* ScreenPara::m_pInstance = NULL;
ScreenPara::ScreenPara(QObject *parent) : QObject(parent)
{
    QDesktopWidget *desktop = QApplication::desktop();
    QRect screen = desktop->availableGeometry();
    m_screenHeight = screen.height();
    m_screenWidth  = screen.width();
}

quint32 ScreenPara::screenHeight()
{
#ifdef Q_OS_IOS
    return m_screenHeight;
#elif Q_OS_ANDROID
    return m_screenHeight;
#else
    return 568;
#endif
}

quint32 ScreenPara::screenWidth()
{
#ifdef Q_OS_IOS
    return m_screenWidth;
#elif Q_OS_ANDROID
    return m_screenWidth;
#else
    return 320;
#endif
}

bool ScreenPara::is_iPhone()
{
#ifdef Q_OS_IOS
    if ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPhone){
        return true;
    }else
        return false;
#else
    return true;
#endif
}

ScreenPara::~ScreenPara()
{

}
