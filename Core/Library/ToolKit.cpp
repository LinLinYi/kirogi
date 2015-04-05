#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "ToolKit.h"
#include "TeaCrypt.h"
#include "UserProtocol.hpp"

ToolKit::ToolKit(){}
ToolKit::~ToolKit(){}

QByteArray ToolKit::shortToByteArray(unsigned short conData)
{
    QByteArray conRslt;
    conRslt[0] = ( conData & 0x0000ff00 ) >> 8;
    conRslt[1] = conData & 0x000000ff;
    return conRslt;
}

quint16 ToolKit::cutCharPtr(char *dest, char *src)
{
    char* offset = (char*)memccpy(dest, src, '\0', ConstNum::MAX_PACKET_SIZE);
    quint16 len = offset - dest;
    return len;
}

quint16 ToolKit::cutCharPtr(char *dest, char *src, quint16 len)
{
    memcpy(dest, src, len);
    return len;
}

int ToolKit::EncryptData(unsigned char *instr, quint32 instrlen, unsigned char *key, unsigned char *outstr)
{
    if(instr == NULL || instrlen == 0 || key == NULL || outstr == NULL) return -1;
    return TeaCrypt::teaEncrypt(instr, instrlen, key, outstr);
}

int ToolKit::DecryptData(unsigned char *instr, quint32 instrlen, unsigned char *key, unsigned char *outstr)
{
    if(instr == NULL || instrlen == 0 || key == NULL || outstr == NULL) return -1;
    return TeaCrypt::teaDecrypt(instr, instrlen, key, outstr);
}

