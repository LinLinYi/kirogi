#ifndef DCLUSTERAUTHDEVINFO_H
#define DCLUSTERAUTHDEVINFO_H

#include <QObject>
#include "GlobalDefine.hpp"

class DClusterAuthDevInfo : QObject
{
    Q_OBJECT
public:
    explicit DClusterAuthDevInfo(QObject* parent = 0);
    virtual ~DClusterAuthDevInfo();

public:
    quint16 unpack(char *data);
    quint64 getUserID() { return m_userID; }
    quint64 getDeviceID() { return m_deviceID; }
    QString getDeviceName() { return m_deviceName; }
    Mi::AuthLevel  getUserRole() { return m_userRole; }
    quint64 getClusterID() { return m_clusterID; }

    void setUserID(quint64 id) { m_userID = id; }
    void setClusterID(quint64 id) { m_clusterID = id; }

private:
    quint64 m_clusterID;
    Mi::AuthLevel m_userRole;
    quint64 m_userID;
    quint64 m_deviceID;
    QString m_deviceName;
};

#endif // DCLUSTERAUTHDEVINFO_H
