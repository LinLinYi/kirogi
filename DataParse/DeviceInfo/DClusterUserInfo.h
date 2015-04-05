#ifndef DCLUSTERUSERINFO_H
#define DCLUSTERUSERINFO_H

#include <QString>
#include "GlobalDefine.hpp"

/*! \class DClusterUserInfo
 *  \brief 用来保存与设备群相关联的用户信息类。
 */
class DClusterUserInfo : public QObject
{
public:
    explicit DClusterUserInfo(quint64 clusID = 0);
    virtual ~DClusterUserInfo();

public:
    quint16 unpack(char *data);
    void loadDataSource(quint64 clusID, quint64 uID);
    static void deleteUserInfo(quint64 clusID, quint64 userID);
    static bool isExistUserInfo(const quint64 clusID = 0, const quint64 userID = 0);

    quint64 getUserID() { return m_userID; }
    QString getUserName() { return m_userName; }
    quint64 getClusterID() { return m_clusterID; }
    Mi::AuthLevel getUserRole() { return m_userRole; }
    QString getRemarks() { return m_remarks; }
    QString getDescribe() { return m_describe; }

    void setUserRole(Mi::AuthLevel id) { m_userRole = id; }
    void setClusterID(quint64 id) { m_clusterID = id; }
    void setUserID(quint64 id) { m_userID = id; }
    void setUserName(QString& name) { m_userName = name; }
    void setRemarks(QString& remark) { m_remarks = remark; }
    void setDescribe(QString& describe) { m_describe = describe; }

private:
    Mi::AuthLevel m_userRole;
    quint64 m_clusterID;
    quint64 m_userID;
    QString m_userName;
    QString m_remarks;
    QString m_describe;
};

#endif // DCLUSTERUSERINFO_H
