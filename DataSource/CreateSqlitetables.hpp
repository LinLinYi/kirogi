#ifndef CREATESQLITETABLES_HPP
#define CREATESQLITETABLES_HPP

#include "DClusterNotifyDataSource.h"
#include "DClusterDataSource.h"
#include "DClusterDeviceDataSource.h"
#include "MyDeviceDataSource.h"
#include "DeviceTransferDataSource.h"
#include "DClusterUserDataSource.h"
#include "DNotifySummaryDataSource.h"
#include "DClusterNotifyDataSource.h"
#include "DeviceTransNotifyDataSource.h"

#include "UserDataSource.h"
#include "UClusterDataSource.h"
#include "UserJoinNotifyDataSource.h"
#include "UClusterNotifyDataSource.h"
#include "UNotifySummaryDataSource.h"


class CreateSqliteTables
{
public:
    static void CreateTables(quint64 accountID){
        //创建与设备相关的数据库表
        DNotifySummaryDataSource::createTable(accountID);    //创建设备页面通知列表
        DClusterDataSource::createTable(accountID);          //创建设备群信息表
        MyDeviceDataSource::createTable(accountID);          //创建我的设备表
        DClusterDeviceDataSource::createTable(accountID);    //创建设备群设备列表
        DeviceTransferDataSource::createTable(accountID);    //创建移交设备列表
        DClusterUserDataSource::createTable(accountID);      //创建设备群用户列表
        DClusterNotifyDataSource::createTable(accountID);    //创建设备群通知列表
        DeviceTransNotifyDataSource::createTable(accountID); //创建设备移交通知列表

        //创建与用户相关的数据库表
        UserDataSource::createTable(accountID);              //创建用户信息表
        UClusterDataSource::createTable(accountID);          //创建用户群列表
        UClusterNotifyDataSource::createTable(accountID);    //创建用户群通知信息表
        UNotifySummaryDataSource::createTable(accountID);    //创建联系人通知列表
        UserJoinNotifyDataSource::createTable(accountID);    //创建用户加入通知列表
    }
};

#endif // CREATESQLITETABLES_HPP

