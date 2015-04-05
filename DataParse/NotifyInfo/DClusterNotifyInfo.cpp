#include <QTime>
#include "DClusterNotifyInfo.h"
#include "DispersalData.h"
#include "DClusterNotifyDataSource.h"

#if defined Q_OS_WIN32 || defined Q_OS_WIN64 || defined Q_OS_WIN
    using namespace std;
    # pragma execution_character_set("utf-8")
#endif

DClusterNotifyInfo::DClusterNotifyInfo(QObject* parent) : QObject(parent){}

void DClusterNotifyInfo::unpack(char *data, MiCo::ClusterNotify notify)
{
    if(data == NULL) return;

    int offset = 0;
    DispersalData dispersalData;
    offset += dispersalData.outlet(m_msgResult, data + offset);   //通知结果
    offset += dispersalData.outlet(m_clusterID, data + offset);   //群ID
    offset += dispersalData.outlet(m_clusterName, data + offset); //群名称
    offset += dispersalData.outlet(m_userID, data + offset);      //发送方ID
    offset += dispersalData.outlet(m_userName, data + offset);    //发送方名称
    dispersalData.outlet(m_msgVerify, data + offset);             //验证消息

    m_msgType = notify;
    m_msgStatus = MiCo::IsUncheck;
    m_msgTime = this->getCurrentTime();
}

DClusterNotifyInfo::DClusterNotifyInfo(const DClusterNotifyInfo& info)
{
    m_msgType = info.m_msgType;
    m_userID = info.m_userID;
    m_userName = info.m_userName;
    m_clusterID = info.m_clusterID;
    m_clusterName = info.m_clusterName;
    m_msgVerify = info.m_msgVerify;
    m_msgTime = info.m_msgTime;
    m_msgContent = info.m_msgContent;
    m_msgStatus = info.m_msgStatus;
    m_msgResult = info.m_msgResult;
}

DClusterNotifyInfo& DClusterNotifyInfo::operator =(const DClusterNotifyInfo& info)
{
    m_msgType = info.m_msgType;
    m_userID = info.m_userID;
    m_userName = info.m_userName;
    m_clusterID = info.m_clusterID;
    m_clusterName = info.m_clusterName;
    m_msgVerify = info.m_msgVerify;
    m_msgTime = info.m_msgTime;
    m_msgContent = info.m_msgContent;
    m_msgStatus = info.m_msgStatus;
    m_msgResult = info.m_msgResult;
    return *this;
}

void DClusterNotifyInfo::updateInfoStatus(MiCo::MsgStatus status)
{
    m_msgStatus = status;
    DClusterNotifyDataSource::updateCheckStatus(m_msgType, m_userID, m_clusterID, m_msgStatus);
}

void DClusterNotifyInfo::saveToLocal()
{
    DClusterNotifyDataField fields;
    fields.notifyType = m_msgType;
    fields.userID = m_userID;
    fields.userName = m_userName;
    fields.clusterID = m_clusterID;
    fields.clusterName = m_clusterName;
    fields.msgVerify = m_msgVerify;
    fields.notifyTime = m_msgTime;
    fields.notifyContent = m_msgContent;
    fields.reviewStatus = m_msgStatus;
    DClusterNotifyDataSource::insertData(fields);
}

QString DClusterNotifyInfo::getCurrentTime()
{
    QDateTime curDateTime = QDateTime::currentDateTime();
    QString curTime = curDateTime.toString("yyyy-MM-dd hh:mm");
    return curTime;
}

DClusterNotifyInfo::~DClusterNotifyInfo()
{

}
