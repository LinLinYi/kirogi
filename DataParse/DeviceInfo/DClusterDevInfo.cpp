#include "DispersalData.h"
#include "DClusterDevInfo.h"
#include "DClusterDeviceDataSource.h"

DClusterDevInfo::DClusterDevInfo(quint64 clusterID, quint64 devID, QObject* parent)
    : QObject(parent)
    , m_clusterID(clusterID)
    , m_deviceID(devID){}

quint16 DClusterDevInfo::unpack(char *data)
{   
    if(data == NULL) return 0;

    quint16 offset = 0;
    char chMAC[8] = { 0 };
    DispersalData dispersalData;
    offset += dispersalData.outlet(m_deviceID, data + offset);  //设备ID
    offset += dispersalData.outlet(chMAC, data + offset, 8);    //设备MAC
    offset += dispersalData.outlet(m_deviceName, data + offset);//设备名称
    offset += dispersalData.outlet(m_remarks, data + offset);   //设备备注
    offset += dispersalData.outlet(m_describe, data + offset);  //设备描述
//    offset += dispersalData.outlet(m_ipAddr, data + offset);    //设备IP地址
//    offset += dispersalData.outlet(m_port, data + offset);      //设备端口号
    m_deviceMac = macToString(chMAC);

    return offset;
}

QString DClusterDevInfo::macToString(char *data)
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

void DClusterDevInfo::loadDataSource(quint64 clusID, quint64 devID)
{
    CppSQLite3Query src;
    DClusterDeviceDataSource::queryData(src, clusID, devID);

    while(!src.eof()){
        DClusterDevDataField dest;
        DClusterDeviceDataSource::parseDataField(src, dest);

        m_deviceID = dest.deviceID;
        m_deviceName = dest.deviceName;
        m_clusterID = dest.clusterID;
        m_deviceMac = dest.macAddress;
        m_remarks = dest.remarks;
        m_describe = dest.describe;
        src.nextRow();
    }
}

void DClusterDevInfo::updateRemarks(const QString &remarks)
{
    m_remarks = remarks;
    DClusterDeviceDataSource::updateRemarks(m_clusterID, m_deviceID, remarks);
}

void DClusterDevInfo::updateDescribe(const QString &describe)
{
    m_describe = describe;
    DClusterDeviceDataSource::updateDescribe(m_clusterID, m_deviceID, describe);
}

DClusterDevInfo::~DClusterDevInfo()
{

}

