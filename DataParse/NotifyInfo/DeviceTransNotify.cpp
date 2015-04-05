#include <QTime>
#include "DispersalData.h"
#include "DeviceTransNotify.h"
#include "DeviceTransferInfoList.h"
#include "DClusterDeviceDataSource.h"
#include "DeviceTransNotifyDataSource.h"

DeviceTransNotify::DeviceTransNotify(QObject* parent) : QObject(parent)
{

}

void DeviceTransNotify::unpack(char *data, Mi::DeviceTransfer type)
{
    if(data == NULL) return;
    int offset = 0;
    DispersalData dispersalData;
    if(type == Mi::DeviceTransferApproval){
        offset += dispersalData.outlet(m_msgResult, data + offset);       //通知结果
        if(m_msgResult == Mi::Refused) return;//拒绝请求

        offset += dispersalData.outlet(m_srcUserID, data + offset);       //移交人ID
        offset += dispersalData.outlet(m_srcUserName, data + offset);     //移交人名称
        offset += dispersalData.outlet(m_srcClusterID, data + offset);    //移交源群ID
        offset += dispersalData.outlet(m_srcClusterName, data + offset);  //移交源群名称
        offset += dispersalData.outlet(m_destClusterID, data + offset);   //移交目标群ID
        offset += dispersalData.outlet(m_destClusterName, data + offset); //移交目标名称

        quint16 devCount = 0;
        offset += dispersalData.outlet(devCount, data + offset);
        for(int i = 0; i < devCount; i++){
            quint64 devID = 0;
            offset += dispersalData.outlet(devID, data + offset);
            DClusterDeviceDataSource::deleteData(devID);
        }

    }else if(type == Mi::DeviceTransferReq){
        DeviceTransferInfoList infoList;
        offset += dispersalData.outlet(m_srcUserID, data + offset);       //移交人ID
        offset += dispersalData.outlet(m_srcUserName, data + offset);     //移交人名称
        offset += dispersalData.outlet(m_srcClusterID, data + offset);    //源群ID
        offset += dispersalData.outlet(m_srcClusterName, data + offset);  //源群名称
        offset += dispersalData.outlet(m_destClusterID, data + offset);   //目标群ID
        offset += dispersalData.outlet(m_destClusterName, data + offset); //目标群名称

        infoList.setSrcUserID(m_srcUserID);
        infoList.setSrcUserName(m_srcUserName);
        infoList.setSrcClusterID(m_srcClusterID);
        infoList.setScrClusterName(m_srcClusterName);
        infoList.setDestClusterID(m_destClusterID);
        infoList.setDestClusterName(m_destClusterName);
        infoList.appendList(data + offset);
    }else
        return;

    m_msgStatus = Mi::Pending;
    m_msgTime = this->getCurrentTime();
}

void DeviceTransNotify::saveToLocal()
{
    DeviceTransNotifyDataField fields;
    fields.srcUserID = m_srcUserID;
    fields.srcUserName = m_srcUserName;
    fields.srcClusterID = m_srcClusterID;
    fields.srcClusterName = m_srcClusterName;
    fields.destClusterID = m_destClusterID;
    fields.destClusterName = m_destClusterName;
    fields.status = m_msgStatus;
    fields.notifyTime = m_msgTime;
    fields.notifyContent = m_msgContent;
    DeviceTransNotifyDataSource::insertData(fields);
}

void DeviceTransNotify::updateInfoStatus(Mi::MsgStatus status, quint64 userID, quint64 srcID, quint64 destID)
{
    m_msgStatus = status;
    DeviceTransNotifyDataField fields;
    if(srcID == 0 || destID == 0 || userID == 0){
        fields.srcUserID = m_srcUserID;
        fields.srcClusterID = m_srcClusterID;
        fields.destClusterID = m_destClusterID;
        fields.status = m_msgStatus;
    }else{
        fields.srcUserID = userID;
        fields.srcClusterID = srcID;
        fields.destClusterID = destID;
        fields.status = status;
    }

    DeviceTransNotifyDataSource::updateCheckStatus(fields);
}

QString DeviceTransNotify::getCurrentTime()
{
    QDateTime curDateTime = QDateTime::currentDateTime();
    QString curTime = curDateTime.toString("yyyy-MM-dd hh:mm");
    return curTime;
}

DeviceTransNotify::~DeviceTransNotify()
{

}
