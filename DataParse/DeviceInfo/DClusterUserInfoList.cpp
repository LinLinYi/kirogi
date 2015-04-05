#include "DispersalData.h"
#include "DClusterUserInfoList.h"
#include "DClusterUserDataSource.h"

#define NAME_MAX_LEN 50
DClusterUserInfoList::DClusterUserInfoList(quint64 clusterID)
    : curClusterID(clusterID)
{

}

void DClusterUserInfoList::appendList(char *data, quint16 dataLen,  Mi::SaveInfo type)
{
    if(data == NULL || dataLen == 0)
       return;

    int offset = 0;
    quint64 clusterID = 0;
    quint16 userCount = 0;
    DispersalData dispersalData;
    offset += dispersalData.outlet(clusterID, data + offset);//设备群ID
    offset += dispersalData.outlet(userCount, data + offset);//设备群的用户数目

    for(int i = 0; i < userCount; i++){//群成员信息
        DClusterUserInfo* info = new DClusterUserInfo(clusterID);
        offset += info->unpack(data + offset);
        this->insertList(info);

        if(type ==  Mi::SaveAll)
            this->insertUser(info);
    }
}

void DClusterUserInfoList::append(char *data, quint16 dataLen)
{
    if(data == NULL || dataLen == 0)
        return;

    int offset = 0;
    quint16 clusterID = 0;
    DispersalData dispersalData;
    offset += dispersalData.outlet(clusterID, data + offset);//设备群ID

    //添加用户信息
    DClusterUserInfo* info = new DClusterUserInfo;
    info->unpack(data + offset);
    this->insertList(info);
    this->insertUser(info);
}

QHash<quint64, DClusterUserInfo*>& DClusterUserInfoList::fetchList()
{
    return infoList;
}

void DClusterUserInfoList::append(DClusterUserInfo *info)
{
    if(info == NULL) return;
    this->insertList(info);
    this->insertUser(info);
}

void DClusterUserInfoList::remove(DClusterUserInfo *info)
{
    if(info == NULL) return;
    quint64 userID = info->getUserID();
    if(infoList.size() > 0 && infoList.contains(userID)){
        DClusterUserInfo* temp = infoList.value(userID);
        infoList.remove(userID);
        SAFE_DELETE(temp);
    }
}

void DClusterUserInfoList::remove(quint64 userID)
{
    if(infoList.size() > 0 && infoList.contains(userID)){
        DClusterUserInfo* temp = infoList.value(userID);
        infoList.remove(userID);
        SAFE_DELETE(temp);
    }
}

void DClusterUserInfoList::loadDataSource()
{
    CppSQLite3Query src;
    DClusterUserDataSource::queryData(src, curClusterID);

    while(!src.eof()){
        DClusterUserDataField dest;
        DClusterUserDataSource::parseDataField(src, dest);

        DClusterUserInfo* info = new DClusterUserInfo;
        info->setUserID(dest.userID);
        info->setClusterID(dest.clusterID);

        Mi::AuthLevel role = AuthContent::numToAuthlevel(dest.userRole);
        info->setUserRole(role);
        info->setUserName(dest.userName);
        info->setRemarks(dest.remarks);
        info->setDescribe(dest.describe);
        this->insertList(info);
        src.nextRow();
    }
}

int DClusterUserInfoList::count()
{
    return infoList.count();
}

void DClusterUserInfoList::insertList(DClusterUserInfo *info)
{
    //保证列表数据不重复
    quint64 usrID = info->getUserID();
    if(infoList.contains(usrID))
        this->remove(usrID);
    infoList.insert(usrID, info);
}

void DClusterUserInfoList::insertUser(DClusterUserInfo *info)
{
    DClusterUserDataField fields;
    fields.clusterID = info->getClusterID();
    fields.userID = info->getUserID();
    fields.userName = info->getUserName();
    fields.remarks = info->getRemarks();
    fields.describe = info->getDescribe();
    fields.userRole = info->getUserRole();
    DClusterUserDataSource::insertData(fields);
}

void DClusterUserInfoList::takeAt(quint64 clusID, quint64 userID)
{
    this->remove(userID);
    this->deleteUser(clusID, userID);
}

void DClusterUserInfoList::takeAt(DClusterUserInfo *baseInfo)
{
    quint64 clusID = baseInfo->getClusterID();
    quint64 userID = baseInfo->getUserID();
    this->remove(baseInfo);
    this->deleteUser(clusID, userID);
}

void DClusterUserInfoList::deleteUser(quint64 clusterID, quint64 userID)
{
    DClusterUserDataSource::deleteData(clusterID, userID);
}

void DClusterUserInfoList::deleteUsers(quint64 clusterID)
{
    DClusterUserDataSource::deleteUsers(clusterID);
}

DClusterUserInfoList::~DClusterUserInfoList()
{
    QHashIterator<quint64, DClusterUserInfo*> iter(infoList);
    while (iter.hasNext()) {
        iter.next();
        DClusterUserInfo* info = iter.value();
        SAFE_DELETE(info);
    }
}

