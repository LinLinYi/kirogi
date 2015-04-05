#ifndef UNOTIFYSUMMARYLIST_H
#define UNOTIFYSUMMARYLIST_H

#include <QObject>
#include "UNotifySummary.h"

class UNotifySummaryList : public QObject
{
public:
    explicit UNotifySummaryList(QObject* parent = 0);
    virtual ~UNotifySummaryList();

public:
    void loadDataSource();
    QList<UNotifySummary*>& fetchList();
    void append(UNotifySummary* info);
    void remove(Mi::NotifyCategory type, quint64 notifyID);
    int getTotalUnreadCount();

private:
    int curTotalUnreadMsg;
    QList<UNotifySummary*> infoList;

private:
    void insertList(UNotifySummary* info);
    void updateList(UNotifySummary* existInfo, UNotifySummary* newInfo);
    void insertDB(UNotifySummary* info);
    void updateDB(UNotifySummary* info);
};

#endif // UNOTIFYSUMMARYLIST_H
