#ifndef ASSEMBLYDATA_H
#define ASSEMBLYDATA_H

#include <QObject>
#include "ToolKit.h"

class AssemblyData : public QObject
{
public:
    explicit AssemblyData(QObject* parent = 0);
    virtual ~AssemblyData();

public:
    template<typename T> void append(T src){
        quint16 tempLen = assemblyDataLen + sizeof(T);
        if(tempLen >= initialBufLen) return;

        ToolKit::HostToNetEndian(assemblyBuf + assemblyDataLen, src);
        assemblyDataLen += sizeof(T);
    }

    void append(const QString& str);
    void append(char* src, quint16 srcLen);
    char* getAssemblyData();
    quint16 getAssemblyDataLen();

private:
    char* assemblyBuf;
    quint16 initialBufLen;
    quint16 assemblyDataLen;
};

#endif // ASSEMBLYDATA_H
