#include "UdpToServer.h"
#include "InputPacket.h"
#include "GlobalDefine.hpp"
#include "DistributeDataStation.h"

DistributeDataStation::DistributeDataStation(QObject* parent) : QObject(parent)
{
    udpToServer = new UdpToServer(this);
    connect(udpToServer, SIGNAL(readyRead(InputPacket&)), this, SLOT(on_udpFromNet(InputPacket&)));
}

void DistributeDataStation::fetchData(quint16 cmdCode, char *data, quint16 dataLen)
{
    udpToServer->fetchData(cmdCode, data, dataLen);
}

void DistributeDataStation::on_udpFromNet(InputPacket& inPack)
{
    quint16 cmdCode = inPack.getCmdCode();
    DPRINT("-->DistributeDataStation receive data, the cmdCode is:0x%x", cmdCode);

    emit readyRead(inPack);
}

DistributeDataStation::~DistributeDataStation(){}
