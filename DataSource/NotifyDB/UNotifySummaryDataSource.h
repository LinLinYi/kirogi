#ifndef UNOTIFYSUMMARYDATASOURCE_H
#define UNOTIFYSUMMARYDATASOURCE_H

#include "SqliteManagment.h"
#include "GlobalDefine.hpp"

typedef struct{
    quint64 accountID;
    quint8 notifyType;
    quint64 notifyID;
    QString notifyName;
    QString notifyContent;
    QString notifyTime;
    quint64 unreadCount;
    QString portraitPath;
}UNotifySummaryDataField;

class UNotifySummaryDataSource
{
public:
    explicit UNotifySummaryDataSource(){}
    virtual ~UNotifySummaryDataSource(){}

public:
    static void createTable(quint64);
    static void insertData(const UNotifySummaryDataField& fields);
    static void deleteData(Mi::NotifyCategory type, quint64 notifyID);
    static void queryData(CppSQLite3Query&);
    static void updateUnreadCount(Mi::NotifyCategory, quint64, quint64);
    static void parseDataField(CppSQLite3Query &src, UNotifySummaryDataField &dest);

private:
    static quint64 m_accountID;
};

#endif // UNOTIFYSUMMARYDATASOURCE_H
