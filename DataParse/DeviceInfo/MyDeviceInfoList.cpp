#include "MyDeviceInfoList.h"
#include "MyDeviceDataSource.h"
#include "DispersalData.h"

MyDeviceInfoList::MyDeviceInfoList(QObject* parent) : QObject(parent){}

void MyDeviceInfoList::appendList(char *data, quint16 dataLen,  Mi::SaveInfo)
{
    if(data == NULL || dataLen == 0)
        return;

    int offset = 0;
    quint16 devCount = 0;
    DispersalData dispersalData;
    offset += dispersalData.outlet(devCount, data + offset);//设备总数

    for(int i = 0; i < devCount; i++){//设备信息
        MyDeviceInfo* info = new MyDeviceInfo;
        offset += info->unpack(data + offset);
        this->insertList(info);
        this->insertDB(info);
    }
}

void MyDeviceInfoList::append(char *data, quint16 dataLen)
{
    if(data == NULL || dataLen == 0)
        return;

    MyDeviceInfo* info = new MyDeviceInfo;
    info->unpack(data);
    this->insertList(info);
    this->insertDB(info);
}

QHash<quint64, MyDeviceInfo*>& MyDeviceInfoList::fetchList()
{
    return infoList;
}

void MyDeviceInfoList::append(MyDeviceInfo *info)
{
    if(info == NULL) return;
    this->insertList(info);
    this->insertDB(info);
}

void MyDeviceInfoList::remove(MyDeviceInfo *info)
{
    if(info == NULL)
        return;

    quint64 devID = info->getDeviceID();
    if(infoList.size() > 0 && infoList.contains(devID)){
        MyDeviceInfo* temp = infoList.value(devID);
        infoList.remove(devID);
        SAFE_DELETE(temp);
    }
    MyDeviceDataSource::deleteData(devID);
}

void MyDeviceInfoList::remove(quint64 removeID)
{
    if(infoList.size() > 0 && infoList.contains(removeID)){
        MyDeviceInfo* temp = infoList.value(removeID);
        infoList.remove(removeID);
        SAFE_DELETE(temp);
    }
    MyDeviceDataSource::deleteData(removeID);
}

void MyDeviceInfoList::deleteDevices(quint64 clusterID)
{
    MyDeviceDataSource::deleteDevices(clusterID);
}

void MyDeviceInfoList::loadDataSource()
{
    CppSQLite3Query src;
    MyDeviceDataSource::queryData(src);

    while(!src.eof()){
        MyDeviceDataField dest;
        MyDeviceInfo* info = new MyDeviceInfo;
        MyDeviceDataSource::parseDataField(src, dest);

        info->setClusterID(dest.clusterID);
        info->setDeviceID(dest.deviceID);
        info->setDeviceName(dest.deviceName);
        info->setDeviceMac(dest.macAddress);
        this->insertList(info);
        src.nextRow();
    }
}

int MyDeviceInfoList::count()
{
    return infoList.count();
}

void MyDeviceInfoList::insertList(MyDeviceInfo *info)
{
    //保证列表数据不重复
    quint64 devID = info->getDeviceID();
    if(infoList.contains(devID))
        this->remove(devID);
    infoList.insert(devID, info);
}

void MyDeviceInfoList::insertDB(MyDeviceInfo *info)
{
    MyDeviceDataField fields;
    fields.deviceID = info->getDeviceID();
    fields.deviceName = info->getDeviceName();
    fields.macAddress = info->getDeviceMac();
    MyDeviceDataSource::insertData(fields);
}

MyDeviceInfoList::~MyDeviceInfoList()
{
    QHashIterator<quint64, MyDeviceInfo*> iter(infoList);
    while (iter.hasNext()) {
        iter.next();
        MyDeviceInfo* info = iter.value();
        SAFE_DELETE(info);
    }
}
