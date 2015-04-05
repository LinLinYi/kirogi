#include "ToolKit.h"
#include "UserProtocol.hpp"
#include "RetranMechanism.h"

RetranMechanism::RetranMechanism(QObject* parent)
    : QObject(parent)
    , timer(NULL)
{
    this->settingTimer();
    this->settingTransceiver();
}

void RetranMechanism::settingTimer()
{
    timer = new QTimer(this);
    defaultMaxTimeoutTimes = 3;
    defaultTimeInterval = 5000;
    connect(timer, SIGNAL(timeout()), this, SLOT(on_timeout()));
}

void RetranMechanism::settingTransceiver()
{
    udpTransceiver = new UdpTransceiver(this);
    udpTransceiver->setDestIP(DestServer::SERVER_IP);
    udpTransceiver->setDestPort(DestServer::SERVER_PORT);
    connect(udpTransceiver, SIGNAL(readyRead(QByteArray&,QHostAddress&,quint16)),
            this, SLOT(on_udpTransceiver(QByteArray&,QHostAddress&,quint16)));
}

void RetranMechanism::appendOutpack(OutputPacket* outpack)
{
    if(outpack->getCmdCode() == CommandCode::HEART_BEAT || outpack->getCmdCode() == CommandCode::SERVER_NOTIFY){//心跳包、服务器通知回复不重发
        char* data = outpack->getPackData();
        quint16 dataLen = outpack->getPackDataLen();
        udpTransceiver->sendData(data, dataLen);
        delete outpack;
        return;
    }

    outputPackList.insert(outpack->getSerialNum(), outpack);//保存发送包列表
}

void RetranMechanism::startTimer(int interval)
{
    if(outputPackList.count() < 1)
        return;//列表为空

    if(interval <= 0)//默认重发3次
        timeInterval = defaultTimeInterval;
    else
        timeInterval = interval;

    if(!timer->isActive())//检测定时器是否打开
    {
        this->sendData(outputPackList);
        timer->start(timeInterval);
    }
    else
    {
        QHashIterator<quint16, OutputPacket*> iter(outputPackList);
        while(iter.hasNext())
        {
            iter.next();
            OutputPacket* pack = iter.value();
            if(pack->getTimeoutCount() == 0){//数据包第一次直接发送，重发计数加1
                int count = pack->getTimeoutCount() + 1;
                pack->setTimeoutCount(count);

                char* data = pack->getPackData();
                quint16 dataLen = pack->getPackDataLen();
                udpTransceiver->sendData(data, dataLen);
            }
        }
    }
}

void RetranMechanism::stopTimer()
{
    if(timer->isActive())
        timer->stop();
}

bool RetranMechanism::isActive()
{
    return timer->isActive();
}

void RetranMechanism::setMaxTimeoutTimes(quint8 times)
{
    if(times <= 0)
        maxTimeoutTimes = defaultMaxTimeoutTimes;
    else
        maxTimeoutTimes = times;
}

void RetranMechanism::setInterval(int interval)
{
    if(interval <= 0)
        timeInterval = defaultTimeInterval;
    else
        timeInterval = interval;
}

void RetranMechanism::sendData(QHash<quint16, OutputPacket*> packList)
{
    QHashIterator<quint16, OutputPacket*> iter(packList);
    while(iter.hasNext()){
        iter.next();
        OutputPacket* pack = iter.value();
        int count = pack->getTimeoutCount() + 1;
        pack->setTimeoutCount(count);

        char* data = pack->getPackData();
        quint16 dataLen = pack->getPackDataLen();
        udpTransceiver->sendData(data, dataLen);
    }
}
bool RetranMechanism::reviewPackNum(quint16 serialNum)
{
    if(outputPackList.contains(serialNum)){//收发包序号匹配
        OutputPacket* pack = outputPackList.value(serialNum);
        outputPackList.remove(serialNum);//移除已成功操作的数据包
        delete pack;
        return true;
    }
    return false;
}

void RetranMechanism::sendNextPackage()
{
    if(outputPackList.count() > 0){
        this->sendData(outputPackList);
    }else{
        this->stopTimer();
    }
}

void RetranMechanism::netErrorWaring()
{   
    //网络错误处理
    InputPacket inpack;
    inpack.setCmdCode(CommandCode::NET_ERROR);
    inpack.setAnswerCode(0);
    emit readyRead(inpack);
}

void RetranMechanism::on_timeout()
{
    QHashIterator<quint16, OutputPacket*> iter(outputPackList);
    while(iter.hasNext()){
        iter.next();
        OutputPacket* pack = iter.value();

        if(pack->getTimeoutCount() == maxTimeoutTimes){//达到超时次数,返回网络错误
            this->netErrorWaring();
            outputPackList.remove(pack->getSerialNum());
            delete pack;//删除达到超时次数的数据包
        }
    }
    this->sendNextPackage();
}

void RetranMechanism::on_udpTransceiver(QByteArray& datagram, QHostAddress& hostIP, quint16 hostPort)
{
    if(!((quint8)datagram[0] == ConstNum::PACKET_START_SYSMBOL && (quint8)datagram[datagram.size() - 1] ==
         ConstNum::PACKET_END_SYSMBOL && hostIP.toString() == DestServer::SERVER_IP && hostPort == DestServer::SERVER_PORT)){
        return;
    }

    //数据包解包解密
    InputPacket inpack(datagram.data(), datagram.size());
    quint16 serialNum = inpack.getSerialNum();
    quint16 cmdCode = inpack.getCmdCode();

    this->reviewPackNum(serialNum);
    bool isValidCmdCode = (cmdCode >= CommandCode::MIN_VALUE && cmdCode <= CommandCode::MAX_VALUE);

    if(isValidCmdCode){
        emit readyRead(inpack);
    }
}

RetranMechanism::~RetranMechanism()
{

}
