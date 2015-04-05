#ifndef SQLITECONFIG_HPP
#define SQLITECONFIG_HPP

#include <stdlib.h>
#include <qglobal.h>
#include <QString>

/***********************************************各系统下数据库名称*******************************************/
#define DATABASE_NAME_IOS @("HistoryInfo.sqlite")                            /*! IOS:sqlite数据库名称*/
#define DATABASE_NAME_ANDROID "HistoryInfo.sqlite"                           /*! ADROID:sqlite数据库名称*/
#define DATABASE_NAME_OTHER "HistoryInfo.sqlite"                             /*! OTHER:sqlite数据库名称*/

/***********************************************设备相关数据库表*********************************************/
const quint64 DEFAULT_NOTIFYID                  = 0;                         /*! 默认通知信息标识ID*/
const QString DT_SUMMARY_NOTIFY                 = "DT_SummaryNotify";        /*! 表名称：设备所有通知概述*/
const QString DT_CLUSTER_NOTIFY                 = "DT_ClusterNotify";        /*! 表名称：设备群消息通知*/
const QString DT_CLUSTER_INFO                   = "DT_ClusterInfo";          /*! 表名称：设备群信息*/
const QString DT_MYDEVICE_INFO                  = "DT_MyDeviceInfo";         /*! 表名称：我的设备信息*/
const QString DT_CLUSTER_USER_INFO              = "DT_ClusterUserInfo";      /*! 表名称：设备群用户信息*/
const QString DT_CLUSTER_DEVICE_INFO            = "DT_ClusterDevInfo";       /*! 表名称：设备群设备信息*/
const QString DT_TRANSFER_DEVICE_INFO           = "DT_TransferDevInfo";      /*! 表名称：移交设备信息*/
const QString DT_TRANSFER_DEVICE_NOTIFY         = "DT_TransferDevNotify";    /*! 表名称：设备移交通知信息*/

/***********************************************联系人相关数据库表*********************************************/
const QString UT_USER_INFO                      = "UT_UserInfo";             /*! 表名称：用户信息表*/
const QString UT_CLUSTER_INFO                   = "UT_ClusterInfo";          /*! 表名称：用户群信息表*/
const QString UT_SUMMARY_NOTIFY                 = "UT_SummaryNotify";        /*! 表名称：用户所有通知概述*/
const QString UT_CLUSTER_NOTIFY                 = "UT_ClusterNotify";        /*! 表名称：用户群消息通知*/
const QString UT_USER_JOIN_NOTIFY               = "UT_UserJoinNotify";       /*! 表名称：用户加入信息通知*/

#endif // SQLITECMDCONFIG_HPP
