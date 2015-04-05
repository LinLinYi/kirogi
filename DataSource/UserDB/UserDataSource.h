#ifndef USERDATASOURCE_H
#define USERDATASOURCE_H

#include "SqliteManagment.h"

typedef struct{
    quint64 accountID;
    quint64 userID;
    QString userName;
    QString remarks;
    QString describe;
    QString userMail;
    QString userPortrait;
}UserDataField;

class UserDataSource
{
public:
    explicit UserDataSource(){}
    virtual ~UserDataSource(){}

public:
    static void createTable(quint64 accountID);
    static void insertData(UserDataField& fields);
    static void deleteData();
    static void deleteData(quint64 userID);
    static void queryData(CppSQLite3Query& outSqlData);
    static void queryData(CppSQLite3Query& outSqlData, quint64 userID);
    static void updateRemarks(const QString& remarks, quint64 userID);
    static void updateDescribe(const QString& describe, quint64 userID);
    static void updateHeadPortrait(const QString& portrait, quint64 userID);
    static void parseDataField(CppSQLite3Query& src, UserDataField& dest);

private:
    static quint64 m_accountID;
};

#endif // USERDATASOURCE_H
