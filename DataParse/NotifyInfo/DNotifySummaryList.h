#ifndef DNOTIFYSUMMARYLIST_H
#define DNOTIFYSUMMARYLIST_H

#include <QObject>
#include "DNotifySummary.h"

class DNotifySummaryList : public QObject
{
public:
    explicit DNotifySummaryList(QObject* parent = 0);
    virtual ~DNotifySummaryList();

public:
    QList<DNotifySummary*>& fetchList();
    void append(DNotifySummary* info);
    void loadDataSource();
    void remove(Mi::NotifyCategory type, quint64 notifyID);
    int getTotalUnreadCount();

private:
    int curTotalUnreadMsg;
    QList<DNotifySummary*> infoList;

private:
    void insertList(DNotifySummary* info);
    void insertDB(DNotifySummary* info);
};

#endif // DNOTIFYSUMMARYLIST_H
