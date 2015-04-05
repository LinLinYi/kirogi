#include "DispersalData.h"
#define MAX_PACK_SIZE 1024

DispersalData::DispersalData(QObject* parent) : QObject(parent){}
quint16 DispersalData::outlet(QString& dest, char *src)
{
    char buf[MAX_PACK_SIZE] = {0};
    char* offset = (char*)memccpy(buf, src, '\0', MAX_PACK_SIZE);
    dest = QString(buf);

    quint16 len = offset - buf;
    return len;
}

quint16 DispersalData::outlet(char *dest, char *src)
{
    if(dest == NULL || src == NULL) return 0;
    char* offset = (char*)memccpy(dest, src, '\0', MAX_PACK_SIZE);
    quint16 len = offset - dest;
    return len;
}

quint16 DispersalData::outlet(char *dest, char *src, quint16 len)
{
    if(dest == NULL || src == NULL || len == 0) return 0;
    memcpy(dest, src, len);
    return len;
}

DispersalData::~DispersalData()
{

}
