#include "UserInfoList.h"
#include "UserDataSource.h"
#include "DispersalData.h"

UserInfoList::UserInfoList(){}

void UserInfoList::appendList(char *data, quint16 dataLen)
{
    if(data == NULL || dataLen == 0) return;

    int offset = 0;
    quint16 userCount = 0;
    DispersalData dispersalData;
    offset += dispersalData.outlet(userCount, data + offset);//用户组的成员数目

    for(int i = 0; i < userCount; i++){
        UserInfo* userInfo = new UserInfo;
        offset += userInfo->unpack(data + offset);
        this->insertList(userInfo);
        this->insertDB(userInfo);
    }
}

void UserInfoList::append(char *data, quint16 dataLen)
{
    if(data == NULL || dataLen == 0) return;
    DPRINT("-->Added user to the list successed.");

    //添加用户信息
    UserInfo* userInfo = new UserInfo;
    userInfo->unpack(data);
    this->insertList(userInfo);
    this->insertDB(userInfo);
}

QHash<quint64, UserInfo*>& UserInfoList::fetchList()
{
    return infoList;
}

void UserInfoList::append(UserInfo *info)
{
    if(info == NULL) return;
    this->insertList(info);
    this->insertDB(info);
}

void UserInfoList::remove(UserInfo *info)
{
    if(info == NULL)
        return;

    quint64 infoID = info->getUserID();
    if(infoList.size() > 0 && infoList.contains(infoID)){
        UserInfo* temp = infoList.value(infoID);
        infoList.remove(infoID);
        SAFE_DELETE(temp);
    }
}

void UserInfoList::remove(quint64 removeID)
{
    if(infoList.size() > 0 && infoList.contains(removeID)){
        UserInfo* temp = infoList.value(removeID);
        infoList.remove(removeID);
        SAFE_DELETE(temp);
    }
}

void UserInfoList::loadDataSource()
{
    CppSQLite3Query src;
    UserDataSource::queryData(src);

    while(!src.eof()){
        UserDataField dest;
        UserDataSource::parseDataField(src, dest);

        UserInfo* info = new UserInfo;
        info->setUserID(dest.userID);
        info->setUserName(dest.userName);
        info->setRemarks(dest.remarks);
        info->setDescribe(dest.describe);
        this->insertList(info);
        src.nextRow();
    }
}

int UserInfoList::count()
{
    return infoList.count();
}

void UserInfoList::insertList(UserInfo *info)
{
    //保证列表数据不重复
    quint64 usrID = info->getUserID();
    if(infoList.contains(usrID)){
        this->remove(usrID);
    }
    infoList.insert(usrID, info);
}

void UserInfoList::insertDB(UserInfo *info)
{
    UserDataField fields;
    fields.describe = info->getDescribe();
    fields.remarks = info->getRemarks();
    fields.userID = info->getUserID();
    fields.userMail = info->getUserMail();
    fields.userName = info->getUserName();
    fields.userPortrait = info->getUserProtrait();
    UserDataSource::insertData(fields);
}

UserInfoList::~UserInfoList()
{
    if(!infoList.isEmpty())
    {
        UserInfo * userInfo;
        QHashIterator<quint64, UserInfo*> iter(infoList);

        while(iter.hasNext())
        {
            iter.next();
            userInfo = dynamic_cast<UserInfo*>(iter.value());
            SAFE_DELETE(userInfo);
        }
    }
}
