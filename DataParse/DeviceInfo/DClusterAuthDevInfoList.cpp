#include "DispersalData.h"
#include "DClusterAuthDevInfoList.h"

DClusterAuthDevInfoList::DClusterAuthDevInfoList(QObject* parent) : QObject(parent){}

void DClusterAuthDevInfoList::appendList(char *data, quint16 dataLen,  Mi::SaveInfo)
{
    if(data == NULL || dataLen == 0)
       return;

    int offset = 0;
    quint64 clusterID = 0;
    quint64 userID = 0;
    quint16 deviceCount = 0;
    DispersalData dispersalData;

    offset += dispersalData.outlet(clusterID, data + offset);//设备群ID
    offset += dispersalData.outlet(userID, data + offset);//用户ID
    offset += dispersalData.outlet(deviceCount, data + offset);//设备群的设备数目

    for(int i = 0; i < deviceCount; i++){
        DClusterAuthDevInfo* devInfo = new DClusterAuthDevInfo;
        offset += devInfo->unpack(data + offset);
        devInfo->setClusterID(clusterID);
        devInfo->setUserID(userID);
        infoList.insert(devInfo->getDeviceID(), devInfo);
    }
}

void DClusterAuthDevInfoList::append(char*, quint16)
{
    ///////////////////////////////////////////////////////////
}

QHash<quint64, DClusterAuthDevInfo*>& DClusterAuthDevInfoList::fetchList()
{
    return infoList;
}

void DClusterAuthDevInfoList::append(DClusterAuthDevInfo *info)
{
    if(info == NULL)
        return;

    quint64 infoID = info->getDeviceID();
    if(infoList.size() > 0 && infoList.contains(infoID)){
        DClusterAuthDevInfo* temp = infoList.value(infoID);
        SAFE_DELETE(temp);
    }
    infoList.insert(infoID, info);
}

void DClusterAuthDevInfoList::remove(DClusterAuthDevInfo *info)
{
    if(info == NULL)
        return;

    quint64 infoID = info->getDeviceID();
    if(infoList.size() > 0 && infoList.contains(infoID)){
        DClusterAuthDevInfo* temp = infoList.value(infoID);
        infoList.remove(infoID);
        SAFE_DELETE(temp);
    }
}

void DClusterAuthDevInfoList::remove(quint64 removeID)
{
    if(infoList.size() > 0 && infoList.contains(removeID)){
        DClusterAuthDevInfo* temp = infoList.value(removeID);
        infoList.remove(removeID);
        SAFE_DELETE(temp);
    }
}

DClusterAuthDevInfoList::~DClusterAuthDevInfoList()
{
    QHashIterator<quint64, DClusterAuthDevInfo*> iter(infoList);
    while (iter.hasNext()) {
        iter.next();
        DClusterAuthDevInfo* info = iter.value();
        SAFE_DELETE(info);
    }
}

