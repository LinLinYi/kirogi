#include <QtGlobal>
#include "StatusBarParam.h"

#include <QDebug>
#ifdef Q_OS_IOS
    #import <UIKit/UIKit.h>
    #import <Foundation/Foundation.h>
#endif

StatusBarParam::StatusBarParam(){}
StatusBarParam::~StatusBarParam(){}

quint16 StatusBarParam::StatusBarHeight()
{
#ifdef Q_OS_IOS
    CGRect rect  = [[UIApplication sharedApplication] statusBarFrame];
    return (quint16)(rect.size.height);
#elif Q_OS_ANDROID
    return 20;
#else
    return 20;
#endif
}

quint16 StatusBarParam::StatusBarWidth()
{
#ifdef Q_OS_IOS
    CGRect rect  = [[UIApplication sharedApplication] statusBarFrame];
    return (quint16)(rect.size.width);
#elif Q_ANDROID
    return 320;
#else
    return 320;
#endif
}

void StatusBarParam::setStatusBar()
{

}

void StatusBarParam::setStatusBarShow()
{

}
