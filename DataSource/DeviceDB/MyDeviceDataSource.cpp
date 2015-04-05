#include "MyDeviceDataSource.h"

quint64 MyDeviceDataSource::m_accountID = 0;
void MyDeviceDataSource::createTable(quint64 accountID)
{
    m_accountID = accountID;
    QString sqlCmd = QString("create table %1 ([accountID] INTEGER, [deviceID] INTEGER, [clusterID] INTEGER, ").arg(DT_MYDEVICE_INFO);
    sqlCmd += QString("[deviceName] TEXT, [macAddr] TEXT, PRIMARY KEY([accountID], [deviceID]))");

    QString tableName = DT_MYDEVICE_INFO;
    SqliteManagment::GetInstance()->createDbTable(sqlCmd, tableName);
}

void MyDeviceDataSource::queryData(CppSQLite3Query& outSqlData)
{
    QString sqlCmd = QString("select * from %1 ").arg(DT_MYDEVICE_INFO);
    sqlCmd += QString("where [accountID] = %2").arg(m_accountID);
    SqliteManagment::GetInstance()->query(sqlCmd, outSqlData);
}

void MyDeviceDataSource::queryData(CppSQLite3Query &outSqlData, quint64 deviceID)
{
    QString sqlCmd = QString("select * from %1").arg(DT_MYDEVICE_INFO);
    sqlCmd += QString("where [accountID] = %2 and [deviceID] = %3").arg(m_accountID).arg(deviceID);
    SqliteManagment::GetInstance()->query(sqlCmd, outSqlData);
}

void MyDeviceDataSource::insertData(const MyDeviceDataField& fields)
{
    QString sqlCmd = QString("replace into %1").arg(DT_MYDEVICE_INFO);
    sqlCmd += QString("([accountID], [deviceID], [clusterID], [deviceName], [macAddr])");
    sqlCmd += QString("values(%1, %2, %3, '%4', '%5')").arg(m_accountID).arg(fields.deviceID)
            .arg(fields.clusterID).arg(fields.deviceName).arg(fields.macAddress);
   SqliteManagment::GetInstance()->insert(sqlCmd);
}

void MyDeviceDataSource::deleteData(quint64 deviceID)
{
    QString sqlCmd = QString("delete from %1 ").arg(DT_MYDEVICE_INFO);
    sqlCmd += QString("where [accountID] = %2 and [deviceID] = %3").arg(m_accountID).arg(deviceID);
    SqliteManagment::GetInstance()->release(sqlCmd);
}

void MyDeviceDataSource::deleteDevices(quint64 clusterID)
{
    QString sqlCmd = QString("delete from %1 ").arg(DT_MYDEVICE_INFO);
    sqlCmd += QString("where [accountID] = %2 and [clusterID] = %3").arg(m_accountID).arg(clusterID);
    SqliteManagment::GetInstance()->release(sqlCmd);
}

void MyDeviceDataSource::parseDataField(CppSQLite3Query &src, MyDeviceDataField &dest)
{
    dest.accountID = atoll(src.fieldValue(0));
    dest.deviceID = atoll(src.fieldValue(1));
    dest.clusterID = atoll(src.fieldValue(2));
    dest.deviceName = src.fieldValue(3);
    dest.macAddress = src.fieldValue(4);
}
