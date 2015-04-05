#ifndef TOOLKIT_H
#define TOOLKIT_H

#include <QString>
#include <QtEndian>

class ToolKit
{
public:
    ToolKit();
    ~ToolKit();

    template<typename T> static void NetToHostEndian(T& dest, char *src){
        if(src == NULL){ return; }
        T temp;
        memcpy((void*) &temp, (void*) src, sizeof(T));
        dest = qFromLittleEndian(temp);
    }

    template<typename T> static void HostToNetEndian(char *dest, const T& src){
        if(dest == NULL){ return; }
        uchar* temp = (uchar*)dest;
        qToLittleEndian(src, temp);
    }

    static quint16 cutCharPtr(char* dest, char* src);
    static quint16 cutCharPtr(char *dest, char *src, quint16 len);
    static QByteArray shortToByteArray( unsigned short conData );

    static int EncryptData(unsigned char *instr, quint32 instrlen, unsigned char *key, unsigned char *outstr);
    static int DecryptData(unsigned char *instr, quint32 instrlen, unsigned char *key, unsigned char *outstr);
};

#endif // TOOLKIT_H
