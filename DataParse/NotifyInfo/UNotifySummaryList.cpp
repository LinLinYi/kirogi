#include "UNotifySummaryList.h"
#include "UNotifySummaryDataSource.h"

UNotifySummaryList::UNotifySummaryList(QObject* parent)
    : QObject(parent)
    , curTotalUnreadMsg(0)
{

}

void UNotifySummaryList::append(UNotifySummary* info)
{
    if(info == NULL) return;
    this->insertList(info);
    this->insertDB(info);
}

QList<UNotifySummary*>& UNotifySummaryList::fetchList()
{
    return infoList;
}

void UNotifySummaryList::loadDataSource()
{
    CppSQLite3Query src;
    UNotifySummaryDataSource::queryData(src);

    while(!src.eof()){
        UNotifySummaryDataField dest;
        UNotifySummaryDataSource::parseDataField(src, dest);

        UNotifySummary* info = new UNotifySummary;
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

int UNotifySummaryList::getTotalUnreadCount()
{
    CppSQLite3Query src;
    UNotifySummaryDataSource::queryData(src);

    curTotalUnreadMsg = 0;
    while(!src.eof()){
        UNotifySummaryDataField dest;
        UNotifySummaryDataSource::parseDataField(src, dest);
        curTotalUnreadMsg += dest.unreadCount;
        src.nextRow();
    }
    return curTotalUnreadMsg;
}

void UNotifySummaryList::remove(Mi::NotifyCategory type, quint64 notifyID)
{
    for(int i = 0; i < infoList.count(); i++){
        UNotifySummary* info = infoList.at(i);
        if(info->getNotifyType() == type && info->getNotifyID() == notifyID){
            UNotifySummaryDataSource::deleteData(type, notifyID);
            infoList.removeAt(i);
            SAFE_DELETE(info);
            return;
        }
    }
}

void UNotifySummaryList::insertList(UNotifySummary *info)
{
    if(info == NULL) return;
    this->remove(info->getNotifyType(), info->getNotifyID());
    infoList.insert(0, info);
}

void UNotifySummaryList::insertDB(UNotifySummary *info)
{
    UNotifySummaryDataField fields;
    fields.portraitPath = info->getNotifyImg();
    fields.notifyContent = info->getNotifyContent();
    fields.notifyName = info->getNotifyName();
    fields.notifyTime = info->getNotifyTime();
    fields.notifyType = info->getNotifyType();
    fields.notifyID = info->getNotifyID();
    fields.unreadCount = info->getUnreadCount();
    UNotifySummaryDataSource::insertData(fields);
}

UNotifySummaryList::~UNotifySummaryList()
{

}
