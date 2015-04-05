#ifndef UCLUSTERJOINNOTIFYLIST_H
#define UCLUSTERJOINNOTIFYLIST_H

#include <QObject>
#include "UClusterJoinNotify.h"

class UClusterJoinNotifyList : public QObject
{
public:
    explicit UClusterJoinNotifyList(QObject* parent = 0);
    virtual ~UClusterJoinNotifyList();

public:
    void append(char* data, Mi::ClusterJoin type);
    void append(UClusterJoinNotify* info);
    void loadDataSource();
    void remove(UClusterJoinNotify* info);
    QList<UClusterJoinNotify*>& fetchList();

private:
    QList<UClusterJoinNotify*> infoList;

private:
    void insertList(UClusterJoinNotify* info);
    void insertDB(UClusterJoinNotify* info);
};

#endif // UCLUSTERJOINNOTIFYLIST_H
