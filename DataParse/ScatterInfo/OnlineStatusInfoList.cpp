#include "OnlineStatusInfoList.h"
#include "DispersalData.h"

OnlineStatusInfoList::OnlineStatusInfoList(QObject *parent) : QObject(parent){}

void OnlineStatusInfoList::append(char *data, quint16 dataLen)
{
    if(data == NULL || dataLen == 0) return;
    OnlineStatusInfo* onlineInfo = new OnlineStatusInfo;
    onlineInfo->unpack(data);
    onlineList.insert(onlineInfo->getID(), onlineInfo);
}

void OnlineStatusInfoList::appendList(char *data, quint16 dataLen, bool isClusterMember)
{
    if(data == NULL || dataLen == 0) return;

    int offset = 0;
    quint16 onLineCount = 0;
    quint64 curClusterID = 0;
    DispersalData dispersalData;

    if(isClusterMember)
        offset += dispersalData.outlet(curClusterID, data + offset);

    offset += dispersalData.outlet(onLineCount, data + offset);
    for(quint16 i = 0; i != onLineCount; i++){
        OnlineStatusInfo* onlineInfo = new OnlineStatusInfo;
        offset += onlineInfo->unpack(data + offset);

        if(isClusterMember){
            onlineInfo->setClusterID(curClusterID);
        }
        onlineList.insert(onlineInfo->getID(), onlineInfo);
    }
}

QHash<quint64, OnlineStatusInfo*> OnlineStatusInfoList::fetchList()
{
    return onlineList;
}

void OnlineStatusInfoList::append(OnlineStatusInfo *info)
{
    if(info == NULL)
        return;

    quint64 infoID = info->getID();
    if(onlineList.size() > 0 && onlineList.contains(infoID)){
        OnlineStatusInfo* temp = onlineList.value(infoID);
        SAFE_DELETE(temp);
    }
    onlineList.insert(infoID, info);
}

void OnlineStatusInfoList::remove(OnlineStatusInfo *info)
{
    if(info == NULL)
        return;

    quint64 infoID = info->getID();
    if(onlineList.size() > 0 && onlineList.contains(infoID)){
        OnlineStatusInfo* temp = onlineList.value(infoID);
        onlineList.remove(infoID);
        SAFE_DELETE(temp);
    }
}

void OnlineStatusInfoList::remove(quint64 removeID)
{
    if(onlineList.size() > 0 && onlineList.contains(removeID)){
        OnlineStatusInfo* temp = onlineList.value(removeID);
        onlineList.remove(removeID);
        SAFE_DELETE(temp);
    }
}

OnlineStatusInfoList::~OnlineStatusInfoList()
{
    QHashIterator<quint64, OnlineStatusInfo*> iter(onlineList);
    while (iter.hasNext()) {
        iter.next();
        OnlineStatusInfo* info = iter.value();
        SAFE_DELETE(info);
    }
}

