#include "DeviceTransNotifyDataSource.h"

quint64 DeviceTransNotifyDataSource::m_accountID = 0;
void DeviceTransNotifyDataSource::createTable(quint64 accountID)
{
    m_accountID = accountID;
    QString sqlCmd = QString("create table %1 ([accountID] INTEGER, [srcUserID] INTEGER, ").arg(DT_TRANSFER_DEVICE_NOTIFY);
    sqlCmd += QString("[srcUserName] TEXT, [srcClusterID] INTEGER, [srcClusterName] TEXT, [destClusterID] INTEGER, ");
    sqlCmd += QString("[destClusterName] TEXT, [notifyContent] TEXT, [notifyVerify] TEXT, [notifyTime] TEXT, [status] INTEGER, ");
    sqlCmd += QString("PRIMARY KEY([srcUserID], [srcClusterID], [destClusterID]))");

    QString tableName = DT_TRANSFER_DEVICE_NOTIFY;
    SqliteManagment::GetInstance()->createDbTable(sqlCmd, tableName);
}

void DeviceTransNotifyDataSource::queryData(CppSQLite3Query& outSqlData)
{
    QString sqlCmd = QString("select * from %1 ").arg(DT_TRANSFER_DEVICE_NOTIFY);
    sqlCmd += QString("where [accountID] = %2").arg(m_accountID);
    SqliteManagment::GetInstance()->query(sqlCmd, outSqlData);
}

void DeviceTransNotifyDataSource::updateCheckStatus(const DeviceTransNotifyDataField& fields)
{
    QString sqlCmd = QString("update %1 set [status] = %2").arg(DT_TRANSFER_DEVICE_NOTIFY).arg(fields.status);
    sqlCmd += QString(" where [accountID] = %3 and [srcUserID] = %4 and [srcClusterID] = %5 and [destClusterID] = %6")
            .arg(m_accountID).arg(fields.srcUserID).arg(fields.srcClusterID).arg(fields.destClusterID);
    SqliteManagment::GetInstance()->update(sqlCmd);
}

void DeviceTransNotifyDataSource::insertData(const DeviceTransNotifyDataField& fields)
{
    QString sqlCmd = QString("replace into %1").arg(DT_TRANSFER_DEVICE_NOTIFY);
    sqlCmd += QString("([accountID], [srcUserID], [srcUserName], [srcClusterID], [srcClusterName], [destClusterID], [destClusterName], ");
    sqlCmd += QString("[notifyContent], [notifyVerify], [notifyTime], [status])values(%1, %2, '%3', %4, '%5', %6, '%7', '%8', '%9', '%10', %11)")
            .arg(m_accountID).arg(fields.srcUserID).arg(fields.srcUserName).arg(fields.srcClusterID).arg(fields.srcClusterName)
            .arg(fields.destClusterID).arg(fields.destClusterName).arg(fields.notifyContent).arg(fields.notifyVerify)
            .arg(fields.notifyTime).arg(fields.status);

   SqliteManagment::GetInstance()->insert(sqlCmd);
}

void DeviceTransNotifyDataSource::deleteData()
{
    QString sqlCmd = QString("delete from %1 ").arg(DT_TRANSFER_DEVICE_NOTIFY);
    sqlCmd += QString("where [accountID] = %2").arg(m_accountID);
    SqliteManagment::GetInstance()->release(sqlCmd);
}

void DeviceTransNotifyDataSource::parseDataField(CppSQLite3Query &src, DeviceTransNotifyDataField &dest)
{
    dest.accountID = atoll(src.fieldValue(0));
    dest.srcUserID = atoll(src.fieldValue(1));
    dest.srcUserName = src.fieldValue(2);
    dest.srcClusterID = atoll(src.fieldValue(3));
    dest.srcClusterName = src.fieldValue(4);
    dest.destClusterID = atoll(src.fieldValue(5));
    dest.destClusterName = src.fieldValue(6);
    dest.notifyContent = src.fieldValue(7);
    dest.notifyVerify = src.fieldValue(8);
    dest.notifyTime = src.fieldValue(9);
    dest.status = atoll(src.fieldValue(10));
}
