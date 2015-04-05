#ifndef UDPTRANSCEIVER_H
#define UDPTRANSCEIVER_H

#include <QUdpSocket>

class UdpTransceiver : public QObject
{
    Q_OBJECT
public:
    UdpTransceiver(QObject* parent = 0, QString ip = 0, quint16 port = 0);
    ~UdpTransceiver();

public:
    quint16 sendData(const char* outStr, quint16 len);
    quint16 sendData(QByteArray byteArray);

    void setDestIP(QString ip);
    void setDestPort(quint16 port);

    void binding();
    void binding(quint16 port);

signals:
    void readyRead(QByteArray& byte, QHostAddress& ip, quint16 port);

public slots:
    void on_processPendingData();

private:
    QUdpSocket *udpSocket;      /*! udp套接字*/
    QHostAddress destIP;        /*! 服务器IP地址*/
    quint16 destPort;           /*! 服务器端口号*/
    quint16 localPort;          /*! 本地绑定端口号*/

private:
    void insureUsefulUdpSocket();
};

#endif //UDPTRANSCEIVER_H

