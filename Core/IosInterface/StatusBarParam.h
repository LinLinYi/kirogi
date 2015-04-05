#ifndef STATUSBARPARAM_H
#define STATUSBARPARAM_H

#include <QString>

class StatusBarParam
{
public:
    StatusBarParam();
    ~StatusBarParam();

public:
    static quint16 StatusBarHeight();
    static quint16 StatusBarWidth();
    static void setStatusBar();
    static void setStatusBarShow();
};

#endif // STATUSBARPARAM_H
