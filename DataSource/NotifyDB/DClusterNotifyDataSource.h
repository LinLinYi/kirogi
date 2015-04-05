#ifndef DCLUSTERNOTIFYDATASOURCE_H
#define DCLUSTERNOTIFYDATASOURCE_H

#include "SqliteManagment.h"

typedef struct{
    quint64 accountID;
    quint64 clusterID;
    QString clusterName;
    quint64 userID;
    QString userName;
    quint8 notifyType;
    QString notifyContent;
    QString notifyVerify;
    QString notifyTime;
    quint8 status;
}DClusterNotifyDataField;

class DClusterNotifyDataSource
{
public:
    explicit DClusterNotifyDataSource(){}
    virtual ~DClusterNotifyDataSource(){}

public:
    static void createTable(quint64 accountID);
    static void insertData(const DClusterNotifyDataField& fields);
    static void deleteData();
    static void queryData(CppSQLite3Query& outSqlData);
    static void updateCheckStatus(quint64 notifyType, quint64 usrID, quint64 clusID, quint8 status);
    static void parseDataField(CppSQLite3Query& src, DClusterNotifyDataField& dest);

private:
    static quint64 m_accountID;
};

#endif // DCLUSTERNOTIFYCommandCode::H
