#include <QTime>
#include "UNotifySummary.h"
#include "UNotifySummaryDataSource.h"

UNotifySummary::UNotifySummary(QObject* parent) : QObject(parent)
{
    notifyID = 0;
    unReadCount = 0;
    notifyType = Mi::NotifyNone;
    notifyTime = getCurrentTime();
}

UNotifySummary::UNotifySummary(Mi::NotifyCategory type, QString &msgHint)
{
    notifyID = 0;
    unReadCount = 0;
    notifyType = type;
    notifyContent = msgHint;
    notifyTime = getCurrentTime();
}

UNotifySummary& UNotifySummary::operator=(const UNotifySummary& info)
{
    notifyType = info.notifyType;
    notifyName = info.notifyName;
    notifyTime = info.notifyTime;
    notifyContent = info.notifyContent;
    unReadCount = info.unReadCount;
    notifyImg = info.notifyImg;
    notifyID = info.notifyID;
    return *this;
}

void UNotifySummary::updateUnreadMsgCount(int count)
{
    unReadCount = count;
    UNotifySummaryDataSource::updateUnreadCount(notifyType, notifyID, unReadCount);
}

QString UNotifySummary::getCurrentTime()
{
    QDateTime curDateTime = QDateTime::currentDateTime();
    QString curTime = curDateTime.toString("yyyy-MM-dd hh:mm");
    return curTime;
}

UNotifySummary::~UNotifySummary()
{

}
