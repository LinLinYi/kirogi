#ifndef TEACRYPT_H
#define TEACRYPT_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

class TeaCrypt
{
public:
    TeaCrypt();
    ~TeaCrypt();

public:
    static int teaEncrypt(unsigned char* inStr, quint32 inStrlen, unsigned char* keyStr, unsigned char*  outStr);
    static int teaDecrypt(unsigned char* inStr, quint32 inStrlen, unsigned char* keyStr, unsigned char*  outstr);
private:
    static int teaEncipher(unsigned char *inStr, unsigned char *outStr, quint32 pos, quint32 key[]);
    static int teaDecipher(unsigned char *inStr, unsigned char *outStr, quint32 pos, quint32 key[]);
};

#endif // TEACRYPT_H
