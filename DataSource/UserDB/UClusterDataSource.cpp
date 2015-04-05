#include "UClusterDataSource.h"
#include "SqliteManagment.h"

quint64 UClusterDataSource::m_accountID = 0;

void UClusterDataSource::createTable(quint64 accountID)
{
    m_accountID = accountID;
    QString sqlCmd = QString("create table %1([accountID] INTEGER, [clusterID] INTEGER,").arg(UT_CLUSTER_INFO);
    sqlCmd += QString("[clusterName] TEXT, [describe] TEXT, [userRole] INTEGER, PRIMARY KEY([accountID], [clusterID]))");

    QString tableName = UT_CLUSTER_INFO;
    SqliteManagment::GetInstance()->createDbTable(sqlCmd, tableName);
}

void UClusterDataSource::queryData(CppSQLite3Query& outSqlData)
{
    QString sqlCmd = QString("select * from %1 ").arg(UT_CLUSTER_INFO);
    sqlCmd += QString("where [accountID] = %2").arg(m_accountID);
    SqliteManagment::GetInstance()->query(sqlCmd, outSqlData);
}

void UClusterDataSource::queryData(CppSQLite3Query &outSqlData, quint64 clusterID)
{
    QString sqlCmd = QString("select * from %1 ").arg(UT_CLUSTER_INFO);
    sqlCmd += QString("where [accountID] = %2 and [clusterID] = %3").arg(m_accountID).arg(clusterID);
    SqliteManagment::GetInstance()->query(sqlCmd, outSqlData);
}

void UClusterDataSource::insertData(const UClusterDataField& fields)
{
    QString sqlCmd = QString("replace into %1").arg(UT_CLUSTER_INFO);
    sqlCmd += QString("([accountID], [clusterID], [clusterName], [describe], [userRole])");
    sqlCmd += QString("values(%1, %2, '%3', '%4', %5)").arg(m_accountID).arg(fields.clusterID)
            .arg(fields.clusterName).arg(fields.describe).arg(fields.userRole);
    SqliteManagment::GetInstance()->insert(sqlCmd);
}

void UClusterDataSource::updateDescribe(const QString &describe, quint64 clusterID)
{
    QString sqlCmd = QString("update %1 set ").arg(UT_CLUSTER_INFO);
    sqlCmd += QString("[describe] = '%2' where [accountID] = %3 and [clusterID] = %4")
            .arg(describe).arg(m_accountID).arg(clusterID);
    SqliteManagment::GetInstance()->update(sqlCmd);
}

void UClusterDataSource::deleteData()
{
    QString sqlCmd = QString("delete from %1 ").arg(UT_CLUSTER_INFO);
    sqlCmd += QString("where [accountID] = %2").arg(m_accountID);
    SqliteManagment::GetInstance()->release(sqlCmd);
}

void UClusterDataSource::deleteData(quint64 clusterID)
{
    QString sqlCmd = QString("delete from %1 ").arg(UT_CLUSTER_INFO);
    sqlCmd += QString("where [accountID] = %2 and [clusterID] = %3").arg(m_accountID).arg(clusterID);
    SqliteManagment::GetInstance()->release(sqlCmd);
}

void UClusterDataSource::parseDataField(CppSQLite3Query &src, UClusterDataField &dest)
{
    dest.accountID = atoll(src.fieldValue(0));
    dest.clusterID = atoll(src.fieldValue(1));
    dest.clusterName = src.fieldValue(2);
    dest.describe = src.fieldValue(3);
    dest.userRole = atoll(src.fieldValue(4));
}


