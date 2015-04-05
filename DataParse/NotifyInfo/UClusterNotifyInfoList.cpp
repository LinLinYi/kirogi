#include "ToolKit.h"
#include "UClusterNotifyDataSource.h"
#include "UClusterNotifyInfoList.h"

UClusterNotifyInfoList::UClusterNotifyInfoList(QObject* parent) : QObject(parent)
{

}

void UClusterNotifyInfoList::append(char *data, MiCo::ClusterNotify type)
{
    if(data == NULL) return;
    UClusterNotifyInfo* info = new UClusterNotifyInfo;
    info->unpack(data, type);
    this->insertList(info);
    this->insertDB(info);
}

void UClusterNotifyInfoList::append(UClusterNotifyInfo *info)
{
    if(info == NULL) return;
    this->insertList(info);
    this->insertDB(info);
}

void UClusterNotifyInfoList::loadDataSource()
{
    CppSQLite3Query src;
    UClusterNotifyDataSource::queryData(src);

    while(!src.eof()){
        UClusterNotifyDataField dest;
        UClusterNotifyDataSource::parseDataField(src, dest);

        UClusterNotifyInfo* info = new UClusterNotifyInfo;
        MiCo::ClusterNotify type = (MiCo::ClusterNotify)(dest.notifyType);
        info->setMsgType(type);

        info->setUserID(dest.userID);
        info->setUserName(dest.userName);
        info->setClusterID(dest.clusterID);
        info->setClusterName(dest.clusterName);
        info->setNotifyContent(dest.notifyContent);
        info->setMsgVerify(dest.msgVerify);
        info->setNotifyTime(dest.notifyTime);

        int reStatus = dest.reviewStatus;
        MiCo::MsgStatus status = (MiCo::MsgStatus)reStatus;
        info->setMsgStatus(status);
        this->insertList(info);
        src.nextRow();
    }
}

void UClusterNotifyInfoList::removeAll()
{
    if(infoList.size() == 0) return;
    for(int i = 0; i < infoList.size(); i++){
        UClusterNotifyInfo* info;
        info = infoList.at(i);
        infoList.removeAt(i);
        SAFE_DELETE(info);
    }
}

void UClusterNotifyInfoList::remove(UClusterNotifyInfo *info)
{
    if(info == NULL) return;
    for(int i = 0; i < infoList.size(); i++)
    {
        UClusterNotifyInfo* notifyInfo = infoList.at(i);
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

QList<UClusterNotifyInfo*>& UClusterNotifyInfoList::fetchList()
{
    return infoList;
}

void UClusterNotifyInfoList::insertList(UClusterNotifyInfo *info)
{
    if(info == NULL) return;

    //去掉重复的数据，然后更新或加入
    this->remove(info);
    infoList.append(info);
}

void UClusterNotifyInfoList::insertDB(UClusterNotifyInfo *info)
{
    UClusterNotifyDataField fields;
    fields.clusterID = info->getClusterID();
    fields.clusterName = info->getClusterName();
    fields.userID = info->getUserID();
    fields.userName = info->getUserName();
    fields.notifyContent = info->getNotifyContent();
    fields.notifyType = info->getNotifyType();
    fields.msgVerify = info->getMsgVerify();
    fields.notifyTime = info->getNotifyTime();
    fields.reviewStatus = info->getMsgStatus();
    UClusterNotifyDataSource::insertData(fields);
}

UClusterNotifyInfoList::~UClusterNotifyInfoList()
{
    for(int i = 0; i < infoList.size(); i++){
        UClusterNotifyInfo* info = infoList.at(i);
        SAFE_DELETE(info);
    }
}
