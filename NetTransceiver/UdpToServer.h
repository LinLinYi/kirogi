#ifndef UDPTOSERVER_H
#define UDPTOSERVER_H

#include <QObject>
#include <QTimer>

#include "InputPacket.h"
#include "UserProtocol.hpp"
#include "RetranMechanism.h"

class UdpToServer : public QObject
{
    Q_OBJECT
public:
    explicit UdpToServer(QObject *parent = 0);
    ~UdpToServer();

public:
    void fetchData(quint16 cmdCode, char* data, quint16 dataLen);

private:
    quint64 curLoginUserID;
    QString curLoginUserName;
    quint64 serverRandomNum;
    QTimer* heartBeat;
    RetranMechanism* retranMechanism;
    QHash<QString, quint64> loginInfoList;

private:
    void sendData(quint16 cmdCode, char* data, quint16 dataLen);
    void dealRegisterFirstStepReply(InputPacket& inpack);
    void dealRegisterSecondStepReply(InputPacket& inpack);
    void dealLoginFirstStepReply(InputPacket& inpack);
    void dealFetchSessionkeyReply(InputPacket& inpack);
    void dealForgetPasswordReply(InputPacket& inpack);
    void dealServerNotify(InputPacket& inpack);
    quint64 findLoginUserID(QString name);
    void matchUserID(InputPacket& pack);

signals:
    void readyRead(InputPacket& inpack);

private slots:
    void on_sendHeartBeat();
    void on_retranMechanism_readyRead(InputPacket& inpack);
};

#endif // UDPTOSERVER_H

