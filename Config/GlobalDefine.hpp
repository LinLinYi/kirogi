#ifndef GLOBALDEFINE_HPP
#define GLOBALDEFINE_HPP

#include <QObject>
#include <QString>
#include <QtGlobal>
#include <QDebug>
#include "ScreenPara.h"

class Mi
{
public:
    enum Ver{//系统版本号
        Version = 0x0103
    };

    enum NotifyID{//默认通知信息ID
        ClusterJoinNotifyID = 0x01,
        UserJoinNotifyID = 0x02,
        DeviceClaimNotifyID = 0x03,
        DeviceTransferNotifyID = 0x04
    };

    enum NetStatus{//网络状态
        NetNone  = 0x00,                  /*! 无效*/
        OnLine   = 0x01,                  /*! 在线*/
        OffLine  = 0x02                   /*! 离线*/
    };

    enum AuthLevel{//权限级别
        AuthNone       = 0x00,            /*! 无效*/
        SySManager     = 0x01,            /*! 系统管理员*/
        SeniorManager  = 0x02,            /*! 高级管理员*/
        Operator       = 0x03             /*! 操作员*/
    };

    enum MsgStatus{//消息状态
        Pending = 0x00,                  /*! 待处理*/
        Success = 0x01,                  /*! 成功*/
        Agreed = 0x02,                   /*! 已同意*/
        Refused = 0x03,                  /*! 已拒绝*/
        Absent = 0x04,                   /*! 不存在*/
        Used = 0x05,                     /*! 已使用*/
        Error = 0x06,                    /*! 错误*/
        UnRead = 0x07,                   /*! 未读*/
        UnPermisson = 0x08,              /*! 无权限*/
        Failure = 0x09,                  /*! 失败*/
    };

    enum ClusterJoin{//群加入通知类型
        JoinNone = 0x00,                  /*! 无效*/
        ClusterInviteReq = 0x01,          /*! 邀请加入群*/
        ClusterApplyReq = 0x02,           /*! 申请加入群*/
        ClusterInviteApproval = 0x03,     /*! 邀请加入群审核*/
        ClusterApplyApproval = 0x04       /*! 申请加入群审核*/
    };

    enum DeviceTransfer{//设备移交通知类型
        TransNone = 0x00,                 /*! 无效*/
        DeviceTransferReq = 0x01,         /*! 设备移交请求*/
        DeviceTransferApproval = 0x02     /*! 设备移交审核*/
    };
    
    enum UserJoin{//用户加入通知类型
        UserJoinNone = 0x00,              /*! 无效*/
        UserJoinReq = 0x01,               /*! 用户加入请求*/
        UserJoinApproval = 0x02           /*! 用户加入审核*/
    };

    enum NotifyCategory{//通知分类
        NotifyNone = 0x00,               /*! 无效*/
        ClusterJoinNotify  = 0x01,       /*! 群加入通知*/
        NotifySummary  = 0x02,           /*! 通知概览*/

        UserJoinNotify  = 0x03,          /*! 用户申请加入通知*/
        UserChatNotify  = 0x04,          /*! 用户聊天通知*/
        UserClusterChatNotify = 0x05,    /*! 群聊天通知*/
        DeviceTransferNotify = 0x06,     /*! 设备移交通知*/
        DeviceClaimNotify = 0x07,        /*! 设备认领通知*/
        DeviceClusterManageNotify = 0x08,/*! 设备群群管理通知*/
    };

    enum ClusterCategory{//群分类
        ClusterNone   = 0x00,            /*! 无效*/
        UserCluster   = 0x01,            /*! 用户群*/
        DeviceCluster = 0x02             /*! 设备群*/
    };

    enum SaveInfo{//数据保存类型
      SaveNone = 0x00,                   /*! 无效*/
      SaveList = 0x01,                   /*! 保存到App内存列表*/
      SaveDB   = 0x02,                   /*! 保存到本地数据库*/
      SaveAll  = 0x03                    /*! 保存所有*/
    };
};

class Screen
{
public:
    enum ParaPhone{
        PhoneStatusBarHeight = 20,
        PhoneNavigationBarHeight = 44,
        PhoneToolBarHeight = 49,
        PhoneMinPagingPixel = 100
    };

    enum ParaPad{
        PadStatusBarHeight = 20,
        PadNavigationBarHeight = 44,
        PadToolBarHeight = 56,
        PadMinPagingPixel = 120
    };
};

class AuthContent : public QObject
{
public:
    static QString GetRole(Mi::AuthLevel role){
        QString manager;
        switch(role)
        {
            case Mi::SySManager:
                manager = QString(tr("创建者"));
                break;
            case Mi::SeniorManager:
                manager = QString(tr("管理员"));
                break;
            case Mi::Operator:
                manager = QString(tr("操作员"));
                break;
            default:
                break;
        }
        return manager;
    }

    static Mi::AuthLevel numToAuthlevel(int level){
        if(level == 0x01){
            return Mi::SySManager;
        }else if(level == 0x02){
            return Mi::SeniorManager;
        }else if(level == 0x03){
            return Mi::Operator;
        }else
            return Mi::AuthNone;
    }
};

class NetLine : public QObject
{
public:
    static QString GetNetStatus(Mi::NetStatus status){
        QString line;
        switch(status)
        {
            case Mi::OnLine:
                line = QString(tr("[在线]"));
                break;
            case Mi::OffLine:
                line = QString(tr("[离线]"));
                break;
            default:
                line = QString("");
                break;
        }
        return line;
    }

    static Mi::NetStatus numToNetstatus(int status){
        if(status == 0x01){
            return Mi::OnLine;
        }else if(status == 0x02){
            return Mi::OffLine;
        }else
            return Mi::NetNone;
    }
};

class SoftwareVer : public QObject
{
public:
    static QString versionContent(){
        quint16 versionNum = Mi::Version;
        quint16 subVerNum = versionNum % 16;
        quint16 masterVerNum = versionNum;

        while(masterVerNum > 15){
            masterVerNum = masterVerNum / 16;
        }

        QString strSubVerNum;
        if(subVerNum < 10 && subVerNum < 16){
            strSubVerNum = "0" + QString::number(subVerNum);
        }else{
            strSubVerNum = QString::number(subVerNum);
        }

        QString strSoftwareVersionIllustrate;
        strSoftwareVersionIllustrate = QString(tr("麦控 V")) + QString::number(masterVerNum) + "." + strSubVerNum;
        return strSoftwareVersionIllustrate;
    }
};

namespace MiDebug {
    #ifdef QT_DEBUG
    #  define DPRINT(format, ...) \
        qDebug(format, ## __VA_ARGS__)
    #  define DWHERE() \
        qDebug("###FILE: %s ###LINE: %d  ###FUNC: %s", __FILE__, __LINE__, __FUNCTION__)
    #else
    #  define DPRINT(format, ...)
    #  define DWHERE()
    #endif

    #define SAFE_DELETE(p) {if(p) {delete (p);(p)=NULL;}}
    #define SAFE_DELETE_ARRAY(p) {if(p) {delete[] (p);(p)=NULL;}}
}

#endif // GLOBALDEFINE_HPP
