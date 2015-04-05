#ifndef MYDEVICEINFOLIST_H
#define MYDEVICEINFOLIST_H

#include <QHash>
#include "MyDeviceInfo.h"
#include "GlobalDefine.hpp"

class MyDeviceInfoList : public QObject
{
    Q_OBJECT
public:
    explicit MyDeviceInfoList(QObject* parent = 0);
    virtual ~MyDeviceInfoList();

public:
    void loadDataSource();
    void appendList(char* data, quint16 dataLen, Mi::SaveInfo type =  Mi::SaveAll);
    void append(char* data, quint16 dataLen);
    void append(MyDeviceInfo* info);
    void remove(quint64 removeID);
    void remove(MyDeviceInfo* info);
    void deleteDevices(quint64 clusterID);
    QHash<quint64, MyDeviceInfo*>& fetchList();
    int count();

private:
     QHash<quint64, MyDeviceInfo*> infoList;

private:
    void insertList(MyDeviceInfo* info);
    void insertDB(MyDeviceInfo* info);
};

#endif // MYDEVICEINFOLIST_H
