#include "ToolKit.h"
#include "DClusterNotifyDataSource.h"
#include "DClusterJoinNotifyList.h"

DClusterJoinNotifyList::DClusterJoinNotifyList(QObject* parent) : QObject(parent)
{

}

void DClusterJoinNotifyList::append(char *data, Mi::ClusterJoin type)
{
    if(data == NULL) return;
    DClusterJoinNotify* info = new DClusterJoinNotify;
    info->unpack(data, type);
    this->insertList(info);
    this->insertDB(info);
}

void DClusterJoinNotifyList::append(DClusterJoinNotify *info)
{
    if(info == NULL) return;
    this->insertList(info);
    this->insertDB(info);
}

void DClusterJoinNotifyList::loadDataSource()
{
    CppSQLite3Query src;
    DClusterNotifyDataSource::queryData(src);

    while(!src.eof()){
        DClusterNotifyDataField dest;
        DClusterNotifyDataSource::parseDataField(src, dest);

        DClusterJoinNotify* info = new DClusterJoinNotify;
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

void DClusterJoinNotifyList::remove(DClusterJoinNotify *info)
{
    if(info == NULL) return;
    for(int i = 0; i < infoList.size(); i++)
    {
        DClusterJoinNotify* notifyInfo = infoList.at(i);
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

QList<DClusterJoinNotify*>& DClusterJoinNotifyList::fetchList()
{
    return infoList;
}

void DClusterJoinNotifyList::insertList(DClusterJoinNotify *info)
{
    if(info == NULL) return;

    //去掉重复的数据，然后更新或加入
    this->remove(info);
    infoList.append(info);
}

void DClusterJoinNotifyList::insertDB(DClusterJoinNotify *info)
{
    DClusterNotifyDataField fields;
    fields.clusterID = info->getClusterID();
    fields.clusterName = info->getClusterName();
    fields.userID = info->getUserID();
    fields.userName = info->getUserName();
    fields.notifyContent = info->getNotifyContent();
    fields.notifyType = info->getNotifyType();
    fields.notifyVerify = info->getMsgVerify();
    fields.notifyTime = info->getNotifyTime();
    fields.status = info->getMsgStatus();
    DClusterNotifyDataSource::insertData(fields);
}

DClusterJoinNotifyList::~DClusterJoinNotifyList()
{

}
