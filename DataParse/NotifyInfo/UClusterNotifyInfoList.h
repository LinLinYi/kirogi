#ifndef UCLUSTERNOTIFYINFOLIST_H
#define UCLUSTERNOTIFYINFOLIST_H

#include <QObject>
#include "UClusterNotifyInfo.h"

class UClusterNotifyInfoList : public QObject
{
public:
    explicit UClusterNotifyInfoList(QObject* parent = 0);
    virtual ~UClusterNotifyInfoList();

public:
    void append(char* data, MiCo::ClusterNotify type);
    void append(UClusterNotifyInfo* info);
    void loadDataSource();
    void removeAll();
    void remove(UClusterNotifyInfo* info);
    QList<UClusterNotifyInfo*>& fetchList();

private:
    QList<UClusterNotifyInfo*> infoList;

private:
    void insertList(UClusterNotifyInfo* info);
    void insertDB(UClusterNotifyInfo* info);
};

#endif // UCLUSTERNOTIFYINFOLIST_H
