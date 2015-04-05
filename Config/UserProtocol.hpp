#ifndef USERPROTOCOL_HPP
#define USERPROTOCOL_HPP

#include <QString>

namespace DestServer
{
    const QString SERVER_IP                       =        "192.168.1.1";//服务器ip地址
    const quint16 SERVER_PORT                     =         8000;//服务器端口号
}

namespace CommandCode
{
    //客户端自定义功能码(2byte)
    const quint16 INVALID                          =         0x0000;     //默认
    const quint16 NET_ERROR                        =         0x0001;     //网络错误
    const quint16 VERIFY_PWD_ERROR                 =         0x0002;     //密码验证错误

    const quint16 BOTTOM_BAR_DISPLAY               =         0x0003;     //页面显示(页面切换时用)
    const quint16 BOTTOM_BAR_UNDISPLAY             =         0x0004;     //页面不显示(页面切换时用)

    const quint16 BACK_LOGIN_PAGE                  =         0x0005;     //返回登录页面
    const quint16 BACK_DEVICE_PAGE                 =         0x0006;     //返回设备页面
    const quint16 BACK_CONTACT_PAGE                =         0x0007;     //返回联系人页面
    const quint16 BACK_ACCOUNT_SAFE_PAGE           =         0x0008;     //返回账户安全页面
    const quint16 BACK_CLUSTER_MANAGE_PAGE         =         0x0009;     //返回群管理页面

    const quint16 SEARCH_LOCAL_USERLIST            =         0x0010;     //搜索本地用户列表
    const quint16 SEARCH_LOCAL_DEVICELIST          =         0x0011;     //搜索本地设备列表
    const quint16 SEARCH_LOCAL_PROJECTLIST         =         0x0012;     //搜索本地项目列表
    const quint16 SEARCH_USER_LOCAL_CLUSTERLIST    =         0x0013;     //搜索本地用户群列表
    const quint16 SEARCH_DEV_LOCAL_CLUSTERLIST     =         0x0014;     //搜素本地设备群列表

    const quint16 LOCAL_DEVICE                     =         0x0015;     //客户端局域网设备功能码
     const quint16 LOCAL_DEVICE_AIRING             =         0x0016;     //客户端局域网设备广播功能码

    //网络传输功能码范围
    const quint16 MIN_VALUE                        =         0x1000;     //最小命令值
    const quint16 MAX_VALUE                        =         0xFFFF;     //最大命令值

    //客户端用户与服务器功能码
    const quint16 REGISTER_FIRST_STEP              =         0x1200;     //用户注册第一步(申请)
    const quint16 REGISTER_SECOND_STEP             =         0x1201;     //用户注册第二步(邮箱验证)
    const quint16 REGISTER_THIRD_STEP              =         0x1202;     //用户注册第三步(填入用户名、密码)
    const quint16 LOGIN_FIRST_STEP                 =         0x1203;     //登录第一步(获取登录因子)
    const quint16 LOGIN_SECOND_STEP                =         0x1204;     //登录第二步(获取会话秘钥)
    const quint16 ACCOUNT_LOGOUT                   =         0x1205;     //退出登录账号
    const quint16 FORGET_PASSWORD_APPLY            =         0x1210;     //忘记密码:申请
    const quint16 FORGET_PASSWORD_VERIFY           =         0x1211;     //忘记密码:服务器邮箱验证
    const quint16 FORGET_PASSWORD_RESET            =         0x1212;     //忘记密码:设置新密码

    const quint16 MODIFY_ACCOUNT_PWD               =         0x1213;     //修改账户密码
    const quint16 MODIFY_MAIL_ADDR                 =         0x1214;     //修改邮箱地址
    const quint16 MODIFY_MAIL_VERIFYCODE           =         0x1215;     //修改邮箱验证
    const quint16 MODIFY_ACCOUNT_NICKNAME          =         0x1216;     //修改账户昵称
    const quint16 MODIFY_ACCOUNT_SIGNATURE         =         0x1217;     //修改账户签名
    const quint16 FETCH_PerSONAL_INFO              =         0x1230;     //获取个人信息
    const quint16 SOFTWARE_INFO_FEEDBACK           =         0x1E00;     //软件信息反馈

    const quint16 USER_JOIN_REQ                    =         0x1400;     //加入用户请求
    const quint16 USER_JOIN_APPROVAL               =         0x1401;     //加入用户审核
    const quint16 FETCH_USER_INFO_LIST             =         0x1410;     //获取用户信息列表

    const quint16 UCLUSTER_CREATE                  =         0x1600;     //创建用户群
    const quint16 SEARCH_UCLUSTER                  =         0x1601;     //查找用户群
    const quint16 UCLUSTER_APPLY_REQ               =         0x1602;     //用户群加入请求
    const quint16 UCLUSTER_APPLY_APPROVAL          =         0x1603;     //用户群加入审核
    const quint16 FETCH_USER_CLUSTER_LIST          =         0x1610;     //获取用户群列表
    const quint16 FETCH_USER_CLUSTER_INFO          =         0x1611;     //获取用户群信息

    const quint16 DCLUSTER_DEVICE_ALTER_REMARKS    =         0x1800;     //修改设备备注
    const quint16 DCLUSTER_DEVICE_ALTER_DESCRIBE   =         0x1801;     //修改设备描述
    const quint16 FETCH_MY_DEVICE_LIST             =         0x1810;     //获取我的设备信息列表
    const quint16 FETCH_MY_DEVICE_NETSTATUS_LIST   =         0x1811;     //获取我的设备在线列表

    const quint16 MY_DEVICE_NETSTAUS_NOTIFY        =         0x2000;     //我的设备网络状态通知
    const quint16 SEARCH_USER                      =         0x2200;     //查找用户
    const quint16 DCLUSTER_SEARCH_USER             =         0x2201;     //设备群搜索用户
    const quint16 FETCH_USER_INFO                  =         0x2203;     //获取用户信息
    const quint16 FETCH_DEVICE_INFO                =         0x2204;     //获取我的设备信息

    //客户端设备与服务器功能码
    const quint16 DCLUSTER_CREATE                  =         0x1A00;     //创建设备群
    const quint16 DCLUSTER_DELETE                  =         0x1A01;     //删除设备群请求
    const quint16 DCLUSTER_DELETE_VERIFY           =         0x1A02;     //验证：删除群
    const quint16 SEARCH_DCLUSTER                  =         0x1A03;     //查找设备群
    const quint16 DCLUSTER_APPLY_REQ               =         0x1A04;     //加入设备群请求
    const quint16 DCLUSTER_APPLY_APPROVAL          =         0x1A05;     //审核加入设备群审核
    const quint16 DCLUSTER_EXIT                    =         0x1A06;     //退出设备群
    const quint16 DCLUSTER_INVITE_REQ              =         0x1A07;     //设备群邀请用户请求
    const quint16 DCLUSTER_INVITE_APPROVAL         =         0x1A08;     //设备群邀请用户审核
    const quint16 DCLUSTER_DELETE_USER             =         0x1A09;     //设备群删除用户
    const quint16 FETCH_DCLUSTER_INFO              =         0x1A10;     //获取设备群信息
    const quint16 FETCH_DCLUSTER_LIST              =         0x1A11;     //获取设备群列表
    const quint16 FETCH_DCLUSTER_DEVICE_LIST       =         0x1A12;     //获取设备群设备列表
    const quint16 FETCH_DCLUSTER_DEV_ONLINE_LIST   =         0x1A13;     //获取设备群设备在线列表
    const quint16 FETCH_DCLUSTER_USER_LIST         =         0x1A15;     //获取设备群用户列表
    const quint16 FETCH_DEV_AUTHORIZED_USER_LIST   =         0x1A14;     //获取设备群设备已授权用户列表
    const quint16 FETCH_USER_AUTHORIZED_DEV_LIST   =         0x1A16;     //获取设备群用户已授权设备列表
    const quint16 DCLUSTER_CLAIM_DEVICE_LIST       =         0x1A17;     //获取认领设备列表

    const quint16 DCLUSTER_DEV_TRANSFER_LIST       =         0x1A20;     //获取移交设备列表
    const quint16 DCLUSTER_DEV_TRANSFER_REQ        =         0x1A21;     //设备群设备移交
    const quint16 DCLUSTER_DEV_TRANSFER_APPROVAL   =         0x1A22;     //移交设备审核(同意、拒绝)

    const quint16 DCLUSTER_SETTING_USER_AUTHORIZE  =         0x1A30;     //设置群管理员
    const quint16 DCLUSTER_ALTER_REMARKS           =         0x1A31;     //修改设备群备注
    const quint64 DCLUSTER_ALTER_DESCRIBE          =         0x1A32;     //修改设备群描述
    const quint16 DCLUSTER_USER_ADD_DEVICE         =         0x1A33;     //设备群用户添加设备
    const quint16 DCLUSTER_USER_DELETE_DEVICE      =         0x1A34;     //设备群用户删除设备
    const quint16 DCLUSTER_DEVICE_ADD_USER         =         0x1A35;     //设备群设备添加用户
    const quint16 DCLUSTER_DEVICE_DELETE_USER      =         0x1A36;     //设备群设备删除用户
    const quint16 DCLUSTER_CLAIM_DEVICE            =         0x1A40;     //设备认领

    //服务器通知与客户端心跳包功能码
    const quint16 SERVER_NOTIFY                    =         0xFFFE;     //回复服务端
    const quint16 HEART_BEAT                       =         0xFFFF;     //客户端心跳包

    //待完成功能码
    const quint16 UCLUSTER_INVITE_REQ              =         0x3000;     //用户群邀请加入请求
    const quint16 UCLUSTER_INVITE_APPROVAL         =         0x3010;     //用户群邀请加入审核
    const quint16 FETCH_DCLUSTER_DEVICE_INFO       =         0x3020;     //获取设备群单个设备信息
}

namespace ConstNum
{
    const quint16 MAX_PACK_RETRAN_TIMES             =         0003;       //数据包重发次数
    const quint16 MAX_PACK_RETRAN_TIME_INTERVAL     =         5000;       //重发时间间隔(ms)
    const quint16 MAX_NAME_LEN                      =         0050;       //名称最大字符数
    const quint16 MAX_DESCRIBE_LEN                  =         0255;       //最大描述字符数
    const quint16 MAX_PACKET_SIZE                   =         1024;       //数据包最大长度
    const quint16 MAX_TIME_INTERVAL                 =         0010;       //页面发送数据到网络时间
    const quint16 MAX_HEARTBEAT_TIME_INTERVAL       =         30000;      //最大心跳时间(ms)

    const quint8 PACKET_START_SYSMBOL               =         0x00F0;     //前缀(发送数据包开始标志)
    const quint8 PACKET_END_SYSMBOL                 =         0x00F1;     //后缀(发送数据包结束标志)

    const quint64 DEVICE_ID_BASIC_PREFIX            =         0x0200000000000000;//设备ID前缀
    const quint64 DEVICE_ID_BASIC_POSTFIX           =         0x0000FFFFFFFFFFFF;//设备ID后缀
}

#endif // USERPROTOCOL_HPP
