#ifndef DCLUSTERUSERDATASOURCE_H
#define DCLUSTERUSERDATASOURCE_H

#include "SqliteManagment.h"

typedef struct{
    quint64 accountID;
    quint64 clusterID;
    quint64 userID;
    quint64 userRole;
    QString userName;
    QString remarks;
    QString describe;
}DClusterUserDataField;

class DClusterUserDataSource
{
public:
    explicit DClusterUserDataSource(){}
    virtual ~DClusterUserDataSource(){}

public:
    static void createTable(quint64 accountID);
    static void insertData(const DClusterUserDataField& fields);
    static void deleteData();
    static void deleteData(quint64 clusterID, quint64 userID);
    static void deleteUsers(quint64 clusterID);
    static void queryData(CppSQLite3Query& outSqlData, quint64 clusterID);
    static void queryData(CppSQLite3Query &outSqlData, quint64 clusID, quint64 userID);
    static void parseDataField(CppSQLite3Query& src, DClusterUserDataField& dest);

private:
    static quint64 m_accountID;
};

#endif // DCLUSTERUSERDATASOURCE_H
