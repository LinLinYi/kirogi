#include "ServerNotifyInfo.h"
#include "DispersalData.h"

ServerNotifyInfo::ServerNotifyInfo(){}

ServerNotifyInfo::ServerNotifyInfo(char *data, quint16 dataLen)
{
    this->unpack(data, dataLen);
}

void ServerNotifyInfo::unpack(char* data, quint16 dataLen)
{
    if(data == NULL || dataLen == 0) return;
    quint16 offset  = 0;
    DispersalData dispersalData;
    offset += dispersalData.outlet(m_notifyNum, data + offset);
    offset += dispersalData.outlet(m_cmdCode, data + offset);
    m_data.append(data + offset, dataLen - offset);
    m_dataLen = dataLen - offset;
}

quint16 ServerNotifyInfo::getCmdCode()
{
    return m_cmdCode;
}

quint32 ServerNotifyInfo::getNotifyNum()
{
    return m_notifyNum;
}

quint16 ServerNotifyInfo::getDataLen()
{
    return m_dataLen;
}

char* ServerNotifyInfo::getData()
{
    return m_data.data();
}

ServerNotifyInfo::~ServerNotifyInfo()
{

}
