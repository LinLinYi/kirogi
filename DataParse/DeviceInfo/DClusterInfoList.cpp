#include "DClusterInfoList.h"
#include "DClusterDataSource.h"
#include "DispersalData.h"

DClusterInfoList::DClusterInfoList(QObject* parent) : QObject(parent){}

void DClusterInfoList::appendList(char *data, quint16 dataLen,  Mi::SaveInfo type)
{
    if(data == NULL || dataLen == 0) return;

    int offset = 0;
    quint16 clusterCount = 0;
    DispersalData dispersalData;

    offset += dispersalData.outlet(clusterCount, data + offset);//群数量
    for(int i = 0; i < clusterCount; i++){//群信息
        DClusterInfo* info = new DClusterInfo;
        offset += info->unpack(data + offset);
        this->insertList(info);

        if(type ==  Mi::SaveAll)
            this->insertDB(info);
    }
}

void DClusterInfoList::append(char *data, quint16 dataLen)
{
    if(data == NULL || dataLen == 0)
        return;

    DClusterInfo* info = new DClusterInfo;
    info->unpack(data);
    this->insertList(info);
    this->insertDB(info);
}

QHash<quint64, DClusterInfo*>& DClusterInfoList::fetchList()
{
    return infoList;
}

void DClusterInfoList::append(DClusterInfo *info)
{
    if(info == NULL) return;
    this->insertList(info);
    this->insertDB(info);
}

void DClusterInfoList::remove(DClusterInfo *info)
{
    if(info == NULL) return;
    quint64 removeID = info->getClusterID();

    if(infoList.size() > 0 && infoList.contains(removeID)){
        DClusterInfo* temp = infoList.value(removeID);
        infoList.remove(removeID);
        SAFE_DELETE(temp);
    }
    DClusterDataSource::deleteData(removeID);
}

void DClusterInfoList::remove(quint64 removeID)
{
    if(infoList.size() > 0 && infoList.contains(removeID)){
        DClusterInfo* temp = infoList.value(removeID);
        infoList.remove(removeID);
        SAFE_DELETE(temp);
    }
    DClusterDataSource::deleteData(removeID);
}

void DClusterInfoList::loadDataSource()
{
    CppSQLite3Query src;
    DClusterDataSource::queryData(src);

    while(!src.eof()){
        DClusterDataField dest;
        DClusterDataSource::parseDataField(src, dest);

        Mi::AuthLevel authRole = AuthContent::numToAuthlevel(dest.userRole);
        DClusterInfo* info = new DClusterInfo;
        info->setClusterID(dest.clusterID);
        info->setClusterName( dest.clusterName);
        info->setRemarks(dest.remarks);
        info->setDescribe(dest.describe);
        info->setUserRole(authRole);
        this->insertList(info);
        src.nextRow();
    } 
}

int DClusterInfoList::count()
{
   return infoList.count();
}

void DClusterInfoList::insertList(DClusterInfo *info)
{
    //保证列表数据不重复
    quint64 clusID = info->getClusterID();
    if(infoList.contains(clusID))
        this->remove(clusID);
    infoList.insert(clusID, info);
}

void DClusterInfoList::insertDB(DClusterInfo *info)
{
    DClusterDataField fields;
    fields.clusterID = info->getClusterID();
    fields.clusterName = info->getClusterName();
    fields.remarks = info->getRemarks();
    fields.describe = info->getDescribe();
    fields.userRole = info->getUserRole();
    DClusterDataSource::insertData(fields);
}

DClusterInfoList::~DClusterInfoList()
{
    QHashIterator<quint64, DClusterInfo*> iter(infoList);
    while (iter.hasNext()) {
        iter.next();
        DClusterInfo* info = iter.value();
        SAFE_DELETE(info);
    }
}
