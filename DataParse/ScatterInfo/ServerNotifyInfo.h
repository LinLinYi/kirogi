#ifndef SERVERNOTIFYINFO_H
#define SERVERNOTIFYINFO_H

#include <QObject>

class ServerNotifyInfo
{
public:
    explicit ServerNotifyInfo();
    explicit ServerNotifyInfo(char* data, quint16 dataLen);
    virtual ~ServerNotifyInfo();

public:
    quint16 getCmdCode();
    quint32 getNotifyNum();
    char* getData();
    quint16 getDataLen();
    void unpack(char* data, quint16 dataLen);

private:
    quint16 m_cmdCode;
    quint32 m_notifyNum;
    quint16 m_dataLen;
    QByteArray m_data;
};

#endif // SERVERNOTIFYINFO_H
