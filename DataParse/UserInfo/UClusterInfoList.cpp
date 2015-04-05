#include "UClusterInfoList.h"
#include "UClusterDataSource.h"
#include "DispersalData.h"

UClusterInfoList::UClusterInfoList(QObject* parent) : QObject(parent){}

void UClusterInfoList::appendList(char *data, quint16 dataLen,  Mi::SaveInfo type)
{
    if(data == NULL || dataLen == 0) return;

    int offset = 0;
    quint16 clusterCount = 0;
    DispersalData dispersalData;

    offset += dispersalData.outlet(clusterCount, data + offset);//群数量
    for(int i = 0; i < clusterCount; i++){//群信息
        UClusterInfo* info = new UClusterInfo;
        offset += info->unpack(data + offset);
        this->insertList(info);

        if(type ==  Mi::SaveAll){
            this->insertDB(info);
        }
    }
}

void UClusterInfoList::append(char *data, quint16 dataLen)
{
    if(data == NULL || dataLen == 0) return;
    UClusterInfo* info = new UClusterInfo;
    info->unpack(data);
    this->insertList(info);
    this->insertDB(info);
}

QHash<quint64, UClusterInfo*>& UClusterInfoList::fetchList()
{
    return infoList;
}

void UClusterInfoList::append(UClusterInfo *info)
{
    if(info == NULL) return;
    this->insertList(info);
    this->insertDB(info);
}

void UClusterInfoList::remove(UClusterInfo *info)
{
    if(info == NULL) return;
    quint64 removeID = info->getClusterID();

    if(infoList.size() > 0 && infoList.contains(removeID)){
        UClusterInfo* temp = infoList.value(removeID);
        infoList.remove(removeID);
        SAFE_DELETE(temp);
    }
    UClusterDataSource::deleteData(removeID);
}

void UClusterInfoList::remove(quint64 removeID)
{
    if(infoList.size() > 0 && infoList.contains(removeID)){
        UClusterInfo* temp = infoList.value(removeID);
        infoList.remove(removeID);
        SAFE_DELETE(temp);
    }
    UClusterDataSource::deleteData(removeID);
}

void UClusterInfoList::loadDataSource()
{
    CppSQLite3Query src;
    UClusterDataSource::queryData(src);

    while(!src.eof()){
        UClusterDataField dest;
        UClusterDataSource::parseDataField(src, dest);

        Mi::AuthLevel authRole = AuthContent::numToAuthlevel(dest.userRole);
        UClusterInfo* info = new UClusterInfo;
        info->setClusterID(dest.clusterID);
        info->setClusterName( dest.clusterName);
        info->setDescribe(dest.describe);
        info->setUserRole(authRole);
        this->insertList(info);
        src.nextRow();
    }
}

int UClusterInfoList::count()
{
   return infoList.count();
}

void UClusterInfoList::insertList(UClusterInfo *info)
{
    //保证列表数据不重复
    quint64 clusID = info->getClusterID();
    if(infoList.contains(clusID))
        this->remove(clusID);
    infoList.insert(clusID, info);
}

void UClusterInfoList::insertDB(UClusterInfo *info)
{
    UClusterDataField fields;
    fields.clusterID = info->getClusterID();
    fields.clusterName = info->getClusterName();
    fields.describe = info->getDescribe();
    fields.userRole = info->getUserRole();
    UClusterDataSource::insertData(fields);
}

UClusterInfoList::~UClusterInfoList()
{
    QHashIterator<quint64, UClusterInfo*> iter(infoList);
    while (iter.hasNext()) {
        iter.next();
        UClusterInfo* info = iter.value();
        SAFE_DELETE(info);
    }
}
