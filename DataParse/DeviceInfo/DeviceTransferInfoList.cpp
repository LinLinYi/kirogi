#include "DeviceTransferInfoList.h"
#include "DeviceTransferDataSource.h"
#include "DispersalData.h"
#include "GlobalDefine.hpp"

DeviceTransferInfoList::DeviceTransferInfoList(QObject* parent) : QObject(parent){}

void DeviceTransferInfoList::appendList(char *data)
{
    if(data == NULL) return;
    int offset = 0;
    quint16 devCount = 0;
    DispersalData dispersalData;

    offset += dispersalData.outlet(devCount, data + offset);
    for(int i = 0; i < devCount; i++){//设备信息
        DeviceTransferInfo* info = new DeviceTransferInfo;
        offset += info->unpack(data + offset);
        info->setTransUserID(srcUserID);
        info->setTransUserName(srcUserName);
        info->setSrcClusterID(srcClusterID);
        info->setSrcClusterName(srcClusterName);
        info->setDestClusterID(destClusterID);
        info->setDestClusterName(destClusterName);
        this->insertList(info);
        this->insertDB(info);
    }
}

QHash<quint64, DeviceTransferInfo*>& DeviceTransferInfoList::fetchList()
{
    return infoList;
}

void DeviceTransferInfoList::loadDataSource(quint64 srcClusterID, quint64 destClusterID)
{
    CppSQLite3Query src;
    DeviceTransferDataSource::queryData(src, srcClusterID, destClusterID);

    while(!src.eof()){
        DeviceTransferDataField dest;
        DeviceTransferInfo* info = new DeviceTransferInfo;
        DeviceTransferDataSource::parseDataField(src, dest);

        info->setTransUserID(dest.srcUserID);
        info->setTransUserName(dest.srcUserName);
        info->setSrcClusterID(dest.srcClusterID);
        info->setSrcClusterName(dest.srcClusterName);
        info->setDestClusterID(dest.destClusterID);
        info->setDestClusterName(dest.srcClusterName);
        info->setDeviceID(dest.deviceID);
        info->setDeviceName(dest.deviceName);
        this->insertList(info);
        src.nextRow();
    }
}

int DeviceTransferInfoList::count()
{
    return infoList.count();
}

void DeviceTransferInfoList::insertList(DeviceTransferInfo *info)
{
    //保证列表数据不重复
    quint64 devID = info->getDeviceID();
    if(infoList.contains(devID))
        this->remove(devID);
    infoList.insert(devID, info);
}

void DeviceTransferInfoList::remove(quint64 removeID)
{
    if(infoList.size() > 0 && infoList.contains(removeID)){
        DeviceTransferInfo* temp = infoList.value(removeID);
        infoList.remove(removeID);
        SAFE_DELETE(temp);
    }
    DeviceTransferDataSource::deleteData(removeID);
}

void DeviceTransferInfoList::deleteLocalTransferDevices(quint64 srcClusterID, quint64 destClusterID)
{
    DeviceTransferDataSource::deleteData(srcClusterID, destClusterID);
}

void DeviceTransferInfoList::insertDB(DeviceTransferInfo *info)
{
    DeviceTransferDataField fields;
    fields.srcUserID = info->getTransUserID();
    fields.srcUserName = info->getTransUserName();
    fields.srcClusterID = info->getSrcClusterID();
    fields.srcClusterName = info->getSrcClusterName();
    fields.destClusterID = info->getDestClusterID();
    fields.destClusterName = info->getDestClusterName();
    fields.deviceID = info->getDeviceID();
    fields.deviceName = info->getDeviceName();
    DeviceTransferDataSource::insertData(fields);
}

DeviceTransferInfoList::~DeviceTransferInfoList()
{
    QHashIterator<quint64, DeviceTransferInfo*> iter(infoList);
    while (iter.hasNext()) {
        iter.next();
        DeviceTransferInfo* info = iter.value();
        SAFE_DELETE(info);
    }
}


