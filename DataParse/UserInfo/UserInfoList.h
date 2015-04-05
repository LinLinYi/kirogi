#ifndef USERINFOLIST_H
#define USERINFOLIST_H

#include <QHash>
#include "UserInfo.h"

/*! \class UserInfoList
 *  \brief 用来管理用户信息的类。
 */
class UserInfoList
{
public:
    explicit UserInfoList();
    virtual ~UserInfoList();

public:
    void loadDataSource();
    void appendList(char* data, quint16 dataLen);
    void append(char *data, quint16 dataLen);
    void append(UserInfo* info);
    void remove(quint64 removeID);
    void remove(UserInfo* info);
    QHash<quint64, UserInfo*>& fetchList();
    int count();

private:
    QHash<quint64, UserInfo*> infoList;

private:
    void insertList(UserInfo* info);
    void insertDB(UserInfo* info);
};

#endif // USERINFOLIST_H
