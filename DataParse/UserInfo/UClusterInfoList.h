#ifndef UCLUSTERINFOLIST_H
#define UCLUSTERINFOLIST_H

#include <QObject>
#include "UClusterInfo.h"

class UClusterInfoList : public QObject
{
    Q_OBJECT
public:
    explicit UClusterInfoList(QObject* parent = 0);
    virtual ~UClusterInfoList();

public:
    QHash<quint64, UClusterInfo*>& fetchList();
    void appendList(char* data, quint16 dataLen,  Mi::SaveInfo type =  Mi::SaveAll);
    void append(char* data, quint16 dataLen);
    void append(UClusterInfo* info);
    void remove(quint64 removeID);
    void remove(UClusterInfo* info);
    void loadDataSource();
    int count();

private:
    QHash<quint64, UClusterInfo*> infoList;

private:
    void insertList(UClusterInfo* info);
    void insertDB(UClusterInfo* info);
};


#endif // UCLUSTERINFOLIST_H
