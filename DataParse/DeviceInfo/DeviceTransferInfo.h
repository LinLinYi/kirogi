#ifndef DEVICETRANSFERINFO_H
#define DEVICETRANSFERINFO_H

#include <QObject>

class DeviceTransferInfo : public QObject
{
    Q_OBJECT
public:
    explicit DeviceTransferInfo(quint64 dev = 0);
    virtual ~DeviceTransferInfo();

public:
    quint16 unpack(char *data);
    void loadDataSource(quint64 dev = 0);

    void setTransUserID(quint64 id) { m_userID = id; }
    void setTransUserName(QString name) { m_userName = name; }
    void setSrcClusterID(quint64 id) { m_srcClusterID = id; }
    void setSrcClusterName(QString name) { m_srcClusterName = name; }
    void setDestClusterID(quint64 id) { m_destClusterID = id; }
    void setDestClusterName(QString name) { m_destClusterName = name; }
    void setDeviceID(quint64 id) { m_deviceID = id; }
    void setDeviceName(QString name) { m_deviceName = name; }

    quint64 getTransUserID() { return m_userID; }
    QString& getTransUserName() { return m_userName; }
    quint64 getSrcClusterID() { return m_srcClusterID; }
    QString& getSrcClusterName() { return m_srcClusterName; }
    quint64 getDestClusterID() { return m_destClusterID; }
    QString& getDestClusterName() { return m_destClusterName; }
    quint64 getDeviceID() { return m_deviceID; }
    QString& getDeviceName() { return m_deviceName; }

private:
    quint64 m_userID;
    QString m_userName;
    quint64 m_srcClusterID;
    QString m_srcClusterName;
    quint64 m_destClusterID;
    QString m_destClusterName;
    quint64 m_deviceID;
    QString m_deviceName;
};

#endif // DEVICETRANSFERINFO_H
