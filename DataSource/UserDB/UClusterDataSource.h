#ifndef UCLUSTERDATASOURCE_H
#define UCLUSTERDATASOURCE_H

#include "SqliteManagment.h"

typedef struct{
    quint64 accountID;
    quint64 clusterID;
    quint64 userRole;
    QString clusterName;
    QString describe;
}UClusterDataField;

class UClusterDataSource
{
public:
    explicit UClusterDataSource(){}
    virtual ~UClusterDataSource(){}

public:
    static void createTable(quint64 accountID);
    static void insertData(const UClusterDataField& fields);
    static void deleteData();
    static void deleteData(quint64 clusterID);
    static void queryData(CppSQLite3Query& outSqlData);
    static void queryData(CppSQLite3Query &outSqlData, quint64 clusterID);
    static void updateDescribe(const QString& describe, quint64 clusterID);
    static void parseDataField(CppSQLite3Query& src, UClusterDataField& dest);

private:
    static quint64 m_accountID;

};

#endif // UCLUSTERDATASOURCE_H
