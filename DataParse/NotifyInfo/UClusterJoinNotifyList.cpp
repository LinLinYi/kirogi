#include "ToolKit.h"
#include "UClusterNotifyDataSource.h"
#include "UClusterJoinNotifyList.h"

UClusterJoinNotifyList::UClusterJoinNotifyList(QObject* parent) : QObject(parent)
{

}

void UClusterJoinNotifyList::append(char *data, Mi::ClusterJoin type)
{
    if(data == NULL) return;
    UClusterJoinNotify* info = new UClusterJoinNotify;
    info->unpack(data, type);
    this->insertList(info);
    this->insertDB(info);
}

void UClusterJoinNotifyList::append(UClusterJoinNotify *info)
{
    if(info == NULL) return;
    this->insertList(info);
    this->insertDB(info);
}

void UClusterJoinNotifyList::loadDataSource()
{
    CppSQLite3Query src;
    UClusterNotifyDataSource::queryData(src);

    while(!src.eof()){
        UClusterNotifyDataField dest;
        UClusterNotifyDataSource::parseDataField(src, dest);

        UClusterJoinNotify* info = new UClusterJoinNotify;
        Mi::ClusterJoin type = (Mi::ClusterJoin)(dest.notifyType);
        info->setNotifyType(type);

        info->setUserID(dest.userID);
        info->setUserName(dest.userName);
        info->setClusterID(dest.clusterID);
        info->setClusterName(dest.clusterName);
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

void UClusterJoinNotifyList::remove(UClusterJoinNotify *info)
{
    if(info == NULL) return;
    for(int i = 0; i < infoList.size(); i++)
    {
        UClusterJoinNotify* notifyInfo = infoList.at(i);
        if(info->getNotifyType() == notifyInfo->getNotifyType() &&
           info->getUserID() == notifyInfo->getUserID() &&
           info->getClusterID() == notifyInfo->getClusterID())
        {
            infoList.removeAt(i);
            SAFE_DELETE(notifyInfo);
            return;
        }
    }
}

QList<UClusterJoinNotify*>& UClusterJoinNotifyList::fetchList()
{
    return infoList;
}

void UClusterJoinNotifyList::insertList(UClusterJoinNotify *info)
{
    if(info == NULL) return;

    //去掉重复的数据，然后更新或加入
    this->remove(info);
    infoList.append(info);
}

void UClusterJoinNotifyList::insertDB(UClusterJoinNotify *info)
{
    UClusterNotifyDataField fields;
    fields.clusterID = info->getClusterID();
    fields.clusterName = info->getClusterName();
    fields.userID = info->getUserID();
    fields.userName = info->getUserName();
    fields.notifyContent = info->getNotifyContent();
    fields.notifyType = info->getNotifyType();
    fields.notifyVerify = info->getMsgVerify();
    fields.notifyTime = info->getNotifyTime();
    fields.status = info->getMsgStatus();
    UClusterNotifyDataSource::insertData(fields);
}

UClusterJoinNotifyList::~UClusterJoinNotifyList()
{
    for(int i = 0; i < infoList.size(); i++){
        UClusterJoinNotify* info = infoList.at(i);
        SAFE_DELETE(info);
    }
}
