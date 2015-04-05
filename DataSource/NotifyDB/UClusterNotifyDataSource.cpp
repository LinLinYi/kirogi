#include "UClusterNotifyDataSource.h"

quint64 UClusterNotifyDataSource::m_accountID = 0;
void UClusterNotifyDataSource::createTable(quint64 accountID)
{
    m_accountID = accountID;
    QString sqlCmd = QString("create table %1").arg(UT_CLUSTER_NOTIFY);
    sqlCmd += QString("([accountID] INTEGER, [notifyType] INTEGER, [userID] INTEGER, [userName] TEXT, ");
    sqlCmd += QString("[clusterID] INTEGER,  [clusterName] TEXT, [notifyContent] TEXT, [notifyVerify] TEXT, ");
    sqlCmd += QString("[notifyTime] TEXT, [status] INTEGER, PRIMARY KEY([notifyType], [userID], [clusterID]))");

    QString tableName = UT_CLUSTER_NOTIFY;
    SqliteManagment::GetInstance()->createDbTable(sqlCmd, tableName);
}

void UClusterNotifyDataSource::queryData(CppSQLite3Query &outSqlData)
{
    QString sqlCmd = QString("select * from %1 ").arg(UT_CLUSTER_NOTIFY);
    sqlCmd += QString("where [accountID] = %2").arg(m_accountID);
    SqliteManagment::GetInstance()->query(sqlCmd, outSqlData);
}

void UClusterNotifyDataSource::insertData(const UClusterNotifyDataField& fields)
{
    QString sqlCmd = QString("replace into %1").arg(UT_CLUSTER_NOTIFY);
    sqlCmd += QString("([accountID], [notifyType], [userID], [userName], [clusterID], [clusterName], [notifyContent], ");
    sqlCmd += QString("[notifyVerify], [notifyTime], [status])values(%1, %2, %3, '%4', %5, '%6', '%7', '%8', '%9', %10)")
            .arg(m_accountID).arg(fields.notifyType).arg(fields.userID).arg(fields.userName).arg(fields.clusterID)
            .arg(fields.clusterName).arg(fields.notifyContent).arg(fields.notifyVerify).arg(fields.notifyTime).arg(fields.status);
    SqliteManagment::GetInstance()->insert(sqlCmd);
}

void UClusterNotifyDataSource::deleteData()
{
    QString sqlCmd = QString("delete from %1").arg(UT_CLUSTER_NOTIFY);
    SqliteManagment::GetInstance()->release(sqlCmd);
}

void UClusterNotifyDataSource::updateCheckStatus(quint64 notifyType, quint64 userID, quint64 clusterID, quint8 status)
{
    QString sqlCmd = QString("update %1 set [status] = %2").arg(UT_CLUSTER_NOTIFY).arg(status);
    sqlCmd += QString(" where [accountID] = %3 and [notifyType] = %4").arg(m_accountID).arg(notifyType);
    sqlCmd += QString(" and [userID] = %5 and [clusterID] = %6").arg(userID).arg(clusterID);
    SqliteManagment::GetInstance()->update(sqlCmd);
}

void UClusterNotifyDataSource::parseDataField(CppSQLite3Query &src, UClusterNotifyDataField &dest)
{
    dest.accountID = atoll(src.fieldValue(0));
    dest.notifyType = atoll(src.fieldValue(1));
    dest.userID = atoll(src.fieldValue(2));
    dest.userName = src.fieldValue(3);
    dest.clusterID = atoll(src.fieldValue(4));
    dest.clusterName = src.fieldValue(5);
    dest.notifyContent = src.fieldValue(6);
    dest.notifyVerify = src.fieldValue(7);
    dest.notifyTime = src.fieldValue(8);
    dest.status = atoi(src.fieldValue(9));
}


