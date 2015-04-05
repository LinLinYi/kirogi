#include "DispersalData.h"
#include "DClusterAuthDevInfo.h"

DClusterAuthDevInfo::DClusterAuthDevInfo(QObject* parent)
   : QObject(parent)
{

}

quint16 DClusterAuthDevInfo::unpack(char *data)
{
    if(data == NULL) return 0;

    int offset = 0;
    quint8 role = 0;
    DispersalData dispersalData;
    offset += dispersalData.outlet(m_deviceID, data + offset);  //设备ID
    offset += dispersalData.outlet(role, data + offset);        //设备权限
    m_userRole = AuthContent::numToAuthlevel(role);
    offset += dispersalData.outlet(m_deviceName, data + offset);//设备名称
    return offset;
}

DClusterAuthDevInfo::~DClusterAuthDevInfo()
{

}



