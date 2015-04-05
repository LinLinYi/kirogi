#include "GlobalData.h"

GlobalData::GlobalData(){}
quint64 GlobalData::m_accountID = 0;
QString GlobalData::m_accountName = "...";
quint64 GlobalData::getAccountID()
{
    return m_accountID;
}

QString& GlobalData::getAccountName()
{
    return m_accountName;
}

void GlobalData::setAccountID(quint64 id)
{
    m_accountID = id;
}

void GlobalData::setAccountName(QString& name)
{
    m_accountName = name;
}

GlobalData::~GlobalData(){}
