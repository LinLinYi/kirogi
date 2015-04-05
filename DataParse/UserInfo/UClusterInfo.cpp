#include "UClusterInfo.h"
#include "UserProtocol.hpp"
#include "DispersalData.h"
#include "UClusterDataSource.h"

UClusterInfo::UClusterInfo(quint64 clusterID)
    : m_clusterID(clusterID)
{

}

quint16 UClusterInfo::unpack(char *data)
{
    if(data == NULL) return 0;

    quint16 offset = 0;
    quint8 role = 0;
    DispersalData dispersalData;

    offset += dispersalData.outlet(role, data + offset);             //用户角色
    m_userRole = AuthContent::numToAuthlevel(role);
    offset += dispersalData.outlet(m_clusterID, data + offset);      //群ID
    offset += dispersalData.outlet(m_clusterName, data + offset);    //群名称
    offset += dispersalData.outlet(m_clusterDescribe, data + offset);//群介绍
    m_clusterID = m_clusterID | ConstNum::DEVICE_ID_BASIC_PREFIX;
    return offset;
}

void UClusterInfo::loadDataSource(quint64 clusID)
{
    if(clusID == 0){
        clusID = m_clusterID;
    }

    CppSQLite3Query src;
    UClusterDataSource::queryData(src, clusID);

    while(!src.eof()){
        UClusterDataField dest;
        UClusterDataSource::parseDataField(src, dest);

        m_clusterID = dest.clusterID;
        m_clusterName = dest.clusterName;
        m_clusterDescribe = dest.describe;

        quint8 role = dest.userRole;
        m_userRole = AuthContent::numToAuthlevel(role);
        src.nextRow();
    }
}

void UClusterInfo::updateDescribe(const QString& describe)
{
    m_clusterDescribe = describe;
    UClusterDataSource::updateDescribe(describe, m_clusterID);
}

bool UClusterInfo::isExistClusterInfo(quint64 clusID)
{
    CppSQLite3Query outSqlData;
    UClusterDataSource::queryData(outSqlData, clusID);

    if(!outSqlData.eof())
        return true;
    else
        return false;
}

UClusterInfo::UClusterInfo(const UClusterInfo &info)
{
    m_userRole = info.m_userRole;
    m_clusterID = info.m_clusterID;
    m_clusterName = info.m_clusterName;
    m_clusterDescribe = info.m_clusterDescribe;
}

UClusterInfo& UClusterInfo::operator =(const UClusterInfo& info)
{
    m_userRole = info.m_userRole;
    m_clusterID = info.m_clusterID;
    m_clusterName = info.m_clusterName;
    m_clusterDescribe = info.m_clusterDescribe;

    return *this;
}

UClusterInfo::~UClusterInfo(){}


