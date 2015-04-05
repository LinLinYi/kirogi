#ifndef USERJOINNOTIFY_H
#define USERJOINNOTIFY_H

#include <QObject>
#include "GlobalDefine.hpp"

class UserJoinNotify
{
public:
    explicit UserJoinNotify();
    UserJoinNotify(const UserJoinNotify& info);
    UserJoinNotify& operator =(const UserJoinNotify& info);
    virtual ~UserJoinNotify();

public:
    void unpack(char* data, Mi::UserJoin type);
    void updateInfoStatus(Mi::MsgStatus status);
    void saveToLocal();

    void setUserID(const quint64 id) { m_userID = id; }
    void setUserName(const QString& name) { m_userName = name; }
    void setNotifyContent(const QString& hint) { m_msgContent = hint; }
    void setNotifyTime(const QString& time) { m_msgTime = time; }
    void setNotifyStatus(const Mi::MsgStatus status) { m_msgStatus = status; }
    void setNotifyVerify(const QString& verify) { m_msgVerify = verify; }

    quint64 getUserID() { return m_userID; }
    QString getUserName() { return m_userName; }
    QString& getNotifyContent() { return m_msgContent; }
    QString& getMsgVerify() { return m_msgVerify; }
    QString& getNotifyTime() { return m_msgTime; }
    Mi::MsgStatus getMsgStatus() { return m_msgStatus; }
    quint8 getMsgResult() { return m_msgResult; }

private:
    quint64 m_userID;
    QString m_userName;
    QString m_nickName;
    QString m_signature;
    QString m_msgVerify;
    QString m_msgTime;
    QString m_msgContent;
    quint8 m_msgResult;
    Mi::MsgStatus m_msgStatus;

private:
    QString getCurrentTime();

};

#endif // USERJOINNOTIFY_H
