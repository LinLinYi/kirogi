#ifndef SQLITEHANDING_H
#define SQLITEHANDING_H

#include <string.h>
#include <QString>
#include <stdlib.h>
#include <stdio.h>
#include <QWidget>

#include "CppSQLite3.h"
#include "FilePath.h"
#include "SqliteConfig.hpp"

/*! \class SqliteManagment
 *  \brief sqlite3数据库操作的类
 */
class SqliteManagment
{
public:
    const char* m_dbPath;

public:
    bool createDbTable(QString& sqlCmd, QString& tableName);
    void query(QString& sqlCmd, CppSQLite3Query& outSqlData);

    quint32 insert(QString& sqlCmd);
    quint32 update(QString& sqlCmd);
    quint32 release(QString& sqlCmd);
    quint32 sqliteExec(QString& sqlCmd);
    quint64 getLastRowID();
    bool close();

public:
    static SqliteManagment* GetInstance(){
        if(m_pInstance == NULL){
            m_pInstance = new SqliteManagment();
        }
        return m_pInstance;
    }

    static void DeleteInstance(){
        if(m_pInstance){
            delete m_pInstance;
            m_pInstance = NULL;
        }
    }

private:
    static SqliteManagment * m_pInstance;
    CppSQLite3DB * m_pCppSQLite3DB;

private:
    SqliteManagment(void);
    ~SqliteManagment(void);

};

#endif // SQLITEHANDING_H
