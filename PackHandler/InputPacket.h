#ifndef INPUTPACKET_H
#define INPUTPACKET_H

#include "BasePacket.h"
#include <QObject>

class InputPacket : public BasePacket
{
public:
    explicit InputPacket(){ }
    explicit InputPacket(char *buf, quint32 len);
    virtual ~InputPacket();

public:
    char* getPackData();
    quint16 getPackDataLen();
    void setPackData(char *data, quint16 dataLen);

    void dealData(char *buf, quint32 len);
    InputPacket& operator=(const InputPacket& inpack);

private:
    static quint16 inputPacketSerialNum;
    QByteArray plaintextArray;/*! 明文数据*/

private:
    void decryptDataField();
    void unpackHeadData(char *instr, quint32 inLen);
    int adjustDecryptType(char *key);

private:
    bool isDecryptByNull(quint16 cmdCode);
    bool isDecryptByLoginkey(quint16 cmdCode);
    bool isDecryptByVerifykey(quint16 cmdCode);
};
#endif // INPUTPACKET_H
