#ifndef DCLUSTERINFO_H
#define DCLUSTERINFO_H

#include <QObject>
#include "GlobalDefine.hpp"

class DClusterInfo
{
public:
    explicit DClusterInfo(quint64 clusterID = 0);
    DClusterInfo(const DClusterInfo& info);
    DClusterInfo& operator =(const DClusterInfo& info);
    virtual ~DClusterInfo();

public:
    quint16 unpack(char *data);
    void loadDataSource(quint64 clusID = 0);

    void updateRemarks(const QString& remarks);
    void updateDescribe(const QString& describe);
    static bool isExistClusterInfo(quint64 clusID);
    static bool isExistClusterInfo(QString& clusName);

    quint64 getClusterID(){ return m_clusterID; }
    QString getClusterName(){ return m_clusterName; }
    Mi::AuthLevel getUserRole() { return m_userRole; }
    QString getRemarks() { return m_clusterRemarks; }
    QString getDescribe() { return m_clusterDescribe; }
    quint8 getGroupID() { return m_clusterGroupID; }

    void setClusterID(const quint64 id) { m_clusterID = id; }
    void setClusterName(const QString& name) { m_clusterName = name; }
    void setUserRole(const Mi::AuthLevel role) { m_userRole = role; }
    void setRemarks(const QString& remarks) { m_clusterRemarks = remarks; }
    void setDescribe(const QString& describe) { m_clusterDescribe = describe; }
    void setGroupID(const quint64 id) { m_clusterGroupID = id; }

private:
    Mi::AuthLevel m_userRole;  //用户角色
    quint64 m_clusterID;       //群ID
    QString m_clusterName;     //群名称
    QString m_clusterRemarks;  //群备注
    quint8 m_clusterGroupID;   //群分组ID
    QString m_clusterDescribe; //群介绍
};

#endif // DCLUSTERINFO_H
