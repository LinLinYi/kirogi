#ifndef UCLUSTERNOTIFYINFO_H
#define UCLUSTERNOTIFYINFO_H

#include <QObject>
#include "GlobalDefine.hpp"

class UClusterNotifyInfo : public QObject
{
public:
    explicit UClusterNotifyInfo(QObject* parent = 0);
    UClusterNotifyInfo(const UClusterNotifyInfo& info);
    UClusterNotifyInfo& operator =(const UClusterNotifyInfo& info);
    virtual ~UClusterNotifyInfo();

public:
    void unpack(char* data, MiCo::ClusterNotify type);
    void updateInfoStatus(MiCo::MsgStatus status);
    void saveToLocal();

    void setUserID(const quint64 id) { m_userID = id; }
    void setUserName(const QString& name) { m_userName = name; }
    void setClusterID(const quint64 id) { m_clusterID = id; }
    void setClusterName(const QString& name) { m_clusterName = name; }
    void setNotifyContent(const QString& hint) { m_msgContent = hint; }
    void setNotifyTime(const QString& time) { m_msgTime = time; }
    void setMsgStatus(const MiCo::MsgStatus status) { m_msgStatus = status; }
    void setMsgType(const MiCo::ClusterNotify type) { m_msgType = type; }
    void setMsgVerify(const QString& verify) { m_msgVerify = verify; }

    quint64 getUserID() { return m_userID; }
    QString getUserName() { return m_userName; }
    quint64 getClusterID() { return m_clusterID; }
    QString& getClusterName() { return m_clusterName; }
    QString& getNotifyContent() { return m_msgContent; }
    QString& getMsgVerify() { return m_msgVerify; }
    QString& getNotifyTime() { return m_msgTime; }
    MiCo::MsgStatus getMsgStatus() { return m_msgStatus; }
    MiCo::ClusterNotify getNotifyType() { return m_msgType; }

private:
    quint64 m_userID;
    QString m_userName;
    quint64 m_clusterID;
    QString m_clusterName;
    QString m_msgVerify;
    QString m_msgTime;
    QString m_msgContent;
    quint8 m_msgResult;
    MiCo::MsgStatus m_msgStatus;
    MiCo::ClusterNotify m_msgType;

private:
    QString getCurrentTime();
};
#endif // UCLUSTERNOTIFYINFO_H
