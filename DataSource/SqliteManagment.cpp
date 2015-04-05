#include <QCoreApplication>
#include <assert.h>

#include "ToolKit.h"
#include "SqliteManagment.h"

SqliteManagment* SqliteManagment::m_pInstance = NULL;

SqliteManagment::SqliteManagment(void)
{
    m_pCppSQLite3DB = new CppSQLite3DB;
    if(m_pCppSQLite3DB == NULL)
        assert(m_pCppSQLite3DB);
}

bool SqliteManagment::createDbTable(QString& sqlCmd, QString& tableName)
{
    if(m_pCppSQLite3DB->open(FilePath::databaseFilePath()) != 0)
        return false;

    std::string strSqlCmd = sqlCmd.toStdString();
    const char* chSqlCmd = strSqlCmd.c_str();
    char* exSqlCmd = const_cast<char*>(chSqlCmd);

    std::string strValue = tableName.toStdString();
    const char* chValue = strValue.c_str();

    if (!m_pCppSQLite3DB->tableExists(chValue)){
        m_pCppSQLite3DB->execDML(exSqlCmd);
    }

    m_pCppSQLite3DB->close();
    return true;
}

void SqliteManagment::query(QString& sqlCmd, CppSQLite3Query& outSqlData)
{
    if(m_pCppSQLite3DB->open(FilePath::databaseFilePath()) != 0)
        return;

    std::string strSqlCmd = sqlCmd.toStdString();
    const char* chSqlCmd = strSqlCmd.c_str();
    char* exSqlCmd = const_cast<char*>(chSqlCmd);
    outSqlData = m_pCppSQLite3DB->execQuery(exSqlCmd);
}

quint32 SqliteManagment::sqliteExec(QString& sqlCmd)
{
    if(m_pCppSQLite3DB->open(FilePath::databaseFilePath()) != 0)
        return 0;

    quint32 ret;
    std::string strSqlCmd = sqlCmd.toStdString();
    const char* chSqlCmd = strSqlCmd.c_str();
    char* exSqlCmd = const_cast<char*>(chSqlCmd);
    ret = m_pCppSQLite3DB->execDML(exSqlCmd);
    m_pCppSQLite3DB->close();
    return ret;
}

quint64 SqliteManagment::getLastRowID()
{
    return m_pCppSQLite3DB->lastRowId();
}

quint32 SqliteManagment::insert(QString& sqlCmd)
{
    return sqliteExec(sqlCmd);
}

quint32 SqliteManagment::update(QString& sqlCmd)
{
    return sqliteExec(sqlCmd);
}

quint32 SqliteManagment::release(QString& sqlCmd)
{
    return sqliteExec(sqlCmd);
}

bool SqliteManagment::close()
{
    m_pCppSQLite3DB->close();
    return true;
}

SqliteManagment::~SqliteManagment(void)
{

}
