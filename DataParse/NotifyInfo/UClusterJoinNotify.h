#ifndef UCLUSTERJOINNOTIFY_H
#define UCLUSTERJOINNOTIFY_H

#include <QObject>
#include "GlobalDefine.hpp"

class UClusterJoinNotify : public QObject
{
public:
    explicit UClusterJoinNotify(QObject* parent = 0);
    UClusterJoinNotify(const UClusterJoinNotify& info);
    UClusterJoinNotify& operator =(const UClusterJoinNotify& info);
    virtual ~UClusterJoinNotify();

public:
    void unpack(char* data, Mi::ClusterJoin type);
    void updateInfoStatus(Mi::MsgStatus status);
    void saveToLocal();

    void setUserID(const quint64 id) { m_userID = id; }
    void setUserName(const QString& name) { m_userName = name; }
    void setClusterID(const quint64 id) { m_clusterID = id; }
    void setClusterName(const QString& name) { m_clusterName = name; }
    void setNotifyContent(const QString& hint) { m_msgContent = hint; }
    void setNotifyTime(const QString& time) { m_msgTime = time; }
    void setNotifyStatus(const Mi::MsgStatus status) { m_msgStatus = status; }
    void setNotifyType(const Mi::ClusterJoin type) { m_msgType = type; }
    void setNotifyVerify(const QString& verify) { m_msgVerify = verify; }

    quint64 getUserID() { return m_userID; }
    QString getUserName() { return m_userName; }
    quint64 getClusterID() { return m_clusterID; }
    QString& getClusterName() { return m_clusterName; }
    QString& getNotifyContent() { return m_msgContent; }
    QString& getMsgVerify() { return m_msgVerify; }
    QString& getNotifyTime() { return m_msgTime; }
    quint8 getMsgResult() { return m_msgResult; }
    Mi::MsgStatus getMsgStatus() { return m_msgStatus; }
    Mi::ClusterJoin getNotifyType() { return m_msgType; }

private:
    quint64 m_userID;
    QString m_userName;
    quint64 m_clusterID;
    QString m_clusterName;
    QString m_msgVerify;
    QString m_msgTime;
    QString m_msgContent;
    quint8 m_msgResult;
    Mi::MsgStatus m_msgStatus;
    Mi::ClusterJoin m_msgType;

private:
    QString getCurrentTime();
};
#endif // UCLUSTERJOINNOTIFY_H
