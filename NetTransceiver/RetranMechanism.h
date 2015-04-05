#ifndef RETRANMECHANISM_H
#define RETRANMECHANISM_H

#include <QObject>
#include <QTimer>
#include <QQueue>
#include "InputPacket.h"
#include "OutPutPacket.h"
#include "UdpTransceiver.h"

class RetranMechanism : public QObject
{
    Q_OBJECT
public:
    explicit RetranMechanism(QObject* parent = 0);
    virtual ~RetranMechanism();

public:
    void appendOutpack(OutputPacket* outpack);
    void startTimer(int interval = 0);
    void stopTimer();
    bool isActive();
    void setInterval(int interval);
    void setMaxTimeoutTimes(quint8 times);

private:
    QTimer* timer;
    quint8 maxTimeoutTimes;
    quint8 defaultMaxTimeoutTimes;
    quint16 timeInterval;
    quint16 defaultTimeInterval;
    UdpTransceiver* udpTransceiver;
    QHash<quint16, OutputPacket*> outputPackList;

private:
    void settingTimer();
    void settingTransceiver();
    void sendNextPackage();
    void netErrorWaring();
    bool reviewPackNum(quint16 serialNum);
    void sendData(QHash<quint16, OutputPacket*> packList);

signals:
    void readyRead(InputPacket& inpack);

private slots:
    void on_timeout();
    void on_udpTransceiver(QByteArray& datagram, QHostAddress& hostIP, quint16 hostPort);
};

#endif // RETRANMECHANISM_H
