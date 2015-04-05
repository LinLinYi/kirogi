#ifndef DCLUSTERDATASOURCE_H
#define DCLUSTERDATASOURCE_H

#include "SqliteManagment.h"

typedef struct{
    quint64 accountID;
    quint64 clusterID;
    quint64 userRole;
    QString clusterName;
    QString remarks;
    QString describe;
}DClusterDataField;

class DClusterDataSource
{
public:
    explicit DClusterDataSource(){}
    virtual ~DClusterDataSource(){}

public:
    static void createTable(quint64 accountID);
    static void insertData(const DClusterDataField& fields);
    static void deleteData();
    static void deleteData(quint64 clusterID);
    static void queryData(CppSQLite3Query& outSqlData);
    static void queryData(CppSQLite3Query& outSqlData, quint64 clusterID);
    static void queryData(CppSQLite3Query& outSqlData, QString& clusterName);
    static void updateDescribe(const QString& describe, quint64 clusterID);
    static void updateRemarks(const QString& remarks, quint64 clusterID);
    static void parseDataField(CppSQLite3Query& src, DClusterDataField& dest);

private:
    static quint64 m_accountID;

};

#endif // DCLUSTERDATASOURCE_H
