#include "UserDataSource.h"

quint64 UserDataSource::m_accountID = 0;
void UserDataSource::createTable(quint64 accountID)
{
    m_accountID = accountID;
    QString sqlCmd = QString("create table %1 ([accountID] INTEGER, [userID] INTEGER, ").arg(UT_USER_INFO);
    sqlCmd += QString("[userName] TEXT, [remarks] TEXT, [describe] TEXT, [userMail] TEXT, ");
    sqlCmd += QString("[userPortrait], PRIMARY KEY([accountID], [userID]))");

    QString tableName = UT_USER_INFO;
    SqliteManagment::GetInstance()->createDbTable(sqlCmd, tableName);
}

void UserDataSource::queryData(CppSQLite3Query& outSqlData)
{
    QString sqlCmd = QString("select * from %1 ").arg(UT_USER_INFO);
    sqlCmd += QString("where [accountID] = %2").arg(m_accountID);
    SqliteManagment::GetInstance()->query(sqlCmd, outSqlData);
}

void UserDataSource::queryData(CppSQLite3Query &outSqlData, quint64 userID)
{
    QString sqlCmd = QString("select * from %1 ").arg(UT_USER_INFO);
    sqlCmd += QString("where [accountID] = %2 and [userID] = %3").arg(m_accountID).arg(userID);
    SqliteManagment::GetInstance()->query(sqlCmd, outSqlData);
}

void UserDataSource::insertData(UserDataField& fields)
{
    CppSQLite3Query outSqlData;
    QString queryCmd = QString("select * from %1 where [accountID] = %2 and [userID] = %3")
            .arg(UT_USER_INFO).arg(m_accountID).arg(fields.userID);
    SqliteManagment::GetInstance()->query(queryCmd, outSqlData);

    while(!outSqlData.eof()){
        fields.userPortrait = outSqlData.fieldValue(6);
        outSqlData.nextRow();
    }//保证本地头像路径不被修改

    QString sqlCmd = QString("replace into %1").arg(UT_USER_INFO);
    sqlCmd += QString("([accountID], [userID], [userName], [remarks], [describe], [userMail], ");
    sqlCmd += QString("[userPortrait])values(%1, %2,'%3', '%4', '%5', '%6', '%7')").arg(m_accountID)
            .arg(fields.userID).arg(fields.userName).arg(fields.remarks).arg(fields.describe)
            .arg(fields.userMail).arg(fields.userPortrait);
   SqliteManagment::GetInstance()->insert(sqlCmd);
}

void UserDataSource::updateRemarks(const QString &remarks, quint64 userID)
{
    QString sqlCmd = QString("update %1 ").arg(UT_USER_INFO);
    sqlCmd += QString("set [remarks] = '%2' where [accountID] = %3 and [userID] = %4")
            .arg(remarks).arg(m_accountID).arg(userID);
    SqliteManagment::GetInstance()->update(sqlCmd);
}

void UserDataSource::updateDescribe(const QString &describe, quint64 userID)
{
    QString sqlCmd = QString("update %1 ").arg(UT_USER_INFO);
    sqlCmd += QString("set [describe] = '%2' where [accountID] = %3 and [userID] = %4")
            .arg(describe).arg(m_accountID).arg(userID);
    SqliteManagment::GetInstance()->update(sqlCmd);
}

void UserDataSource::updateHeadPortrait(const QString &portrait, quint64 userID)
{
    QString sqlCmd = QString("update %1 ").arg(UT_USER_INFO);
    sqlCmd += QString("set [userPortrait] = '%2' where [accountID] = %3 and [userID] = %4")
            .arg(portrait).arg(m_accountID).arg(userID);
    SqliteManagment::GetInstance()->update(sqlCmd);
}

void UserDataSource::deleteData(quint64 userID)
{
    QString sqlCmd = QString("delete from %1 ").arg(UT_USER_INFO);
    sqlCmd += QString("where [accountID] = %2 and [deviceID] = %3").arg(m_accountID).arg(userID);
    SqliteManagment::GetInstance()->release(sqlCmd);
}

void UserDataSource::parseDataField(CppSQLite3Query &src, UserDataField &dest)
{
    dest.accountID = atoll(src.fieldValue(0));
    dest.userID = atoll(src.fieldValue(1));
    dest.userName = src.fieldValue(2);
    dest.remarks = src.fieldValue(3);
    dest.describe = src.fieldValue(4);
    dest.userMail = src.fieldValue(5);
    dest.userPortrait = src.fieldValue(6);
}
