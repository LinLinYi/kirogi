#ifndef DCLUSTERJOINNOTIFYLIST_H
#define DCLUSTERJOINNOTIFYLIST_H

#include <QObject>
#include "DClusterJoinNotify.h"

class DClusterJoinNotifyList : public QObject
{
public:
    explicit DClusterJoinNotifyList(QObject* parent = 0);
    virtual ~DClusterJoinNotifyList();

public:
    void append(char* data, Mi::ClusterJoin type);
    void append(DClusterJoinNotify* info);
    void loadDataSource();
    void remove(DClusterJoinNotify* info);
    QList<DClusterJoinNotify*>& fetchList();

private:
    QList<DClusterJoinNotify*> infoList;

private:
    void insertList(DClusterJoinNotify* info);
    void insertDB(DClusterJoinNotify* info);
};

#endif // DCLUSTERJOINNOTIFYLIST_H
