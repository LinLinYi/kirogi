#ifndef USERAUTHORIZEDINFO_H
#define USERAUTHORIZEDINFO_H

#include "GlobalDefine.hpp"

class UserAuthorizedInfo : public QObject
{
    Q_OBJECT
public:
    explicit UserAuthorizedInfo(QObject* parent = 0);
    virtual ~UserAuthorizedInfo();

public:
    quint16 unpack(char *data);
    quint64 getUserID() { return m_userID; }
    Mi::AuthLevel  getUserRole() { return m_userRole; }
    QString getUserName() { return m_userName; }
    quint64 getClusterID() { return m_clusterID; }
    quint64 getDeviceID() { return m_deviceID; }

    void setClusterID(quint64 id) { m_clusterID = id; }
    void setDeviceID(quint64 id) { m_deviceID = id; }

private:
    quint64 m_clusterID;
    quint64 m_deviceID;
    Mi::AuthLevel  m_userRole;
    quint64 m_userID;
    QString m_userName;
};

#endif // USERAUTHORIZEDINFO_H
