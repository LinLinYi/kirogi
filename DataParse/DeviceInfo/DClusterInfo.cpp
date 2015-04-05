#include "DClusterInfo.h"
#include "UserProtocol.hpp"
#include "DispersalData.h"
#include "DClusterDataSource.h"

DClusterInfo::DClusterInfo(quint64 clusterID)
    : m_clusterID(clusterID)
{

}

quint16 DClusterInfo::unpack(char *data)
{
    if(data == NULL) return 0;

    quint16 offset = 0;
    quint8 role = 0;
    DispersalData dispersalData;

    offset += dispersalData.outlet(role, data + offset);             //用户角色
    m_userRole = AuthContent::numToAuthlevel(role);
    offset += dispersalData.outlet(m_clusterID, data + offset);      //群ID
    offset += dispersalData.outlet(m_clusterName, data + offset);    //群账号
    offset += dispersalData.outlet(m_clusterRemarks, data + offset); //群备注
    offset += dispersalData.outlet(m_clusterDescribe, data + offset);//群介绍
    m_clusterID = m_clusterID | ConstNum::DEVICE_ID_BASIC_PREFIX;
    return offset;
}

void DClusterInfo::loadDataSource(quint64 clusID)
{
    if(clusID == 0)
        clusID = m_clusterID;

    CppSQLite3Query src;
    DClusterDataSource::queryData(src, clusID);

    while(!src.eof()){
        DClusterDataField dest;
        DClusterDataSource::parseDataField(src, dest);

        m_clusterID = dest.clusterID;
        m_clusterName = dest.clusterName;
        m_clusterRemarks = dest.remarks;
        m_clusterDescribe = dest.describe;

        quint8 role = dest.userRole;
        m_userRole = AuthContent::numToAuthlevel(role);
        src.nextRow();
    }
}

void DClusterInfo::updateRemarks(const QString& remarks)
{
    m_clusterRemarks = remarks;
    DClusterDataSource::updateRemarks(remarks, m_clusterID);
}

void DClusterInfo::updateDescribe(const QString& describe)
{
    m_clusterDescribe = describe;
    DClusterDataSource::updateDescribe(describe, m_clusterID);
}

bool DClusterInfo::isExistClusterInfo(quint64 clusID)
{
    CppSQLite3Query outSqlData;
    DClusterDataSource::queryData(outSqlData, clusID);

    if(!outSqlData.eof())
        return true;
    else
        return false;
}

bool DClusterInfo::isExistClusterInfo(QString& clusName)
{
    CppSQLite3Query outSqlData;
    DClusterDataSource::queryData(outSqlData, clusName);

    if(!outSqlData.eof())
        return true;
    else
        return false;
}

DClusterInfo::DClusterInfo(const DClusterInfo &info)
{
    m_userRole = info.m_userRole;
    m_clusterID = info.m_clusterID;
    m_clusterName = info.m_clusterName;
    m_clusterRemarks = info.m_clusterRemarks;
    m_clusterGroupID = info.m_clusterGroupID;
    m_clusterDescribe = info.m_clusterDescribe;
}

DClusterInfo& DClusterInfo::operator =(const DClusterInfo& info)
{
    m_userRole = info.m_userRole;
    m_clusterID = info.m_clusterID;
    m_clusterName = info.m_clusterName;
    m_clusterRemarks = info.m_clusterRemarks;
    m_clusterGroupID = info.m_clusterGroupID;
    m_clusterDescribe = info.m_clusterDescribe;

    return *this;
}

DClusterInfo::~DClusterInfo(){}
