#include <QtGlobal>

#include "FilePath.h"
#include "SqliteConfig.hpp"

#ifdef Q_OS_IOS
    #import <Foundation/Foundation.h>
#endif

FilePath::FilePath(){}

const char* FilePath::databaseFilePath()
{
#ifdef Q_OS_IOS
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documents = [paths objectAtIndex:0];
    NSString *database_path = [documents stringByAppendingPathComponent:DATABASE_NAME_IOS];
    return ([database_path UTF8String]);
#elif Q_OS_ANDROID
    return DATABASE_NAME_ANDROID;
#else
    return DATABASE_NAME_OTHER;
#endif
}

const char* FilePath::userLoginInfoPath()
{
#ifdef Q_OS_IOS
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documents = [paths objectAtIndex:0];
    NSString *userInfo_path = [documents stringByAppendingPathComponent:USER_LOGIN_INFO_FILE_NAME_IOS];
    return ([userInfo_path UTF8String]);
#elif Q_OS_ANDROID
    return USER_LOGIN_INFO_FILE_NAME_ANDROID;
#else
    return USER_LOGIN_INFO_FILE_NAME_OTHER;
#endif
}

const char* FilePath::systemLogInfoPath()
{
#ifdef Q_OS_IOS
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documents = [paths objectAtIndex:0];
    NSString *logInfo_path = [documents stringByAppendingPathComponent:SYSTEM_LOG_INFO_FILE_NAME_IOS];
    return ([logInfo_path UTF8String]);
#elif Q_OS_ANDROID
    return SYSTEM_LOG_INFO_FILE_NAME_ANDROID;
#else
    return SYSTEM_LOG_INFO_FILE_NAME_OTHER;
#endif
}

FilePath::~FilePath(){}
