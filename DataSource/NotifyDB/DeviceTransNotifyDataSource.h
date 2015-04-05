#ifndef DEVICETRANSNOTIFYDATASOURCE_H
#define DEVICETRANSNOTIFYDATASOURCE_H

#include "SqliteManagment.h"

typedef struct{
    quint64 accountID;
    quint64 srcUserID;
    QString srcUserName;
    quint64 srcClusterID;
    QString srcClusterName;
    quint64 destClusterID;
    QString destClusterName;
    QString notifyTime;
    quint8 status;
    QString notifyContent;
    QString notifyVerify;
}DeviceTransNotifyDataField;

class DeviceTransNotifyDataSource
{
public:
    explicit DeviceTransNotifyDataSource() {}
    virtual ~DeviceTransNotifyDataSource() {}

public:
    static void createTable(quint64 accountID);
    static void queryData(CppSQLite3Query& outSqlData);
    static void updateCheckStatus(const DeviceTransNotifyDataField& fields);
    static void insertData(const DeviceTransNotifyDataField& fields);
    static void deleteData();
    static void parseDataField(CppSQLite3Query &src, DeviceTransNotifyDataField &dest);

private:
    static quint64 m_accountID;
};

#endif // DEVICETRANSNOTIFYDATASOURCE_H
