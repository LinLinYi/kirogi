#include <QtEndian>
#include "MD5.h"
#include "TeaCrypt.h"

#define TEA_LOOP 4
#define MD5_LENGTH 16
TeaCrypt::TeaCrypt(){}

int TeaCrypt::teaEncipher(unsigned char *inStr, unsigned char *outStr, quint32 pos, quint32 key[])
{
    quint32* inNow, *inPre;
    quint32* outNow, *outPre;
    quint32 delta = 0x9E3779B9;
    
    inNow = (quint32*)(inStr + pos);
    outNow = (quint32*)(outStr + pos);
    
    if(pos > 0){
        outPre = (quint32*)(outStr + pos - 8);
        inNow[0] = inNow[0] ^ outPre[0];
        inNow[1] = inNow[1] ^ outPre[1];
    }
    
    quint32 sum = 0;
    quint32 n = 1 << TEA_LOOP;
    quint32 y = qFromBigEndian(inNow[0]);
    quint32 z = qFromBigEndian(inNow[1]);
    for(quint32 i = 0; i < n; i++){
        sum += delta;
        y += ((z << 4) + key[0] ^ (z + sum) ^ (z >> 5) + key[1]);
        z += ((y << 4) + key[2] ^ (y + sum) ^ (y >> 5) + key[3]);
    }
    
    outNow[0] = qFromBigEndian(y);
    outNow[1] = qFromBigEndian(z);
    
    if(pos > 0){
        inPre = (quint32*)(inStr + pos - 8);
        outNow[0] = outNow[0] ^ inPre[0];
        outNow[1] = outNow[1] ^ inPre[1];
    }
    
    return 0;
}

int TeaCrypt::teaDecipher(unsigned char *inStr, unsigned char *outStr, quint32 pos, quint32 key[])
{
    quint32 delta = 0x9e3779b9;
    quint32 *inNow, *outPre, *outNow;
    
    inNow = (quint32 *)(inStr + pos);
    outNow = (quint32 *)(outStr + pos);
    if (pos > 0) {
        outPre = (quint32 *)(outStr + pos - 8);
        outNow[0] = inNow[0] ^ outPre[0];
        outNow[1] = inNow[1] ^ outPre[1];
    }
    else{
        memcpy(outStr, inStr, 8);
    }
    
    quint32 sum = delta << TEA_LOOP;
    quint32 n = 1 << TEA_LOOP;
    quint32 y = qFromBigEndian(outNow[0]);
    quint32 z = qFromBigEndian(outNow[1]);
    for (quint32 i = 0; i < n; i++) {
        z -= ((y << 4) + key[2]) ^ (y + sum) ^ ((y >> 5) + key[3]);
        y -= ((z << 4) + key[0]) ^ (z + sum) ^ ((z >> 5) + key[1]);
        sum -= delta;
    }

    outNow[0] = qFromBigEndian(y);
    outNow[1] = qFromBigEndian(z);
    return 0;
}

int TeaCrypt::teaEncrypt( unsigned char* inStr, quint32 inStrlen, unsigned char* keyStr, unsigned char* outStr)
{
    quint32 pos, outlen, key[4];
    unsigned char *plain, md[16];
    
    pos = (inStrlen + 10) % 8;
    if (pos) pos = 8 - pos;
    outlen = pos + inStrlen + 10;
    
    MD5 md5Pwd;//保证key为16byte
    md5Pwd.calc((const char*)keyStr, MD5_LENGTH, (char*)md);

    for (quint32 i = 0; i < 4; i++)
        key[i] = qFromBigEndian(*((uint32_t *)md + i));
    plain = (unsigned char*)malloc(outlen);

    for (quint32 i = 0; i < pos + 3; i++)
        plain[i] = rand() & 0xff;
    memcpy(plain + pos + 3, inStr, inStrlen);

    for (quint32 i = inStrlen + pos + 3; i < outlen - 1; i++)
        plain[i] = 0;
    plain[outlen - 1] = pos;

    for (quint32 i = 0; i < outlen; i += 8)
        teaEncipher(plain, outStr, i, key);
    free(plain);

    return outlen;
}

int TeaCrypt::teaDecrypt( unsigned char* inStr, quint32 inStrlen, unsigned char* keyStr, unsigned char*  outStr)
{
    quint32 pos, key[4];
    unsigned char md[16];
    
    if (inStrlen % 8 != 0 || inStrlen < 16) return -1;
    
    MD5 md5Pwd;//保证key为16byte
    md5Pwd.calc((const char*)keyStr, MD5_LENGTH, (char*)md);
    
    for (quint32 i = 0; i < 4; i++)
        key[i] = qFromBigEndian(*((uint32_t *)md + i));

    for (quint32 i = 0; i < inStrlen; i += 8)
        teaDecipher(inStr, outStr, i, key);

    for (quint32 i = 8; i < inStrlen; i += 4)
        *(quint32 *)(outStr + i) = *(quint32 *)(outStr + i) ^ *(quint32 *)(inStr + i - 8);

    pos = outStr[inStrlen - 1];
    if (pos > 7) return -1;
    
    for (quint32 i = inStrlen - 7; i < inStrlen - 1; i++) {
        if (outStr[i]) return -1;
    }
    
    quint32 outLen = 0;
    outLen = inStrlen - pos - 10;
    memmove(outStr, outStr + pos + 3, outLen + 1);
    return outLen;
}

TeaCrypt::~TeaCrypt(){}
