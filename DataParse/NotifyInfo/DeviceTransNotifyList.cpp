#include "ToolKit.h"
#include "DeviceTransNotifyDataSource.h"
#include "DeviceTransNotifyList.h"

DeviceTransNotifyList::DeviceTransNotifyList(QObject* parent) : QObject(parent)
{

}

void DeviceTransNotifyList::append(DeviceTransNotify *info)
{
    if(info == NULL) return;
    this->insertList(info);
    this->insertDB(info);
}

void DeviceTransNotifyList::removeAll()
{
    if(infoList.size() == 0) return;
    for(int i = 0; i < infoList.size(); i++){
        DeviceTransNotify* info;
        info = infoList.at(i);
        infoList.removeAt(i);
        SAFE_DELETE(info);
    }
}

void DeviceTransNotifyList::remove(DeviceTransNotify *info)
{
    if(info == NULL) return;
    for(int i = 0; i < infoList.size(); i++)
    {
        DeviceTransNotify* notifyInfo = infoList.at(i);
        if(info->getSrcUserID() == notifyInfo->getSrcUserID() &&
           info->getSrcClusterID() == notifyInfo->getSrcClusterID() &&
           info->getDestClusterID() == notifyInfo->getDestClusterID())
        {
            infoList.removeAt(i);
            SAFE_DELETE(notifyInfo);
            return;
        }
    }
}

QList<DeviceTransNotify*> DeviceTransNotifyList::fetchList()
{
    return infoList;
}

void DeviceTransNotifyList::insertList(DeviceTransNotify *info)
{
    if(info == NULL) return;

    //去掉重复的数据，然后更新或加入
    this->remove(info);
    infoList.append(info);
}

void DeviceTransNotifyList::insertDB(DeviceTransNotify *info)
{
    if(info == NULL) return;
    DeviceTransNotifyDataField fields;
    fields.srcClusterID = info->getSrcClusterID();
    fields.srcClusterName = info->getSrcClusterName();
    fields.notifyTime = info->getNotifyTime();
    fields.status = info->getReviewStatus();
    fields.destClusterID = info->getDestClusterID();
    fields.destClusterName = info->getDestClusterName();
    fields.srcUserID = info->getSrcUserID();
    fields.srcUserName = info->getSrcUserName();
    DeviceTransNotifyDataSource::insertData(fields);
}

void DeviceTransNotifyList::loadDataSource()
{
    CppSQLite3Query src;
    DeviceTransNotifyDataSource::queryData(src);

    while(!src.eof()){
       DeviceTransNotifyDataField dest;
       DeviceTransNotifyDataSource::parseDataField(src, dest);

       DeviceTransNotify* info = new DeviceTransNotify;
       info->setTransUserID(dest.srcUserID);
       info->setTransUserName(dest.srcUserName);
       info->setSrcClusterID(dest.srcClusterID);
       info->setSrcClusterName(dest.srcClusterName);
       info->setDestClusterID(dest.destClusterID);
       info->setDestClusterName(dest.destClusterName);
       info->setNotifyContent(dest.notifyContent);
       info->setNotifyVerify(dest.notifyVerify);
       info->setNotifyTime(dest.notifyTime);

       int reStatus = dest.status;
       Mi::MsgStatus status = (Mi::MsgStatus)reStatus;
       info->setReviewStatus(status);

       this->insertList(info);
       src.nextRow();
    }
}

DeviceTransNotifyList::~DeviceTransNotifyList()
{
//    for(int i = 0; i < infoList.size(); i++){
//        DeviceTransNotify* info = infoList.at(i);
//        SAFE_DELETE(info);
//    }
}
