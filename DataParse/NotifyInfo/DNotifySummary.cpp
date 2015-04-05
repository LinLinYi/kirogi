#include <QTime>
#include "DNotifySummary.h"
#include <DNotifySummaryDataSource.h>

DNotifySummary::DNotifySummary(QObject* parent) : QObject(parent)
{
    notifyID = 0;
    unReadCount = 0;
    notifyType = Mi::NotifyNone;
    notifyTime = getCurrentTime();
}

DNotifySummary::DNotifySummary(Mi::NotifyCategory type, QString &msgHint)
{
    notifyID = 0;
    unReadCount = 0;
    notifyType = type;
    notifyContent = msgHint;
    notifyTime = getCurrentTime();
}

DNotifySummary& DNotifySummary::operator=(const DNotifySummary& info)
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

void DNotifySummary::updateUnreadMsgCount(int count)
{
    unReadCount = count;
    DNotifySummaryDataSource::updateUnreadCount(notifyType, notifyID, unReadCount);
}

QString DNotifySummary::getCurrentTime()
{
    QDateTime curDateTime = QDateTime::currentDateTime();
    QString curTime = curDateTime.toString("yyyy-MM-dd hh:mm");
    return curTime;
}

DNotifySummary::~DNotifySummary()
{

}
