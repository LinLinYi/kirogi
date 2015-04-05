#include "UserJoinNotifyDataSource.h"

quint64 UserJoinNotifyDataSource::m_accountID = 0;
void UserJoinNotifyDataSource::createTable(quint64 accountID)
{
    m_accountID = accountID;
    QString sqlCmd = QString("create table %1").arg(UT_USER_JOIN_NOTIFY);
    sqlCmd += QString("([accountID] INTEGER, [userID] INTEGER, [userName] TEXT, ");
    sqlCmd += QString("[notifyContent] TEXT, [notifyVerify] TEXT, ");
    sqlCmd += QString("[notifyTime] TEXT, [status] INTEGER, PRIMARY KEY([userID]))");

    QString tableName = UT_USER_JOIN_NOTIFY;
    SqliteManagment::GetInstance()->createDbTable(sqlCmd, tableName);
}

void UserJoinNotifyDataSource::queryData(CppSQLite3Query &outSqlData)
{
    QString sqlCmd = QString("select * from %1 ").arg(UT_USER_JOIN_NOTIFY);
    sqlCmd += QString("where [accountID] = %2").arg(m_accountID);
    SqliteManagment::GetInstance()->query(sqlCmd, outSqlData);
}

void UserJoinNotifyDataSource::insertData(const UserJoinNotifyDataField& fields)
{
    QString sqlCmd = QString("replace into %1").arg(UT_USER_JOIN_NOTIFY);
    sqlCmd += QString("([accountID], [userID], [userName], [notifyContent], [notifyVerify], ");
    sqlCmd += QString("[notifyTime], [status])values(%1, %2, '%3', '%4', '%5', '%6', %7)")
            .arg(m_accountID).arg(fields.userID).arg(fields.userName).arg(fields.notifyContent)
            .arg(fields.notifyVerify).arg(fields.notifyTime).arg(fields.status);
    SqliteManagment::GetInstance()->insert(sqlCmd);
}

void UserJoinNotifyDataSource::deleteAll()
{
    QString sqlCmd = QString("delete from %1").arg(UT_USER_JOIN_NOTIFY);
    sqlCmd += QString(" where [accountID] = %2").arg(m_accountID);
    SqliteManagment::GetInstance()->release(sqlCmd);
}

void UserJoinNotifyDataSource::deleteData(quint64 id)
{
    QString sqlCmd = QString("delete from %1").arg(UT_USER_JOIN_NOTIFY);
    sqlCmd += QString(" where [accountID] = %2 and [userID] = %3").arg(m_accountID).arg(id);
    SqliteManagment::GetInstance()->release(sqlCmd);
}

void UserJoinNotifyDataSource::updateCheckStatus(quint64 userID, quint8 rewStatus)
{
    QString sqlCmd = QString("update %1 set [status] = %2").arg(UT_USER_JOIN_NOTIFY).arg(rewStatus);
    sqlCmd += QString(" where [accountID] = %3 and [userID] = %4").arg(m_accountID).arg(userID);
    SqliteManagment::GetInstance()->update(sqlCmd);
}

void UserJoinNotifyDataSource::parseDataField(CppSQLite3Query &src, UserJoinNotifyDataField &dest)
{
    dest.accountID = atoll(src.fieldValue(0));
    dest.userID = atoll(src.fieldValue(1));
    dest.userName = src.fieldValue(2);
    dest.notifyContent = src.fieldValue(3);
    dest.notifyVerify = src.fieldValue(4);
    dest.notifyTime = src.fieldValue(5);
    dest.status = atoi(src.fieldValue(6));
}



