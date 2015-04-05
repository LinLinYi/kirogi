#ifndef ONLINESTATUSINFO_H
#define ONLINESTATUSINFO_H

#include <QObject>
#include "GlobalDefine.hpp"

class OnlineStatusInfo : public QObject
{
    Q_OBJECT
public:
    explicit OnlineStatusInfo(QObject *parent = 0);
    virtual ~OnlineStatusInfo() { }

public:
    quint16 unpack(char* data);
    quint64 getID() { return m_ID; }
    quint64 getClusterID() { return m_clusterID; }
    Mi::NetStatus  getStatus() { return m_onLineStatus; }
    void setClusterID(quint64 id) { m_clusterID = id; }

private:
    quint64 m_ID;
    quint64 m_clusterID;
    Mi::NetStatus  m_onLineStatus;
};

#endif // ONLINESTATUSINFO_H
