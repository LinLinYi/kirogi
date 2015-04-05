#ifndef DCLUSTERNOTIFYLIST_H
#define DCLUSTERNOTIFYLIST_H

#include <QObject>
#include "DClusterNotifyInfo.h"

class DClusterNotifyInfoList : public QObject
{
public:
    explicit DClusterNotifyInfoList(QObject* parent = 0);
    virtual ~DClusterNotifyInfoList();

public:
    void append(char* data, MiCo::ClusterNotify type);
    void append(DClusterNotifyInfo* info);
    void loadDataSource();
    void removeAll();
    void remove(DClusterNotifyInfo* info);
    QList<DClusterNotifyInfo*>& fetchList();

private:
    QList<DClusterNotifyInfo*> infoList;

private:
    void insertList(DClusterNotifyInfo* info);
    void insertDB(DClusterNotifyInfo* info);
};

#endif // DCLUSTERNOTIFYINFOLIST_H
