#include "stdlib.h"
#include "AssemblyData.h"
#include "GlobalDefine.hpp"

AssemblyData::AssemblyData(QObject* parent) : QObject(parent)
{
    assemblyDataLen = 0;
    initialBufLen = 1024;
    assemblyBuf = new char[initialBufLen];
    memset(assemblyBuf, 0, initialBufLen);
}

void AssemblyData::append(const QString &src)
{
    quint16 tempLen = assemblyDataLen + src.length();
    if(tempLen >= initialBufLen) return;

    std::string str = src.toStdString();
    const char* ch = str.c_str();
    memccpy(assemblyBuf + assemblyDataLen, ch, '\0', initialBufLen);
    assemblyDataLen += strlen(ch) + 1;
}

void AssemblyData::append(char *src, quint16 srcLen)
{
    quint16 tempLen = assemblyDataLen + srcLen;
    if(tempLen >= initialBufLen) return;

    memcpy(assemblyBuf + assemblyDataLen, src, initialBufLen);
    assemblyDataLen += srcLen;
}

char* AssemblyData::getAssemblyData()
{
    return assemblyBuf;
}

quint16 AssemblyData::getAssemblyDataLen()
{
    return assemblyDataLen;
}

AssemblyData::~AssemblyData()
{
    SAFE_DELETE_ARRAY(assemblyBuf);
}
