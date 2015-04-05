#include "MyDeviceInfo.h"
#include "DispersalData.h"
#include "MyDeviceDataSource.h"

MyDeviceInfo::MyDeviceInfo(quint64 devID, QObject* parent)
    : QObject(parent)
    , m_deviceID(devID)
{

}

quint16 MyDeviceInfo::unpack(char* data)
{
    if(data == NULL) return 0;
    quint16 offset = 0;
    char chMAC[8] = { 0 };
    DispersalData dispersalData;

    offset += dispersalData.outlet(m_deviceID, data + offset);  //设备ID
    offset += dispersalData.outlet(m_clusterID, data + offset); //群ID
    offset += dispersalData.outlet(chMAC, data + offset, 8);    //设备MAC地址
    offset += dispersalData.outlet(m_deviceName, data + offset);//设备名称

    m_deviceMac = macToString(chMAC);
    return offset;
}

void MyDeviceInfo::loadDataSource(quint64 devID)
{
    CppSQLite3Query src;
    MyDeviceDataSource::queryData(src, devID);

    while(!src.eof()){
        MyDeviceDataField dest;
        MyDeviceDataSource::parseDataField(src, dest);

        m_deviceID = dest.deviceID;
        m_clusterID = dest.clusterID;
        m_deviceName = dest.deviceName;
        m_deviceMac = dest.macAddress;
        src.nextRow();
    }
}

QString MyDeviceInfo::ipAddrFormat(char* data)
{
    int i;
    QString strIP;
    for(i = 0; i < 4; i++){
        quint8 byteInt = (quint8)data[i];
        Q_ASSERT(byteInt >= 0 && byteInt <= 255);

        strIP += QObject::tr("%1").arg(byteInt);
        if(i != 3)
            strIP += ".";
    }

    return strIP;
}

QString MyDeviceInfo::macToString(char *data)
{

    QString strMac;
    for(int i = 0; i <6; i++){
        quint8 byteInt = (quint8)data[i];
        Q_ASSERT(byteInt >= 0 && byteInt <= 255);

        QString str = QString::number(byteInt&0xff,16);
        strMac += str;

        if(i != 5)
            strMac += "-";
    }
    return strMac;
}

quint64 MyDeviceInfo::macToID(char *data)
{
    char id[8] = { 0 };
    id[0] = 0x01;
    id[1] = 0x00;
    memcpy(id + 2, data, 6);

    return (*(quint64*)id);
}

MyDeviceInfo::~MyDeviceInfo()
{

}


