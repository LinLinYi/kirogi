#include "Keys.h"
#include "ToolKit.h"
#include "GlobalDefine.hpp"

char*   Keys::loginKey = NULL;
char*   Keys::sessionKey = NULL;
char*   Keys::verifyCodeKey = NULL;
char*   Keys::encryptFactor = NULL;
char*   Keys::verifyLoginKey = NULL;

void Keys::setSessionKey(char *key, quint32 keyLen)
{
    if(key == NULL || keyLen == 0) return;
    if(Keys::sessionKey!= NULL) SAFE_DELETE_ARRAY(sessionKey);
    Keys::sessionKey = new char[Keys::SessionKeyLen];
    memcpy(Keys::sessionKey, key, Keys::SessionKeyLen);
}

void Keys::setLoginKey(char *passwd, quint32 passwdLen)
{
    if(passwd == NULL || passwdLen == 0) return;

    MD5 md5Pwd;
    char pwdOut[Keys::MD5KeyLen];
    md5Pwd.calc(passwd, passwdLen, pwdOut);

    for(int i = 0; i< Keys::MD5KeyLen; i++)
        pwdOut[i] = pwdOut[i] ^ Keys::encryptFactor[i];

    if(Keys::loginKey != NULL) SAFE_DELETE_ARRAY(Keys::loginKey);
    Keys::loginKey = new char[Keys::MD5KeyLen];
    memcpy(Keys::loginKey, pwdOut, Keys::MD5KeyLen);
}

void Keys::setVerifyCodeKey(char *passwd, quint32 passwdLen)
{
    if(passwd == NULL || passwdLen == 0) return;

    MD5 md5Pwd;
    char pwdOut[Keys::MD5KeyLen];
    md5Pwd.calc(passwd, passwdLen, pwdOut);

    if(Keys::verifyCodeKey != NULL) SAFE_DELETE_ARRAY(Keys::verifyCodeKey);
    Keys::verifyCodeKey = new char[Keys::MD5KeyLen];
    memcpy(Keys::verifyCodeKey, pwdOut, Keys::MD5KeyLen);
}

void Keys::setEncryptFactor(char *key, quint32 keyLen)
{
    if(key == NULL || keyLen == 0) return;
    if(Keys::encryptFactor!= NULL) SAFE_DELETE_ARRAY(encryptFactor);
    Keys::encryptFactor = new char[Keys::EncryptFactorLen];
    memcpy(Keys::encryptFactor, key, Keys::EncryptFactorLen);
}

bool Keys::isMatchLoginPwd(char *passwd, quint32 passwdLen)
{
    if(passwd == NULL || passwdLen == 0) return false;

    MD5 md5Pwd;
    char pwdOut[Keys::MD5KeyLen];
    md5Pwd.calc(passwd, passwdLen, pwdOut);

    for(int i = 0; i< Keys::MD5KeyLen; i++)
        pwdOut[i] = pwdOut[i] ^ Keys::encryptFactor[i];

    if(Keys::verifyLoginKey != NULL) SAFE_DELETE_ARRAY(Keys::verifyLoginKey);
    Keys::verifyLoginKey = new char[Keys::MD5KeyLen];
    memcpy(Keys::verifyLoginKey, pwdOut, Keys::MD5KeyLen);

    for(int i = 0; i < Keys::MD5KeyLen; i++){
        if(Keys::verifyLoginKey[i] != Keys::loginKey[i])
            return false;
    }
    return true;
}

void Keys::deleteAllKeys()
{
    SAFE_DELETE_ARRAY(loginKey);
    SAFE_DELETE_ARRAY(sessionKey);
    SAFE_DELETE_ARRAY(encryptFactor);
    SAFE_DELETE_ARRAY(verifyCodeKey);
    SAFE_DELETE_ARRAY(verifyLoginKey);
}

Keys::~Keys()
{
    deleteAllKeys();
}
