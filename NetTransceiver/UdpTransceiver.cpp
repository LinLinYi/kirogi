#include "UdpTransceiver.h"
#include "ToolKit.h"
#include "GlobalDefine.hpp"

UdpTransceiver::UdpTransceiver( QObject *parent, QString ip, quint16 port)
    : QObject(parent)
    , udpSocket(NULL)
{
    destIP.setAddress(ip);
    destPort = port;
    udpSocket = new QUdpSocket(this);
    connect(udpSocket, SIGNAL(readyRead()), this, SLOT(on_processPendingData()));
}

quint16 UdpTransceiver::sendData(const char* outStr, quint16 len)
{
    qint64 sentLen = 0;
    if(udpSocket != NULL)
        sentLen = udpSocket->writeDatagram((const char*)outStr, len, destIP, destPort);

    if(sentLen <= 0){
        insureUsefulUdpSocket();
        sentLen = udpSocket->writeDatagram((const char*)outStr, len, destIP, destPort);
    }

    DPRINT("--->UdpTransceiver has been sent, the message length:%d", (qint32)sentLen);
    return sentLen;
}

quint16 UdpTransceiver::sendData(QByteArray byteArray)
{
    qint64 sentLen = 0;
    if(udpSocket != NULL)
        sentLen = udpSocket->writeDatagram(byteArray, destIP, destPort);

    if(sentLen <= 0){
        insureUsefulUdpSocket();
        sentLen = udpSocket->writeDatagram(byteArray, destIP, destPort);
    }

    DPRINT("--->Udp msgdata has be sent, the message length:%d", (qint32)sentLen);
    return sentLen;
}

void UdpTransceiver::on_processPendingData()
{
    QByteArray datagram;
    QHostAddress hostIP;
    quint16 hostPort;

    while(udpSocket->hasPendingDatagrams()){
        datagram.resize(udpSocket->pendingDatagramSize());
        DPRINT("--->Udp msgdata has be received, msgLength = %d", datagram.size());
        udpSocket->readDatagram(datagram.data(), datagram.size(), &hostIP, &hostPort);
        emit readyRead(datagram, hostIP, hostPort);
    }
}

void UdpTransceiver::insureUsefulUdpSocket()
{
    SAFE_DELETE(udpSocket);
    while(udpSocket == NULL){
        udpSocket = new QUdpSocket(this);
    }
    connect(udpSocket, SIGNAL(readyRead()), this, SLOT(on_processPendingData()));
}

void UdpTransceiver::binding()
{
    int localPort;
    for ( localPort = 5000; localPort <= 65535; localPort++ ){
        if ( !udpSocket->bind( localPort, QAbstractSocket::DontShareAddress) )
            continue;
        else
            break;
    }
}

void UdpTransceiver::setDestIP(QString ip)
{
    destIP.setAddress(ip);
}

void UdpTransceiver::setDestPort(quint16 port)
{
    destPort = port;
}

void UdpTransceiver::binding(quint16 port)
{
    udpSocket->bind(port);
}

UdpTransceiver::~UdpTransceiver()
{
    SAFE_DELETE(udpSocket);
}



