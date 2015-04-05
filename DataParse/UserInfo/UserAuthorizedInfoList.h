#ifndef USERAUTHORIZEDINFOLIST_H
#define USERAUTHORIZEDINFOLIST_H

#include "UserAuthorizedInfo.h"

class UserAuthorizedInfoList : public QObject
{
    Q_OBJECT
public:
    explicit UserAuthorizedInfoList(QObject* parent = 0);
    virtual ~UserAuthorizedInfoList();

public:
    void appendList(char* data, quint16 dataLen,  Mi::SaveInfo type =  Mi::SaveAll);
    void append(char* data, quint16 dataLen);
    void append(UserAuthorizedInfo* info);
    void remove(quint64 removeID);
    void remove(UserAuthorizedInfo* info);
    QHash<quint64, UserAuthorizedInfo*>& fetchList();

private:
    QHash<quint64, UserAuthorizedInfo*> infoList;
};

#endif // USERAUTHORIZEDINFOLIST_H
