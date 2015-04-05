#ifndef MYDEVICEINFO_H
#define MYDEVICEINFO_H

#include <QObject>

class MyDeviceInfo : public QObject
{
    Q_OBJECT
public:
    explicit MyDeviceInfo(quint64 = 0, QObject* parent = 0);
    virtual ~MyDeviceInfo();

public:
    quint16 unpack(char *data);
    void loadDataSource(quint64 devID = 0);

    quint64 getClusterID() { return m_clusterID; }
    quint64 getDeviceID() { return m_deviceID; }
    QString& getDeviceName() { return m_deviceName; }
    QString& getDeviceMac() { return m_deviceMac; }
    QString& getDescribe() { return m_discribe; }

    void setClusterID(quint64 id) { m_clusterID = id; }
    void setDeviceID(quint64 id) { m_deviceID  =  id; }
    void setDeviceName(QString name) { m_deviceName = name; }
    void setDeviceMac(QString mac) { m_deviceMac = mac; }

private:
    quint64 m_clusterID;
    quint64 m_deviceID;
    QString m_deviceMac;
    QString m_deviceName;
    QString m_discribe;

private:
    quint64 macToID(char *data);
    QString ipAddrFormat(char* data);
    QString macToString(char* data);
};

#endif // MYDEVICEINFO_H
