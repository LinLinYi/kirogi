#ifndef DCLUSTERINFOLIST_H
#define DCLUSTERINFOLIST_H

#include <QObject>
#include "DClusterInfo.h"

class DClusterInfoList : public QObject
{
    Q_OBJECT
public:
    explicit DClusterInfoList(QObject* parent = 0);
    virtual ~DClusterInfoList();

public:
    QHash<quint64, DClusterInfo*>& fetchList();
    void appendList(char* data, quint16 dataLen,  Mi::SaveInfo type =  Mi::SaveAll);
    void append(char* data, quint16 dataLen);
    void append(DClusterInfo* info);
    void remove(quint64 removeID);
    void remove(DClusterInfo* info);
    void loadDataSource();
    int count();

private:
    QHash<quint64, DClusterInfo*> infoList;

private:
    void insertList(DClusterInfo* info);
    void insertDB(DClusterInfo* info);
};

#endif // DCLUSTERINFOLIST_H
