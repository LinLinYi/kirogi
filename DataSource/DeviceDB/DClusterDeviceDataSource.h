#ifndef DCLUSTERDEVICEDATASOURCE_H
#define DCLUSTERDEVICEDATASOURCE_H

#include "SqliteManagment.h"

typedef struct{
    quint64 accountID;
    quint64 clusterID;
    quint64 deviceID;
    QString deviceName;
    QString macAddress;
    QString remarks;
    QString describe;
}DClusterDevDataField;

class DClusterDeviceDataSource
{
public:
    explicit DClusterDeviceDataSource(){}
    virtual ~DClusterDeviceDataSource(){}

public:
    static void createTable(quint64 accountID);
    static void insertData(const DClusterDevDataField& fields);
    static void deleteData();
    static void deleteData(quint64 deviceID);
    static void deleteDevices(quint64 clusterID);
    static void queryData(CppSQLite3Query& outSqlData, quint64 clusID);
    static void queryData(CppSQLite3Query& outSqlData, quint64 clusID, quint64 devID);
    static void updateRemarks(quint64 clusID, quint64 devID, const QString& remarks);
    static void updateDescribe(quint64 clusID, quint64 devID, const QString& describe);
    static void parseDataField(CppSQLite3Query& src, DClusterDevDataField& dest);

private:
    static quint64 m_accountID;
};

#endif // DCLUSTERDEVICEDATASOURCE_H
