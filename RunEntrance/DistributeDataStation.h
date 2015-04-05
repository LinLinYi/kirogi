#ifndef DISTRIBUTEDATASTATION_H
#define DISTRIBUTEDATASTATION_H

class UdpToServer;
class InputPacket;

#include <QObject>

class DistributeDataStation : public QObject
{
    Q_OBJECT
public:
    explicit DistributeDataStation(QObject* parent);
    virtual ~DistributeDataStation();

public:
    void fetchData(quint16 cmdCode, char* data, quint16 dataLen);

private:
    UdpToServer* udpToServer;

signals:
    void readyRead(InputPacket& inPack);

private slots:
    void on_udpFromNet(InputPacket& inPack);
};

#endif // DISTRIBUTEDATASTATION_H
