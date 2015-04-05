#include "DClusterDeviceDataSource.h"

quint64 DClusterDeviceDataSource::m_accountID = 0;
void DClusterDeviceDataSource::createTable(quint64 accountID)
{
    m_accountID = accountID;
    QString sqlCmd = QString("create table %1([accountID] INTEGER, [deviceID] INTEGER,").arg(DT_CLUSTER_DEVICE_INFO);
    sqlCmd += QString("[clusterID] INTEGER, [deviceName] TEXT, [macAddr] TEXT, [remarks] TEXT, [describe] TEXT, PRIMARY KEY([accountID], [deviceID]))");

    QString tableName = DT_CLUSTER_DEVICE_INFO;
    SqliteManagment::GetInstance()->createDbTable(sqlCmd, tableName);
}

void DClusterDeviceDataSource::queryData(CppSQLite3Query &outSqlData, quint64 clusterID)
{
    QString sqlCmd = QString("select * from %1 ").arg(DT_CLUSTER_DEVICE_INFO);
    sqlCmd += QString("where [accountID] = %2 and [clusterID] = %3").arg(m_accountID).arg(clusterID);
    SqliteManagment::GetInstance()->query(sqlCmd, outSqlData);
}

void DClusterDeviceDataSource::queryData(CppSQLite3Query &outSqlData, quint64 clusID, quint64 devID)
{
    QString sqlCmd = QString("select * from %1 ").arg(DT_CLUSTER_DEVICE_INFO);
    sqlCmd += QString("where [accountID] = %2 and [clusterID] = %3 and [deviceID] = %4")
            .arg(m_accountID).arg(clusID).arg(devID);
    SqliteManagment::GetInstance()->query(sqlCmd, outSqlData);
}

void DClusterDeviceDataSource::updateRemarks(quint64 clusID, quint64 devID, const QString &remarks)
{
    QString sqlCmd = QString("update %1 set ").arg(DT_CLUSTER_DEVICE_INFO);
    sqlCmd += QString("[remarks] = '%2' where [accountID] = %3 and [clusterID] = %4 and [deviceID] = %5")
            .arg(remarks).arg(m_accountID).arg(clusID).arg(devID);
    SqliteManagment::GetInstance()->update(sqlCmd);
}

void DClusterDeviceDataSource::updateDescribe(quint64 clusID, quint64 devID, const QString &describe)
{
    QString sqlCmd = QString("update %1 set ").arg(DT_CLUSTER_DEVICE_INFO);
    sqlCmd += QString("[describe] = '%2' where [accountID] = %3 and [clusterID] = %4 and [deviceID] = %5")
            .arg(describe).arg(m_accountID).arg(clusID).arg(devID);
    SqliteManagment::GetInstance()->update(sqlCmd);
}

void DClusterDeviceDataSource::insertData(const DClusterDevDataField& fields)
{
    QString sqlCmd = QString("replace into %1").arg(DT_CLUSTER_DEVICE_INFO);
    sqlCmd += QString("([accountID], [deviceID], [clusterID], [deviceName], [macAddr], ");
    sqlCmd += QString("[remarks], [describe])values(%2, %3, %4, '%5', '%6', '%7', '%8')")
            .arg(m_accountID).arg(fields.deviceID).arg(fields.clusterID).arg(fields.deviceName)
            .arg(fields.macAddress).arg(fields.remarks).arg(fields.describe);
    SqliteManagment::GetInstance()->insert(sqlCmd);
}

void DClusterDeviceDataSource::deleteData()
{
    QString sqlCmd = QString("delete from %1 ").arg(DT_CLUSTER_DEVICE_INFO);
    sqlCmd += QString("where [accountID] = %2").arg(m_accountID);
    SqliteManagment::GetInstance()->release(sqlCmd);
}

void DClusterDeviceDataSource::deleteData(quint64 deviceID)
{
    QString sqlCmd = QString("delete from %1 ").arg(DT_CLUSTER_DEVICE_INFO);
    sqlCmd += QString("where [accountID] = %1 and [deviceID] = %2").arg(m_accountID).arg(deviceID);
    SqliteManagment::GetInstance()->release(sqlCmd);
}

void DClusterDeviceDataSource::deleteDevices(quint64 clusterID)
{
    QString sqlCmd = QString("delete from %1 ").arg(DT_CLUSTER_DEVICE_INFO);
    sqlCmd += QString("where [accountID] = %1 and [clusterID] = %2").arg(m_accountID).arg(clusterID);
    SqliteManagment::GetInstance()->release(sqlCmd);
}

void DClusterDeviceDataSource::parseDataField(CppSQLite3Query &src, DClusterDevDataField &dest)
{
    dest.accountID = atoll(src.fieldValue(0));
    dest.deviceID = atoll(src.fieldValue(1));
    dest.clusterID = atoll(src.fieldValue(2));
    dest.deviceName = src.fieldValue(3);
    dest.macAddress = src.fieldValue(4);
    dest.remarks = src.fieldValue(5);
    dest.describe = src.fieldValue(6);
}
