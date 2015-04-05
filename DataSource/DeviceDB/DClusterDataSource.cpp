#include "DClusterDataSource.h"
#include "SqliteManagment.h"

quint64 DClusterDataSource::m_accountID = 0;

void DClusterDataSource::createTable(quint64 accountID)
{
    m_accountID = accountID;
    QString sqlCmd = QString("create table %1([accountID] INTEGER, [clusterID] INTEGER,").arg(DT_CLUSTER_INFO);
    sqlCmd += QString("[clusterName] TEXT, [remarks] TEXT, [describe] TEXT, [userRole] INTEGER, PRIMARY KEY([accountID], [clusterID]))");

    QString tableName = DT_CLUSTER_INFO;
    SqliteManagment::GetInstance()->createDbTable(sqlCmd, tableName);
}

void DClusterDataSource::queryData(CppSQLite3Query& outSqlData)
{
    QString sqlCmd = QString("select * from %1 ").arg(DT_CLUSTER_INFO);
    sqlCmd += QString("where [accountID] = %2").arg(m_accountID);
    SqliteManagment::GetInstance()->query(sqlCmd, outSqlData);
}

void DClusterDataSource::queryData(CppSQLite3Query &outSqlData, quint64 clusterID)
{
    QString sqlCmd = QString("select * from %1 ").arg(DT_CLUSTER_INFO);
    sqlCmd += QString("where [accountID] = %2 and [clusterID] = %3").arg(m_accountID).arg(clusterID);
    SqliteManagment::GetInstance()->query(sqlCmd, outSqlData);
}

void DClusterDataSource::queryData(CppSQLite3Query &outSqlData, QString &clusterName)
{
    QString sqlCmd = QString("select * from %1 ").arg(DT_CLUSTER_INFO);
    sqlCmd += QString("where [accountID] = %2 and [clusterName] = '%3'").arg(m_accountID).arg(clusterName);
    SqliteManagment::GetInstance()->query(sqlCmd, outSqlData);
}

void DClusterDataSource::insertData(const DClusterDataField& fields)
{
    QString sqlCmd = QString("replace into %1").arg(DT_CLUSTER_INFO);
    sqlCmd += QString("([accountID], [clusterID], [clusterName], [remarks], [describe], [userRole])");
    sqlCmd += QString("values(%1, %2, '%3', '%4', '%5', %6)").arg(m_accountID).arg(fields.clusterID)
            .arg(fields.clusterName).arg(fields.remarks).arg(fields.describe).arg(fields.userRole);
    SqliteManagment::GetInstance()->insert(sqlCmd);
}

void DClusterDataSource::updateRemarks(const QString &remarks, quint64 clusterID)
{
    QString sqlCmd = QString("update %1 set ").arg(DT_CLUSTER_INFO);
    sqlCmd += QString("[remarks] = '%2' where [accountID] = %3 and [clusterID] = %4")
            .arg(remarks).arg(m_accountID).arg(clusterID);
    SqliteManagment::GetInstance()->update(sqlCmd);
}

void DClusterDataSource::updateDescribe(const QString &describe, quint64 clusterID)
{
    QString sqlCmd = QString("update %1 set ").arg(DT_CLUSTER_INFO);
    sqlCmd += QString("[describe] = '%2' where [accountID] = %3 and [clusterID] = %4")
            .arg(describe).arg(m_accountID).arg(clusterID);
    SqliteManagment::GetInstance()->update(sqlCmd);
}

void DClusterDataSource::deleteData()
{
    QString sqlCmd = QString("delete from %1 ").arg(DT_CLUSTER_INFO);
    sqlCmd += QString("where [accountID] = %2").arg(m_accountID);
    SqliteManagment::GetInstance()->release(sqlCmd);
}

void DClusterDataSource::deleteData(quint64 clusterID)
{
    QString sqlCmd = QString("delete from %1 ").arg(DT_CLUSTER_INFO);
    sqlCmd += QString("where [accountID] = %2 and [clusterID] = %3").arg(m_accountID).arg(clusterID);
    SqliteManagment::GetInstance()->release(sqlCmd);
}

void DClusterDataSource::parseDataField(CppSQLite3Query &src, DClusterDataField &dest)
{
    dest.accountID = atoll(src.fieldValue(0));
    dest.clusterID = atoll(src.fieldValue(1));
    dest.clusterName = src.fieldValue(2);
    dest.remarks = src.fieldValue(3);
    dest.describe = src.fieldValue(4);
    dest.userRole = atoll(src.fieldValue(5));
}


