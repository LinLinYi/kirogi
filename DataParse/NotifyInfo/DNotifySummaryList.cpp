#include "DNotifySummaryList.h"
#include "DNotifySummaryDataSource.h"

DNotifySummaryList::DNotifySummaryList(QObject* parent) : QObject(parent){}

void DNotifySummaryList::append(DNotifySummary* info)
{
    if(info == NULL) return;
    this->insertList(info);
    this->insertDB(info);
}

QList<DNotifySummary*>& DNotifySummaryList::fetchList()
{
    return infoList;
}

void DNotifySummaryList::loadDataSource()
{
    CppSQLite3Query src;
    DNotifySummaryDataSource::queryData(src);

    while(!src.eof()){
        DNotifySummaryDataField dest;
        DNotifySummaryDataSource::parseDataField(src, dest);

        DNotifySummary* info = new DNotifySummary;
        quint8 type = dest.notifyType;
        Mi::NotifyCategory notifyType = (Mi::NotifyCategory)type;

       info->setNotifyType(notifyType);
       info->setNotifyID(dest.notifyID);
       info->setNotifyName(dest.notifyName);
       info->setNotifyContent(dest.notifyContent);
       info->setNotifyTime(dest.notifyTime);
       info->setUnreadCount(dest.unreadCount);
       info->setNotifyImg(dest.portraitPath);

       this->insertList(info);
       src.nextRow();
    }
}

void DNotifySummaryList::remove(Mi::NotifyCategory type, quint64 notifyID)
{
    for(int i = 0; i < infoList.count(); i++){
        DNotifySummary* info = infoList.at(i);
        if(info->getNotifyType() == type && info->getNotifyID() == notifyID){
            DNotifySummaryDataSource::deleteData(type, notifyID);
            infoList.removeAt(i);
            SAFE_DELETE(info);
            return;
        }
    }
}

void DNotifySummaryList::insertList(DNotifySummary *info)
{
    if(info == NULL) return;
    this->remove(info->getNotifyType(), info->getNotifyID());
    infoList.append(info);
}

void DNotifySummaryList::insertDB(DNotifySummary *info)
{
    DNotifySummaryDataField fields;
    fields.portraitPath = info->getNotifyImg();
    fields.notifyContent = info->getNotifyContent();
    fields.notifyName = info->getNotifyName();
    fields.notifyTime = info->getNotifyTime();
    fields.notifyType = info->getNotifyType();
    fields.notifyID = info->getNotifyID();
    fields.unreadCount = info->getUnreadCount();
    DNotifySummaryDataSource::insertData(fields);
}

int DNotifySummaryList::getTotalUnreadCount()
{
    CppSQLite3Query src;
    DNotifySummaryDataSource::queryData(src);

    curTotalUnreadMsg = 0;
    while(!src.eof()){
        DNotifySummaryDataField dest;
        DNotifySummaryDataSource::parseDataField(src, dest);
        curTotalUnreadMsg += dest.unreadCount;
        src.nextRow();
    }
    return curTotalUnreadMsg;
}

DNotifySummaryList::~DNotifySummaryList()
{

}
