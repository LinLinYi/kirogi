#ifndef KEYS_H
#define KEYS_H

#include <QString>
#include "MD5.h"

/*! \class Keys
 *  \brief 用来保存和生成各种秘钥的仓库
 */
class Keys
{
public:
    Keys(){}
    ~Keys();

public:
    static const int MD5KeyLen = 16;
    static const int LoginKeyLen = 16;
    static const int SessionKeyLen = 16;
    static const int EncryptFactorLen = 16;
    static const int VerifyCodeKeyLen = 16;

public:
    static char* getSessionKey(){ return sessionKey; }
    static char* getLoginKey(){ return loginKey; }
    static char* getVerifyCodeKey(){ return verifyCodeKey; }
    static char* getEncryptFactor(){ return encryptFactor; }
    static void setSessionKey(char *key, quint32 keyLen);
    static void setLoginKey(char *passwd, quint32 passwdLen);
    static void setVerifyCodeKey(char* passwd, quint32 passwdLen);
    static void setEncryptFactor(char *key, quint32 keyLen);
    static bool isMatchLoginPwd(char *passwd, quint32 passwdLen);
    static void deleteAllKeys();

private:
    static char *sessionKey;        /*! 会话秘钥*/
    static char *loginKey;          /*! 登录秘钥*/
    static char* encryptFactor;     /*! 加密因子*/
    static char* verifyCodeKey;     /*! 验证码MD5*/
    static char* verifyLoginKey;    /*! 需要验证的登录密钥*/
};

#endif // SECRETKEY_H
