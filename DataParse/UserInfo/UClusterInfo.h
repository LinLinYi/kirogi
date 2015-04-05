#ifndef UCLUSTERINFO_H
#define UCLUSTERINFO_H

#include <QObject>
#include "GlobalDefine.hpp"

class UClusterInfo
{
public:
    explicit UClusterInfo(quint64 clusterID = 0);
    UClusterInfo(const UClusterInfo& info);
    UClusterInfo& operator =(const UClusterInfo& info);
    virtual ~UClusterInfo();

public:
    quint16 unpack(char *data);
    void loadDataSource(quint64 clusID = 0);

    void updateRemarks(const QString& remarks);
    void updateDescribe(const QString& describe);
    static bool isExistClusterInfo(quint64 clusID);

    quint64 getClusterID(){ return m_clusterID; }
    QString getClusterName(){ return m_clusterName; }
    Mi::AuthLevel getUserRole() { return m_userRole; }
    QString getDescribe() { return m_clusterDescribe; }

    void setClusterID(const quint64 id) { m_clusterID = id; }
    void setClusterName(const QString& name) { m_clusterName = name; }
    void setUserRole(const Mi::AuthLevel role) { m_userRole = role; }
    void setDescribe(const QString& describe) { m_clusterDescribe = describe; }

private:
    Mi::AuthLevel m_userRole;  //用户角色
    quint64 m_clusterID;       //群ID
    QString m_clusterName;     //群名称
    QString m_clusterDescribe; //群介绍
};

#endif // UCLUSTERINFO_H
