#ifndef GLOBALDATA_H
#define GLOBALDATA_H

#include <QObject>

class GlobalData
{
public:
    GlobalData();
    ~GlobalData();

public:
    static quint64 getAccountID();
    static QString& getAccountName();
    static void setAccountID(quint64 id);
    static void setAccountName(QString& name);

private:
    static quint64 m_accountID;
    static QString m_accountName;
};

#endif // GLOBALDATA_H
