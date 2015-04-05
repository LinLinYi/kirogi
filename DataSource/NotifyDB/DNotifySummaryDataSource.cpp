#include "DNotifySummaryDataSource.h"

quint64 DNotifySummaryDataSource::m_accountID = 0;
void DNotifySummaryDataSource::createTable(quint64 accountID)
{
    m_accountID = accountID;
    QString sqlCmd = QString("create table %1").arg(DT_SUMMARY_NOTIFY);
    sqlCmd += QString("([accountID] INTEGER, [notifyType] INTEGER, [notifyID] INTEGER DEFAULT (0), [notifyName] TEXT, [notifyContent] TEXT, ");
    sqlCmd += QString("[notifyTime] TEXT, [unreadCount] INTEGER DEFAULT (0), [portraitPath] TEXT, PRIMARY KEY([accountID], ");
    sqlCmd += QString("[notifyType], [notifyID]))");

    QString tableName = DT_SUMMARY_NOTIFY;
    SqliteManagment::GetInstance()->createDbTable(sqlCmd, tableName);
}

void DNotifySummaryDataSource::queryData(CppSQLite3Query& outSqlData)
{
    QString sqlCmd = QString("select * from %1 ").arg(DT_SUMMARY_NOTIFY);
    sqlCmd += QString("where [accountID] = %2").arg(m_accountID);
    SqliteManagment::GetInstance()->query(sqlCmd, outSqlData);
}

void DNotifySummaryDataSource::insertData(const DNotifySummaryDataField& fields)
{
    //查询判断是否存在群通知记录
    QString sqlCmd = QString("replace into %1").arg(DT_SUMMARY_NOTIFY);
    sqlCmd += QString("([accountID], [notifyType], [notifyID], [notifyName], [notifyContent], [notifyTime], [unreadCount], [portraitPath])");
    sqlCmd += QString("values(%1, %2, %3, '%4', '%5', '%6', %7, '%8')").arg(m_accountID).arg(fields.notifyType).arg(fields.notifyID)
            .arg(fields.notifyName).arg(fields.notifyContent).arg(fields.notifyTime).arg(fields.unreadCount).arg(fields.portraitPath);
    SqliteManagment::GetInstance()->insert(sqlCmd);
}

void DNotifySummaryDataSource::deleteData(Mi::NotifyCategory type, quint64 notifyID)
{
    QString sqlCmd = QString("delete from %1 ").arg(DT_SUMMARY_NOTIFY);
    sqlCmd += QString("where [accountID] = %2 and [notifyType] = %3 and [notifyID] = %4").arg(m_accountID).arg(type).arg(notifyID);
    SqliteManagment::GetInstance()->release(sqlCmd);
}

void DNotifySummaryDataSource::updateUnreadCount(Mi::NotifyCategory type, quint64 notifyID, quint64 count)
{
    QString sqlCmd = QString("update %1 set [unreadCount] = %2").arg(DT_SUMMARY_NOTIFY).arg(count);
    sqlCmd += QString(" where [accountID] = %3 and [notifyType] = %4 and [notifyID] = %5")
            .arg(m_accountID).arg(type).arg(notifyID);
    SqliteManagment::GetInstance()->update(sqlCmd);
}

void DNotifySummaryDataSource::parseDataField(CppSQLite3Query &src, DNotifySummaryDataField &dest)
{
    dest.accountID = atoll(src.fieldValue(0));
    dest.notifyType = atoll(src.fieldValue(1));
    dest.notifyID = atoll(src.fieldValue(2));
    dest.notifyName = src.fieldValue(3);
    dest.notifyContent = src.fieldValue(4);
    dest.notifyTime = src.fieldValue(5);
    dest.unreadCount = atoll(src.fieldValue(6));
    dest.portraitPath = src.fieldValue(7);
}

