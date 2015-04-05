#include "DispersalData.h"
#include "UserAuthorizedInfoList.h"

UserAuthorizedInfoList::UserAuthorizedInfoList(QObject* parent) : QObject(parent){}

void UserAuthorizedInfoList::appendList(char *data, quint16 dataLen, Mi::SaveInfo)
{
    if(data == NULL || dataLen == 0) return;

    int offset = 0;
    quint64 clusterID = 0;
    quint64 deviceID = 0;
    quint16 userCount = 0;
    DispersalData dispersalData;

    offset += dispersalData.outlet(clusterID, data + offset);//设备群ID
    offset += dispersalData.outlet(deviceID, data + offset);//设备ID
    offset += dispersalData.outlet(userCount, data + offset);//设备的用户数目

    for(int i = 0; i < userCount; i++){
        UserAuthorizedInfo* userInfo = new UserAuthorizedInfo;
        offset += userInfo->unpack(data + offset);
        userInfo->setClusterID(clusterID);
        userInfo->setDeviceID(deviceID);

        //保证列表数据不重复
        quint64 uID = userInfo->getUserID();
        if(infoList.contains(uID))
           this->remove(uID);
        infoList.insert(uID, userInfo);
    }
}

void UserAuthorizedInfoList::append(char*, quint16)
{
    ///////////////////////////////////////////////////////////
}

QHash<quint64, UserAuthorizedInfo*>& UserAuthorizedInfoList::fetchList()
{
    return infoList;
}

void UserAuthorizedInfoList::append(UserAuthorizedInfo *info)
{
    if(info == NULL)
        return;

    quint64 infoID = info->getUserID();
    if(infoList.size() > 0 && infoList.contains(infoID)){
        UserAuthorizedInfo* temp = infoList.value(infoID);
        SAFE_DELETE(temp);
    }
    infoList.insert(infoID, info);
}

void UserAuthorizedInfoList::remove(UserAuthorizedInfo *info)
{
    if(info == NULL)
        return;

    quint64 infoID = info->getUserID();
    if(infoList.size() > 0 && infoList.contains(infoID)){
        UserAuthorizedInfo* temp = infoList.value(infoID);
        infoList.remove(infoID);
        SAFE_DELETE(temp);
    }
}

void UserAuthorizedInfoList::remove(quint64 removeID)
{
    if(infoList.size() > 0 && infoList.contains(removeID)){
        UserAuthorizedInfo* temp = infoList.value(removeID);
        infoList.remove(removeID);
        SAFE_DELETE(temp);
    }
}

UserAuthorizedInfoList::~UserAuthorizedInfoList()
{
    QHashIterator<quint64, UserAuthorizedInfo*> iter(infoList);
    while (iter.hasNext()) {
        iter.next();
        UserAuthorizedInfo* info = iter.value();
        SAFE_DELETE(info);
    }
}
