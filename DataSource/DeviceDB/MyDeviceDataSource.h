#ifndef MYDEVICEDATASOURCE_H
#define MYDEVICEDATASOURCE_H

#include "SqliteManagment.h"

typedef struct{
    quint64 accountID;
    quint64 deviceID;
    quint64 clusterID;
    QString deviceName;
    QString macAddress;
}MyDeviceDataField;

class MyDeviceDataSource
{
public:
    explicit MyDeviceDataSource(){}
    virtual ~MyDeviceDataSource(){}

public:
    static void createTable(quint64 accountID);
    static void insertData(const MyDeviceDataField& fields);
    static void deleteData();
    static void deleteData(quint64 deviceID);
    static void deleteDevices(quint64 clusterID);
    static void queryData(CppSQLite3Query& outSqlData);
    static void queryData(CppSQLite3Query& outSqlData, quint64 deviceID);
    static void parseDataField(CppSQLite3Query& src, MyDeviceDataField& dest);

private:
    static quint64 m_accountID;
};

#endif // MYDEVICEDATASOURCE_H
