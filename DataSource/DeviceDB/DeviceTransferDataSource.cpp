#include "DeviceTransferDataSource.h"

quint64 DeviceTransferDataSource::m_accountID = 0;
void DeviceTransferDataSource::createTable(quint64 accountID)
{
    m_accountID = accountID;
    QString sqlCmd = QString("create table %1 ([accountID] INTEGER, ").arg(DT_TRANSFER_DEVICE_INFO);
    sqlCmd += QString("[srcUserID] INTEGER, [srcUserName] TEXT, [srcClusterID] INTEGER, [srcClusterName] TEXT, ");
    sqlCmd += QString("[destClusterID] INTEGER, [destClusterName] TEXT, [deviceID] INTEGER, [deviceName] TEXT,");
    sqlCmd += QString("PRIMARY KEY([deviceID]))");

    QString tableName = DT_TRANSFER_DEVICE_INFO;
    SqliteManagment::GetInstance()->createDbTable(sqlCmd, tableName);
}

void DeviceTransferDataSource::queryData(CppSQLite3Query& outSqlData)
{
    QString sqlCmd = QString("select * from %1 ").arg(DT_TRANSFER_DEVICE_INFO);
    sqlCmd += QString("where [accountID] = %2").arg(m_accountID);
    SqliteManagment::GetInstance()->query(sqlCmd, outSqlData);
}

void DeviceTransferDataSource::queryData(CppSQLite3Query &outSqlData, quint64 srcID, quint64 destID)
{
    QString sqlCmd = QString("select * from %1 ").arg(DT_TRANSFER_DEVICE_INFO);
    sqlCmd += QString("where [accountID] = %2 and [srcClusterID] = %3 and [destClusterID] = %4")
            .arg(m_accountID).arg(srcID).arg(destID);
    SqliteManagment::GetInstance()->query(sqlCmd, outSqlData);
}

void DeviceTransferDataSource::queryData(CppSQLite3Query &outSqlData, quint64 devID)
{
    QString sqlCmd = QString("select * from %1").arg(DT_TRANSFER_DEVICE_INFO);
    sqlCmd += QString("where [accountID] = %2 and [deviceID] = %3").arg(m_accountID).arg(devID);
    SqliteManagment::GetInstance()->query(sqlCmd, outSqlData);
}

void DeviceTransferDataSource::insertData(const DeviceTransferDataField& fields)
{
    QString sqlCmd = QString("replace into %1").arg(DT_TRANSFER_DEVICE_INFO);
    sqlCmd += QString("([accountID], [srcUserID], [srcUserName], [srcClusterID], [srcClusterName], ");
    sqlCmd += QString("[destClusterID], [destClusterName], [deviceID], [deviceName])");
    sqlCmd += QString("values(%1, %2, '%3', %4, '%5', %6, '%7', %8, '%9')").arg(m_accountID)
            .arg(fields.srcUserID).arg(fields.srcUserName).arg(fields.srcClusterID)
            .arg(fields.srcClusterName).arg(fields.destClusterID).arg(fields.destClusterName)
            .arg(fields.deviceID).arg(fields.deviceName);
   SqliteManagment::GetInstance()->insert(sqlCmd);
}

void DeviceTransferDataSource::deleteData(quint64 devID)
{
    QString sqlCmd = QString("delete from %1 ").arg(DT_TRANSFER_DEVICE_INFO);
    sqlCmd += QString("where [accountID] = %2 and [devID] = %3")
            .arg(m_accountID).arg(devID);
    SqliteManagment::GetInstance()->release(sqlCmd);
}

void DeviceTransferDataSource::deleteData(quint64 srcClusterID, quint64 destClusterID)
{
    QString sqlCmd = QString("delete from %1 ").arg(DT_TRANSFER_DEVICE_INFO);
    sqlCmd += QString("where [accountID] = %2 and [srcClusterID] = %3 and [destClusterID] = %4")
            .arg(m_accountID).arg(srcClusterID).arg(destClusterID);
    SqliteManagment::GetInstance()->release(sqlCmd);
}

void DeviceTransferDataSource::parseDataField(CppSQLite3Query &src, DeviceTransferDataField &dest)
{
    dest.accountID = atoll(src.fieldValue(0));
    dest.srcUserID = atoll(src.fieldValue(1));
    dest.srcUserName = src.fieldValue(2);
    dest.srcClusterID = atoll(src.fieldValue(3));
    dest.srcClusterName = src.fieldValue(4);
    dest.destClusterID = atoll(src.fieldValue(5));
    dest.destClusterName = src.fieldValue(6);
    dest.deviceID = atoll(src.fieldValue(7));
    dest.deviceName = src.fieldValue(8);
}
