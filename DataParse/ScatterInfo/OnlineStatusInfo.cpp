#include "OnlineStatusInfo.h"
#include "DispersalData.h"

OnlineStatusInfo::OnlineStatusInfo(QObject *parent) : QObject(parent)
{
    m_ID = 0;
}

quint16 OnlineStatusInfo::unpack(char *data)
{
    if(data == NULL) return 0;

    int offset = 0;
    DispersalData dispersalData;
    offset += dispersalData.outlet(m_ID, data + offset);

    quint8 status = 0;
    offset += dispersalData.outlet(status, data + offset);
    m_onLineStatus = NetLine::numToNetstatus(status);
    return offset;
}
