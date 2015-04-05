#ifndef USERINFO_H
#define USERINFO_H

#include <QObject>
#include "GlobalDefine.hpp"

class UserInfo
{
public:
    explicit UserInfo(quint64 userID = 0);
    virtual ~UserInfo();

public:
    void loadDataSource(quint64 userID = 0);
    quint16 unpack(char *data);
    quint64 getUserID() { return m_userID; }
    QString& getUserName() { return m_userName; }
    QString& getRemarks(){ return m_userRemarks; }
    QString& getDescribe(){ return m_userDescribe; }
    QString& getUserMail() { return m_userMail; }
    QString& getUserProtrait() { return m_userPortrait; }
    Mi::NetStatus getStatus(){ return m_userStatus; }

    void setUserID(quint64 id) { m_userID = id; }
    void setUserName(const QString& name) { m_userName = name; }
    void setRemarks(const QString& nickname) { m_userRemarks = nickname; }
    void setDescribe(const QString& describe) { m_userDescribe = describe; }
    void setUserMail(const QString& mail) { m_userDescribe = mail; }
    void setUserStatus(Mi::NetStatus status) { m_userStatus = status; }

    void updateRemarks(const QString& remarks, quint64 userID = 0);
    void updateDescribe(const QString& describe, quint64 userID = 0);
    void updateHeadPortrait(const QString& portrait, quint64 userID = 0);
    bool isExistLocalDataSouce(const quint64 userID = 0);

private:
    quint64 m_userID;
    QString m_userName;
    QString m_userRemarks;
    QString m_userDescribe;
    QString m_userMail;
    QString m_userPortrait;
    Mi::NetStatus  m_userStatus;

private:
    QString ipAddrFormat(char* ipData, quint16 dataLen);
};

#endif // USERINFO_H

