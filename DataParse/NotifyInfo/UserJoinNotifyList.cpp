#include "UserJoinNotifyList.h"

#include "ToolKit.h"
#include "UserJoinNotifyDataSource.h"
#include "UserJoinNotifyList.h"

UserJoinNotifyList::UserJoinNotifyList()
{

}

void UserJoinNotifyList::append(char *data, Mi::UserJoin type)
{
    if(data == NULL) return;
    UserJoinNotify* info = new UserJoinNotify;
    info->unpack(data, type);
    this->insertList(info);
    this->insertDB(info);
}

void UserJoinNotifyList::append(UserJoinNotify *info)
{
    if(info == NULL) return;
    this->insertList(info);
    this->insertDB(info);
}

void UserJoinNotifyList::loadDataSource()
{
    CppSQLite3Query src;
    UserJoinNotifyDataSource::queryData(src);

    while(!src.eof()){
        UserJoinNotifyDataField dest;
        UserJoinNotifyDataSource::parseDataField(src, dest);
        UserJoinNotify* info = new UserJoinNotify;

        info->setUserID(dest.userID);
        info->setUserName(dest.userName);
        info->setNotifyContent(dest.notifyContent);
        info->setNotifyVerify(dest.notifyVerify);
        info->setNotifyTime(dest.notifyTime);

        int reStatus = dest.status;
        Mi::MsgStatus status = (Mi::MsgStatus)reStatus;
        info->setNotifyStatus(status);
        this->insertList(info);
        src.nextRow();
    }
}

void UserJoinNotifyList::remove(UserJoinNotify *info)
{
    if(info == NULL) return;
    quint64 removeID = info->getUserID();

    if(infoList.size() > 0 && infoList.contains(removeID)){
        UserJoinNotify* temp = infoList.value(removeID);
        infoList.remove(removeID);
        SAFE_DELETE(temp);
    }
    UserJoinNotifyDataSource::deleteData(removeID);
}

void UserJoinNotifyList::remove(quint64 id)
{
    if(infoList.size() > 0 && infoList.contains(id)){
        UserJoinNotify* temp = infoList.value(id);
        infoList.remove(id);
        SAFE_DELETE(temp);
    }
    UserJoinNotifyDataSource::deleteData(id);
}

QHash<quint64, UserJoinNotify*>& UserJoinNotifyList::fetchList()
{
    return infoList;
}

void UserJoinNotifyList::insertList(UserJoinNotify *info)
{
    //保证列表数据不重复
    quint64 usrID = info->getUserID();
    if(infoList.contains(usrID)){
        this->remove(usrID);
    }
    infoList.insert(usrID, info);
}

void UserJoinNotifyList::insertDB(UserJoinNotify *info)
{
    UserJoinNotifyDataField fields;
    fields.userID = info->getUserID();
    fields.userName = info->getUserName();
    fields.notifyContent = info->getNotifyContent();
    fields.notifyVerify = info->getMsgVerify();
    fields.notifyTime = info->getNotifyTime();
    fields.status = info->getMsgStatus();
    UserJoinNotifyDataSource::insertData(fields);
}

UserJoinNotifyList::~UserJoinNotifyList()
{
    QHashIterator<quint64, UserJoinNotify*> iter(infoList);
    while (iter.hasNext()) {
        iter.next();
        UserJoinNotify* info = iter.value();
        SAFE_DELETE(info);
    }
}
