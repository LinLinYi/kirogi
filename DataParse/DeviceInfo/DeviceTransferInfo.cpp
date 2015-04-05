#include "DeviceTransferInfo.h"
#include "DispersalData.h"
#include "DeviceTransferDataSource.h"

DeviceTransferInfo::DeviceTransferInfo(quint64 devID) : m_deviceID(devID)
{

}

quint16 DeviceTransferInfo::unpack(char* data)
{
    if(data == NULL) return 0;
    quint16 offset = 0;
    DispersalData dispersalData;

    offset += dispersalData.outlet(m_deviceID, data + offset);  //设备ID
    offset += dispersalData.outlet(m_deviceName, data + offset);//设备名称
    return offset;
}

void DeviceTransferInfo::loadDataSource(quint64 devID)
{
    CppSQLite3Query src;
    DeviceTransferDataSource::queryData(src, devID);

    while(!src.eof()){
        DeviceTransferDataField dest;
        DeviceTransferDataSource::parseDataField(src, dest);

        m_userID = dest.srcUserID;
        m_userName = dest.srcUserName;
        m_srcClusterID = dest.srcClusterID;
        m_srcClusterName = dest.srcClusterName;
        m_destClusterID = dest.destClusterID;
        m_destClusterName = dest.destClusterName;
        m_deviceID = dest.deviceID;
        m_deviceName = dest.deviceName;
        src.nextRow();
    }
}

DeviceTransferInfo::~DeviceTransferInfo()
{

}




