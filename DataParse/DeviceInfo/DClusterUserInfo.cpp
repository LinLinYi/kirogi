#include "DClusterUserInfo.h"
#include "DispersalData.h"
#include "DClusterUserDataSource.h"

DClusterUserInfo::DClusterUserInfo(quint64 clusID)
    : m_clusterID(clusID)
{

}

quint16 DClusterUserInfo::unpack(char *data)
{
    if(data == NULL) return 0;
    int offset = 0;
    quint8 role = 0;
    DispersalData dispersalData;
    offset += dispersalData.outlet(role, data + offset);        //用户角色
    m_userRole = AuthContent::numToAuthlevel(role);
    offset += dispersalData.outlet(m_userID, data + offset);    //用户ID
    offset += dispersalData.outlet(m_userName, data + offset);  //用户名称
    offset += dispersalData.outlet(m_remarks, data + offset);   //用户备注
    offset += dispersalData.outlet(m_describe, data + offset);  //用户描述
    return offset;
}

void DClusterUserInfo::loadDataSource(quint64 clusID, quint64 uID)
{
    CppSQLite3Query src;
    DClusterUserDataSource::queryData(src, clusID, uID);

    while(!src.eof()){
        DClusterUserDataField dest;
        DClusterUserDataSource::parseDataField(src, dest);

        m_userID = dest.userID;
        m_clusterID = dest.clusterID;
        m_userName = dest.userName;
        m_remarks = dest.remarks;
        m_describe = dest.describe;

        quint8 role = AuthContent::numToAuthlevel(dest.userRole);
        m_userRole = AuthContent::numToAuthlevel(role);
        src.nextRow();
    }
}

void DClusterUserInfo::deleteUserInfo(quint64 clusID, quint64 userID)
{
    DClusterUserDataSource::deleteData(clusID, userID);
}

bool DClusterUserInfo::isExistUserInfo(const quint64 clusID, const quint64 userID)
{
    CppSQLite3Query outSqlData;
    DClusterUserDataSource::queryData(outSqlData, clusID, userID);

    if(outSqlData.eof())
        return false;
    else
        return true;
}

DClusterUserInfo::~DClusterUserInfo(){}

