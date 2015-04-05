#ifndef DCLUSTERAUTHDEVINFOLIST_H
#define DCLUSTERAUTHDEVINFOLIST_H

#include "DClusterAuthDevInfo.h"

class DClusterAuthDevInfoList : public QObject
{
    Q_OBJECT
public:
    explicit DClusterAuthDevInfoList(QObject* parent = 0);
    virtual ~DClusterAuthDevInfoList();

public:
    void appendList(char* data, quint16 dataLen,  Mi::SaveInfo type =  Mi::SaveAll);
    void append(char* data, quint16 dataLen);
    void append(DClusterAuthDevInfo* info);
    void remove(quint64 removeID);
    void remove(DClusterAuthDevInfo* info);
    QHash<quint64, DClusterAuthDevInfo*>& fetchList();

private:
    QHash<quint64, DClusterAuthDevInfo*> infoList;
};

#endif // DCLUSTERAUTHDEVINFOLIST_H
