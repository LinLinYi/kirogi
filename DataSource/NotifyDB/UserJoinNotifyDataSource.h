#ifndef USERJOINNOTIFYDATASOURCE_H
#define USERJOINNOTIFYDATASOURCE_H

#include "SqliteManagment.h"
#include "GlobalDefine.hpp"

#include "SqliteManagment.h"

typedef struct{
    quint64 accountID;
    quint64 userID;
    QString userName;
    QString notifyContent;
    QString notifyVerify;
    QString notifyTime;
    quint8 status;
}UserJoinNotifyDataField;

class UserJoinNotifyDataSource
{
public:
    explicit UserJoinNotifyDataSource(){}
    virtual ~UserJoinNotifyDataSource(){}

public:
    static void createTable(quint64 accountID);
    static void insertData(const UserJoinNotifyDataField& fields);
    static void deleteAll();
    static void deleteData(quint64 id);
    static void queryData(CppSQLite3Query& outSqlData);
    static void updateCheckStatus(quint64 usrID, quint8 rewStatus);
    static void parseDataField(CppSQLite3Query& src, UserJoinNotifyDataField& dest);

private:
    static quint64 m_accountID;
};

#endif // USERJOINNOTIFYDATASOURCE_H
