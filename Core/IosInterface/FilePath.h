#ifndef FETCHFILEPATH_H
#define FETCHFILEPATH_H

#include <iostream>

/*! \class FilePath
 *  \brief ios数据库路径的类
 */

// 用户登录信息保存文件名称(IOS\ANDROID\OTHER)
#define USER_LOGIN_INFO_FILE_NAME_IOS @("loginInfo.dat")
#define USER_LOGIN_INFO_FILE_NAME_ANDROID "loginInfo.dat"
#define USER_LOGIN_INFO_FILE_NAME_OTHER "loginInfo.dat"

// 系统日志信息保存文件名称
#define SYSTEM_LOG_INFO_FILE_NAME_IOS @("sysLogInfo.text")
#define SYSTEM_LOG_INFO_FILE_NAME_ANDROID "sysLogInfo.text"
#define SYSTEM_LOG_INFO_FILE_NAME_OTHER "sysLogInfo.text"

class FilePath
{
public:
    FilePath();
    ~FilePath();
 
public:
    static const char* databaseFilePath();
    static const char* userLoginInfoPath();
    static const char* systemLogInfoPath();
};

#endif /* defined(SETFILEPATH_H) */
