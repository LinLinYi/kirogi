#ifndef DCLUSTERDEVINFO_H
#define DCLUSTERDEVINFO_H

#include <QObject>
#include "DClusterDeviceDataSource.h"

class DClusterDevInfo : public QObject
{
    Q_OBJECT
public:
    explicit DClusterDevInfo(quint64 clusID = 0, quint64 devID = 0, QObject* parent = 0);
    virtual ~DClusterDevInfo();

public:
    quint16 unpack(char *data);
    void loadDataSource(quint64 clusID = 0, quint64 devID = 0);
    void updateRemarks(const QString& remarks);
    void updateDescribe(const QString& describe);

    quint64 getDeviceID(){ return m_deviceID; }
    QString getDeviceName() { return m_deviceName; }
    quint64 getClusterID() { return m_clusterID; }
    QString getDeviceMac() { return m_deviceMac; }
    QString getDescribe() { return m_describe; }
    QString getRemarks() { return m_remarks; }

    void setDeviceID(quint64 id) { m_deviceID = id; }
    void setClusterID(quint64 id) { m_clusterID = id; }
    void setDeviceName(QString& name) { m_deviceName = name; }
    void setDeviceMac(QString& mac) { m_deviceMac = mac; }
    void setRemarks(QString& remarks) { m_remarks = remarks; }
    void setDescribe(QString& describe) { m_describe = describe; }

private:
    quint64 m_clusterID;
    quint64 m_deviceID;
    QString m_deviceMac;
    QString m_deviceName;
    QString m_describe;
    QString m_remarks;
//    quint32 m_ipAddr;
//    quint16 m_port;

private:
    QString macToString(char* data);
};

#endif // DCLUSTERDEVINFO_H
