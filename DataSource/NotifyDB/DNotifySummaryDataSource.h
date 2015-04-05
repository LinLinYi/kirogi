#ifndef DNOTIFYSUMMARYDATASOURCE_H
#define DNOTIFYSUMMARYDATASOURCE_H

#include "GlobalDefine.hpp"
#include "SqliteManagment.h"

typedef struct{
    quint64 accountID;
    quint8 notifyType;
    quint64 notifyID;
    QString notifyName;
    QString notifyContent;
    QString notifyTime;
    quint64 unreadCount;
    QString portraitPath;
}DNotifySummaryDataField;

class DNotifySummaryDataSource
{
public:
    explicit DNotifySummaryDataSource(){}
    virtual ~DNotifySummaryDataSource(){}

public:
    static void createTable(quint64 accountID);
    static void insertData(const DNotifySummaryDataField& fields);
    static void deleteData(Mi::NotifyCategory type, quint64 notifyID);
    static void queryData(CppSQLite3Query& outSqlData);
    static void updateUnreadCount(Mi::NotifyCategory type, quint64 notifyID, quint64 count);
    static void parseDataField(CppSQLite3Query& src, DNotifySummaryDataField& dest);

private:
    static quint64 m_accountID;

};

#endif // DNOTIFYSUMMARYDATASOURCE_H
