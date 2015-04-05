#ifndef DCLUSTERUSERINFOLIST_H
#define DCLUSTERUSERINFOLIST_H

#include "GListWidget.h"
#include "DClusterUserInfo.h"

class DClusterUserInfoList
{
public:
    explicit DClusterUserInfoList(quint64 clusterID = 0);
    virtual ~DClusterUserInfoList();

public:
    QHash<quint64, DClusterUserInfo*>& fetchList();
    void appendList(char* data, quint16 dataLen,  Mi::SaveInfo type =  Mi::SaveAll);
    void append(char* data, quint16 dataLen);
    void append(DClusterUserInfo* info);
    void remove(quint64 removeID);
    void remove(DClusterUserInfo* info);
    void takeAt(DClusterUserInfo* info);
    void takeAt(quint64 clusID, quint64 userID);
    void loadDataSource();
    int count();

    static void deleteUsers(quint64 clusterID);

private:
    quint64 curClusterID;
    QHash<quint64, DClusterUserInfo*> infoList;

private:
    void insertList(DClusterUserInfo* info);
    void insertUser(DClusterUserInfo* info);
    void deleteUser(quint64 clusterID, quint64 userID);
};

#endif // DCLUSTERUSERINFOLIST_H
