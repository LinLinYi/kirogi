#ifndef OUTPUTPACKET_H
#define OUTPUTPACKET_H

#include <QObject>
#include "BasePacket.h"

class OutputPacket : public BasePacket
{
public:
    OutputPacket();
    OutputPacket(quint64 id, quint16 cmd);
    ~OutputPacket();

public:
    char* getPackData();
    quint16 getPackDataLen();

    void dealData(char *buf, quint32 len);
    int getTimeoutCount() { return timeoutCount; }
    OutputPacket& operator=(const OutputPacket& outpack);
    void setTimeoutCount(int count) { timeoutCount = count; }

private:
    int timeoutCount;//超时次数
    QByteArray ciphertextArray;//密文数据
    static quint16 outPacketSerialNum;
    static quint16 outPacketVersionNum;

private:
    void packHeadData();
    void encryptDataField();
    void adjustEncryptType(char* key);

private:
    bool isEncryptByNull(quint16 cmdCode);
    bool isEncryptByLoginkey(quint16 cmdCode);
    bool isEncryptByVerifykey(quint16 cmdCode);
};

#endif // OUTPUTPACKET_H
