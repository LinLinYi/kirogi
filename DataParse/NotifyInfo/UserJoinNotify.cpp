#include <QDateTime>
#include "DispersalData.h"
#include "UserJoinNotify.h"
#include "UserJoinNotifyDataSource.h"

UserJoinNotify::UserJoinNotify()
{

}

void UserJoinNotify::unpack(char *data, Mi::UserJoin type)
{
    if(data == NULL) return;

    int offset = 0;
    DispersalData dispersalData;
    if(type == Mi::UserJoinApproval){
        offset += dispersalData.outlet(m_msgResult, data + offset); //通知结果
    }

    offset += dispersalData.outlet(m_userID, data + offset);        //发送方ID
    offset += dispersalData.outlet(m_userName, data + offset);      //发送方名称
    offset += dispersalData.outlet(m_nickName, data + offset);      //昵称
    offset += dispersalData.outlet(m_signature, data + offset);     //签名

    if(type == Mi::UserJoinReq){
        dispersalData.outlet(m_msgVerify, data + offset);           //验证消息
    }

    m_msgStatus = Mi::Pending;
    m_msgTime = this->getCurrentTime();
}

void UserJoinNotify::updateInfoStatus(Mi::MsgStatus status)
{
    m_msgStatus = status;
    quint8 updateStatus = status;
    UserJoinNotifyDataSource::updateCheckStatus(m_userID, updateStatus);
}

UserJoinNotify::UserJoinNotify(const UserJoinNotify& info)
{
    m_userID = info.m_userID;
    m_userName = info.m_userName;
    m_msgVerify = info.m_msgVerify;
    m_msgTime = info.m_msgTime;
    m_msgContent = info.m_msgContent;
    m_msgStatus = info.m_msgStatus;
}

UserJoinNotify& UserJoinNotify::operator =(const UserJoinNotify& info)
{
    m_userID = info.m_userID;
    m_userName = info.m_userName;
    m_msgVerify = info.m_msgVerify;
    m_msgTime = info.m_msgTime;
    m_msgContent = info.m_msgContent;
    m_msgStatus = info.m_msgStatus;
    return *this;
}

void UserJoinNotify::saveToLocal()
{
    UserJoinNotifyDataField fields;
    fields.userID = m_userID;
    fields.userName = m_userName;
    fields.notifyVerify = m_msgVerify;
    fields.notifyTime = m_msgTime;
    fields.notifyContent = m_msgContent;
    fields.status = m_msgStatus;
    UserJoinNotifyDataSource::insertData(fields);
}

QString UserJoinNotify::getCurrentTime()
{
    QDateTime curDateTime = QDateTime::currentDateTime();
    QString curTime = curDateTime.toString("yyyy-MM-dd hh:mm");
    return curTime;
}

UserJoinNotify::~UserJoinNotify()
{

}

