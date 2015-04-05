#include "DClusterUserDataSource.h"

quint64 DClusterUserDataSource::m_accountID = 0;
void DClusterUserDataSource::createTable(quint64 accountID)
{
    m_accountID = accountID;
    QString sqlCmd = QString("create table %1([accountID] INTEGER, [userID] INTEGER,").arg(DT_CLUSTER_USER_INFO);
    sqlCmd += QString("[clusterID] INTEGER, [userRole] INTEGER, [userName] TEXT,[remarks] TEXT, [describe] TEXT, PRIMARY KEY([accountID], [userID]))");

    QString tableName = DT_CLUSTER_USER_INFO;
    SqliteManagment::GetInstance()->createDbTable(sqlCmd, tableName);
}

void DClusterUserDataSource::queryData(CppSQLite3Query &outSqlData, quint64 clusterID)
{
    QString sqlCmd = QString("select * from %1 ").arg(DT_CLUSTER_USER_INFO);
    sqlCmd += QString("where [accountID] = %2 and [clusterID] = %3").arg(m_accountID).arg(clusterID);
    SqliteManagment::GetInstance()->query(sqlCmd, outSqlData);
}

void DClusterUserDataSource::queryData(CppSQLite3Query &outSqlData, quint64 clusID, quint64 userID)
{
    QString sqlCmd = QString("select * from %1 ").arg(DT_CLUSTER_USER_INFO);
    sqlCmd += QString("where [accountID] = %2 and [clusterID] = %3 and [userID] = %4")
            .arg(m_accountID).arg(clusID).arg(userID);
    SqliteManagment::GetInstance()->query(sqlCmd, outSqlData);
}

void DClusterUserDataSource::insertData(const DClusterUserDataField& fields)
{
    QString sqlCmd = QString("replace into %1").arg(DT_CLUSTER_USER_INFO);
    sqlCmd += QString("([accountID], [userID], [clusterID], [userRole], [userName], [remarks], [describe])");
    sqlCmd += QString("values(%1, %2, %3, %4, '%5', '%6', '%7')").arg(m_accountID).arg(fields.userID)
            .arg(fields.clusterID).arg(fields.userRole).arg(fields.userName).arg(fields.remarks)
            .arg(fields.describe);
    SqliteManagment::GetInstance()->insert(sqlCmd);
}

void DClusterUserDataSource::deleteData()
{
    QString sqlCmd = QString("delete from %1 ").arg(DT_CLUSTER_USER_INFO);
    sqlCmd += QString("where [accountID] = %2").arg(m_accountID);
    SqliteManagment::GetInstance()->release(sqlCmd);
}

void DClusterUserDataSource::deleteData(quint64 clusterID, quint64 userID)
{
    QString sqlCmd = QString("delete from %1 ").arg(DT_CLUSTER_USER_INFO);
    sqlCmd += QString("where [accountID] = %2 and [clusterID] = %3 and [userID] = %4").arg(m_accountID).arg(clusterID).arg(userID);
    SqliteManagment::GetInstance()->release(sqlCmd);
}

void DClusterUserDataSource::deleteUsers(quint64 clusterID)
{
    QString sqlCmd = QString("delete from %1 ").arg(DT_CLUSTER_USER_INFO);
    sqlCmd += QString("where [accountID] = %1 and [clusterID] = %2").arg(m_accountID).arg(clusterID);
    SqliteManagment::GetInstance()->release(sqlCmd);
}

void DClusterUserDataSource::parseDataField(CppSQLite3Query &src, DClusterUserDataField &dest)
{
    dest.accountID = atoll(src.fieldValue(0));
    dest.userID = atoll(src.fieldValue(1));
    dest.clusterID = atoll(src.fieldValue(2));
    dest.userRole = atoll(src.fieldValue(3));
    dest.userName = src.fieldValue(4);
    dest.remarks = src.fieldValue(5);
    dest.describe = src.fieldValue(6);
}
