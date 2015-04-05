#include "UserInfo.h"
#include "UserDataSource.h"
#include "DispersalData.h"

UserInfo::UserInfo(quint64 userID) : m_userID(userID){}
quint16 UserInfo::unpack(char *data)
{
    if(data == NULL) return 0;

    int offset = 0;
    DispersalData dispersalData;
    offset += dispersalData.outlet(m_userID, data + offset);         //用户ID
    offset += dispersalData.outlet(m_userName, data + offset);       //用户名称
    offset += dispersalData.outlet(m_userRemarks, data + offset);    //用户备注
    offset += dispersalData.outlet(m_userDescribe, data + offset);   //用户描述
    return offset;
}

void UserInfo::loadDataSource(quint64 userID)
{
    if(userID == 0) userID = m_userID;

    CppSQLite3Query src;
    UserDataSource::queryData(src, userID);

    while(!src.eof()){
        UserDataField dest;
        UserDataSource::parseDataField(src, dest);

        m_userID = dest.userID;
        m_userName = dest.userName;
        m_userRemarks = dest.remarks;
        m_userDescribe = dest.describe;
        m_userMail = dest.userMail;
        m_userPortrait = dest.userPortrait;
        src.nextRow();
    }
}

void UserInfo::updateRemarks(const QString &remarks, quint64 userID)
{
    if(userID != 0) m_userID = userID;
    m_userRemarks = remarks;
    UserDataSource::updateRemarks(remarks, m_userID);
}

void UserInfo::updateDescribe(const QString &describe, quint64 userID)
{
    if(userID != 0) m_userID = userID;
    m_userDescribe = describe;
    UserDataSource::updateDescribe(describe, m_userID);
}

void UserInfo::updateHeadPortrait(const QString &portrait, quint64 userID)
{
    if(userID != 0) m_userID = userID;
    m_userPortrait = portrait;
    UserDataSource::updateHeadPortrait(portrait, m_userID);
}

bool UserInfo::isExistLocalDataSouce(const quint64 userID)
{
    quint64 uID = userID;
    if(uID == 0)
        uID = m_userID;

    CppSQLite3Query outSqlData;
    UserDataSource::queryData(outSqlData, uID);

    if(outSqlData.eof())
        return false;
    else
        return true;
}

QString UserInfo::ipAddrFormat(char* ipData, quint16 dataLen)
{
    int i;
    QString strIP;
    for(i = 0; i < dataLen; i++){
        quint8 byteInt = ipData[i];
        Q_ASSERT(byteInt >= 0 && byteInt <= 255);

        strIP += QObject::tr("%1").arg(byteInt);
        if(i != dataLen - 1)
            strIP += ".";
    }

    return strIP;
}

UserInfo::~UserInfo(){}
