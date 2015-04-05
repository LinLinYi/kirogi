#ifndef DCLUSTERDEVINFOLIST_H
#define DCLUSTERDEVINFOLIST_H

#include "GlobalDefine.hpp"
#include "DClusterDevInfo.h"

class DClusterDevInfoList
{
public:
    explicit DClusterDevInfoList(quint64 clusterID = 0);
    virtual ~DClusterDevInfoList();

public:
    QHash<quint64, DClusterDevInfo*>& fetchList();
    void appendList(char* data, quint16 dataLen,  Mi::SaveInfo type =  Mi::SaveAll);
    void append(char* data, quint16 dataLen);
    void append(DClusterDevInfo* info);
    void remove(quint64 removeID);
    void remove(DClusterDevInfo* info);
    void loadDataSource();
    int count();

    static void deleteDevices(quint64 clusterID);

private:
    quint64 curClusterID;
    QHash<quint64, DClusterDevInfo*> infoList;

private:
    void insertList(DClusterDevInfo* info);
    void insertDB(DClusterDevInfo* info);
};

#endif // DCLUSTERDEVINFOLIST_H
