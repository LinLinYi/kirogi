#include "ToolKit.h"
#include "DClusterNotifyDataSource.h"
#include "DClusterNotifyInfoList.h"

DClusterNotifyInfoList::DClusterNotifyInfoList(QObject* parent) : QObject(parent)
{

}

void DClusterNotifyInfoList::append(char *data, MiCo::ClusterNotify type)
{
    if(data == NULL) return;
    DClusterNotifyInfo* info = new DClusterNotifyInfo;
    info->unpack(data, type);
    this->insertList(info);
    this->insertDB(info);
}

void DClusterNotifyInfoList::append(DClusterNotifyInfo *info)
{
    if(info == NULL) return;
    this->insertList(info);
    this->insertDB(info);
}

void DClusterNotifyInfoList::loadDataSource()
{
    CppSQLite3Query src;
    DClusterNotifyDataSource::queryData(src);

    while(!src.eof()){
        DClusterNotifyDataField dest;
        DClusterNotifyDataSource::parseDataField(src, dest);

        DClusterNotifyInfo* info = new DClusterNotifyInfo;
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

void DClusterNotifyInfoList::removeAll()
{
    if(infoList.size() == 0) return;
    for(int i = 0; i < infoList.size(); i++){
        DClusterNotifyInfo* info;
        info = infoList.at(i);
        infoList.removeAt(i);
        SAFE_DELETE(info);
    }
}

void DClusterNotifyInfoList::remove(DClusterNotifyInfo *info)
{
    if(info == NULL) return;
    for(int i = 0; i < infoList.size(); i++)
    {
        DClusterNotifyInfo* notifyInfo = infoList.at(i);
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

QList<DClusterNotifyInfo*>& DClusterNotifyInfoList::fetchList()
{
    return infoList;
}

void DClusterNotifyInfoList::insertList(DClusterNotifyInfo *info)
{
    if(info == NULL) return;

    //去掉重复的数据，然后更新或加入
    this->remove(info);
    infoList.append(info);
}

void DClusterNotifyInfoList::insertDB(DClusterNotifyInfo *info)
{
    DClusterNotifyDataField fields;
    fields.clusterID = info->getClusterID();
    fields.clusterName = info->getClusterName();
    fields.userID = info->getUserID();
    fields.userName = info->getUserName();
    fields.notifyContent = info->getNotifyContent();
    fields.notifyType = info->getNotifyType();
    fields.msgVerify = info->getMsgVerify();
    fields.notifyTime = info->getNotifyTime();
    fields.reviewStatus = info->getMsgStatus();
    DClusterNotifyDataSource::insertData(fields);
}

DClusterNotifyInfoList::~DClusterNotifyInfoList()
{
    for(int i = 0; i < infoList.size(); i++){
        DClusterNotifyInfo* info = infoList.at(i);
        SAFE_DELETE(info);
    }
}
