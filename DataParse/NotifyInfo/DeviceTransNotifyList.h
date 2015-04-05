#ifndef DEVICETRANSNOTIFYLIST_H
#define DEVICETRANSNOTIFYLIST_H

#include <QObject>
#include "DeviceTransNotify.h"

class DeviceTransNotifyList : public QObject
{
public:
    explicit DeviceTransNotifyList(QObject* parent = 0);
    virtual ~DeviceTransNotifyList();

public:
    void loadDataSource();
    void append(char* data);
    void append(DeviceTransNotify* info);
    void removeAll();
    void remove(DeviceTransNotify *info);
    QList<DeviceTransNotify*> fetchList();

private:
    QList<DeviceTransNotify*> infoList;

private:
    void insertList(DeviceTransNotify* info);
    void insertDB(DeviceTransNotify* info);
};

#endif // DEVICETRANSNOTIFYLIST_H
