#ifndef DEVICETRANSFERDATASOURCE_H
#define DEVICETRANSFERDATASOURCE_H

#include "SqliteManagment.h"

typedef struct{
    quint64 accountID;
    quint64	srcUserID;
    QString	srcUserName;
    quint64	srcClusterID;
    QString	srcClusterName;
    quint64	destClusterID;
    QString	destClusterName;
    quint64	deviceID;
    QString	deviceName;
}DeviceTransferDataField;

class DeviceTransferDataSource
{
public:
    explicit DeviceTransferDataSource();
    virtual ~DeviceTransferDataSource();

public:
    static void createTable(quint64 accountID);
    static void insertData(const DeviceTransferDataField& fields);
    static void deleteData(quint64 devID);
    static void deleteData(quint64 srcClusterID, quint64 destClusterID);
    static void queryData(CppSQLite3Query& outSqlData);
    static void queryData(CppSQLite3Query& outSqlData, quint64 srcID, quint64 destID);
    static void parseDataField(CppSQLite3Query& src, DeviceTransferDataField& dest);
    static void queryData(CppSQLite3Query& outSqlData, quint64 devID);

private:
    static quint64 m_accountID;
};

#endif // DEVICETRANSFERDATASOURCE_H
