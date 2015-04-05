#include "DispersalData.h"
#include "DClusterDevInfoList.h"
#include "DClusterDeviceDataSource.h"

DClusterDevInfoList::DClusterDevInfoList(quint64 clusterID)
    : curClusterID(clusterID)
{

}

void DClusterDevInfoList::appendList(char *data, quint16 dataLen,  Mi::SaveInfo type)
{
    if(data == NULL || dataLen == 0)  return;

    int offset = 0;
    quint64 clusterID = 0;
    quint16 clusterCount = 0;

    DispersalData dispersalData;
    offset += dispersalData.outlet(clusterID, data + offset);//群ID
    offset += dispersalData.outlet(clusterCount, data + offset);//群成员数量

    for(int i = 0; i < clusterCount; i++){//群成员信息
        DClusterDevInfo* info = new DClusterDevInfo(clusterID);
        offset += info->unpack(data + offset);
        this->insertList(info);

        if(type ==  Mi::SaveAll)
            this->insertDB(info);
    }
}

void DClusterDevInfoList::append(char *data, quint16 dataLen)
{
    if(data == NULL || dataLen == 0)
        return;

    quint16 offset = 0;
    quint64 clusterID = 0;
    DispersalData dispersalData;
    offset += dispersalData.outlet(clusterID, data + offset);//群ID

    DClusterDevInfo* info = new DClusterDevInfo;
    info->unpack(data + offset);
    this->insertList(info);
    this->insertDB(info);
}

QHash<quint64, DClusterDevInfo*>& DClusterDevInfoList::fetchList()
{
    return infoList;
}

void DClusterDevInfoList::append(DClusterDevInfo *info)
{
    if(info == NULL) return;
    this->insertList(info);
    this->insertDB(info);
}

void DClusterDevInfoList::remove(DClusterDevInfo *info)
{
    if(info == NULL)
        return;

    quint64 removeID = info->getDeviceID();
    if(infoList.size() > 0 && infoList.contains(removeID)){
        DClusterDevInfo* temp = infoList.value(removeID);
        infoList.remove(removeID);
        SAFE_DELETE(temp);
    }
    DClusterDeviceDataSource::deleteData(removeID);
}

void DClusterDevInfoList::remove(quint64 removeID)
{
    if(infoList.size() > 0 && infoList.contains(removeID)){
        DClusterDevInfo* temp = infoList.value(removeID);
        infoList.remove(removeID);
        SAFE_DELETE(temp);
    }
    DClusterDeviceDataSource::deleteData(removeID);
}

void DClusterDevInfoList::loadDataSource()
{
    CppSQLite3Query src;
    DClusterDeviceDataSource::queryData(src, curClusterID);

    while(!src.eof()){
        DClusterDevDataField dest;
        DClusterDeviceDataSource::parseDataField(src, dest);

        DClusterDevInfo* info = new DClusterDevInfo;
        info->setDeviceID(dest.deviceID);
        info->setClusterID(dest.clusterID);
        info->setDeviceName(dest.deviceName);
        info->setDeviceMac(dest.macAddress);
        info->setRemarks(dest.remarks);
        info->setDescribe(dest.describe);
        this->insertList(info);
        src.nextRow();
    }
}

int DClusterDevInfoList::count()
{
    return infoList.count();
}

void DClusterDevInfoList::insertList(DClusterDevInfo *info)
{
    //保证列表数据不重复
    quint64 devID = info->getDeviceID();
    if(infoList.contains(devID))
        this->remove(devID);
    infoList.insert(devID, info);
}

void DClusterDevInfoList::insertDB(DClusterDevInfo *info)
{
    DClusterDevDataField fields;
    fields.clusterID = info->getClusterID();
    fields.deviceID = info->getDeviceID();
    fields.deviceName = info->getDeviceName();
    fields.macAddress = info->getDeviceMac();
    fields.remarks = info->getRemarks();
    fields.describe = info->getDescribe();
    DClusterDeviceDataSource::insertData(fields);
}

void DClusterDevInfoList::deleteDevices(quint64 clusterID)
{
    DClusterDeviceDataSource::deleteDevices(clusterID);
}

DClusterDevInfoList::~DClusterDevInfoList()
{
    QHashIterator<quint64, DClusterDevInfo*> iter(infoList);
    while (iter.hasNext()) {
        iter.next();
        DClusterDevInfo* info = iter.value();
        SAFE_DELETE(info);
    }
}
