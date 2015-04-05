#ifndef DEVICETRANSFERINFOLIST_H
#define DEVICETRANSFERINFOLIST_H

#include <QHash>
#include "DeviceTransferInfo.h"

class DeviceTransferInfoList : public QObject
{
    Q_OBJECT
public:
    explicit DeviceTransferInfoList(QObject* parent = 0);
    virtual ~DeviceTransferInfoList();

public:
    void loadDataSource(quint64 srcClusterID, quint64 destClusterID);
    void appendList(char* data);
    QHash<quint64, DeviceTransferInfo*>& fetchList();
    int count();
    void remove(quint64 removeID);
    static void deleteLocalTransferDevices(quint64 srcClusterID, quint64 destClusterID);

public:
    void setSrcUserID(quint64 id) { srcUserID = id; }
    void setSrcUserName(QString name) { srcUserName = name; }
    void setSrcClusterID(quint64 id) { srcClusterID = id; }
    void setScrClusterName(QString name) { srcClusterName = name; }
    void setDestClusterID(quint64 id) { destClusterID = id; }
    void setDestClusterName(QString name) { destClusterName = name; }

private:
    quint64 srcUserID;
    QString srcUserName;
    quint64 srcClusterID;
    QString srcClusterName;
    quint64 destClusterID;
    QString destClusterName;
    QHash<quint64, DeviceTransferInfo*> infoList;

private:
    void insertList(DeviceTransferInfo* info);
    void insertDB(DeviceTransferInfo* info);
};


#endif // DEVICETRANSFERINFOLIST_H
