#ifndef UCLUSTERNOTIFYDATASOURCE_H
#define UCLUSTERNOTIFYDATASOURCE_H

#include "GlobalDefine.hpp"
#include "SqliteManagment.h"

typedef struct{
    quint64 accountID;
    quint64 clusterID;
    QString clusterName;
    quint64 userID;
    QString userName;
    quint64 notifyType;
    QString notifyContent;
    QString notifyVerify;
    QString notifyTime;
    quint8 status;
}UClusterNotifyDataField;

class UClusterNotifyDataSource
{
public:
    explicit UClusterNotifyDataSource(){}
    virtual ~UClusterNotifyDataSource(){}

public:
    static void createTable(quint64 accountID);
    static void insertData(const UClusterNotifyDataField& fields);
    static void deleteData();
    static void queryData(CppSQLite3Query& outSqlData);
    static void updateCheckStatus(quint64 notifyType, quint64 usrID, quint64 clusID, quint8 status);
    static void parseDataField(CppSQLite3Query& src, UClusterNotifyDataField& dest);

private:
    static quint64 m_accountID;
};

#endif // UCLUSTERNOTIFYDATASOURCE_H
