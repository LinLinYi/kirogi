#ifndef USERJOINNOTIFYLIST_H
#define USERJOINNOTIFYLIST_H

#include "UserJoinNotify.h"

class UserJoinNotifyList
{
public:
    explicit UserJoinNotifyList();
    virtual ~UserJoinNotifyList();

public:
    void append(char* data, Mi::UserJoin type);
    void append(UserJoinNotify* info);
    void loadDataSource();
    void remove(UserJoinNotify* info);
    void remove(quint64 id);
    QHash<quint64, UserJoinNotify*>& fetchList();

private:
    QHash<quint64, UserJoinNotify*> infoList;

private:
    void insertList(UserJoinNotify* info);
    void insertDB(UserJoinNotify* info);
};

#endif // USERJOINNOTIFYLIST_H
