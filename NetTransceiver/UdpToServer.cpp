#include <String>

#include "UdpToServer.h"
#include "ToolKit.h"
#include "Keys.h"
#include "AssemblyData.h"
#include "FilePath.h"
#include "OutPutPacket.h"
#include "GlobalData.h"
#include "DispersalData.h"
#include "GlobalDefine.hpp"
#include "ServerNotifyInfo.h"

UdpToServer::UdpToServer(QObject *parent)
    : QObject(parent)
    , curLoginUserID(0)
{
    retranMechanism = new RetranMechanism(this);//超时重发
    connect(retranMechanism, SIGNAL(readyRead(InputPacket&)),
            this, SLOT(on_retranMechanism_readyRead(InputPacket&)));
}

void UdpToServer::fetchData(quint16 cmdCode, char* data, quint16 dataLen)
{
    AssemblyData assemblyData;
    if(cmdCode == CommandCode::LOGIN_FIRST_STEP){
        curLoginUserName = QString(data);
        curLoginUserID =  this->findLoginUserID(QString(data));
    }else if(cmdCode == CommandCode::LOGIN_SECOND_STEP){
        Keys::setLoginKey(data, dataLen);//设置登录秘钥
        assemblyData.append(serverRandomNum);//发送随机数
        data = assemblyData.getAssemblyData();
        dataLen = assemblyData.getAssemblyDataLen();
    }else if(cmdCode == CommandCode::FORGET_PASSWORD_VERIFY){
        Keys::setVerifyCodeKey(data, dataLen);//设置验证密钥,可变
    }

    this->sendData(cmdCode, data, dataLen);
}

void UdpToServer::on_retranMechanism_readyRead(InputPacket &inpack)
{
    quint16 cmdCode = inpack.getCmdCode();
    int dataLen = inpack.getPackDataLen();
    if(dataLen < 0) return;//数据域数据长度小于0，说明数据有误，不处理
    DPRINT("-->UdpToServer send data to UI, the cmdCode is:0x%x", cmdCode);

    switch(cmdCode)
    {
        case CommandCode::REGISTER_FIRST_STEP://注册第一步(申请)
            this->dealRegisterFirstStepReply(inpack);
            break;
        case CommandCode::REGISTER_SECOND_STEP://注册第二步(验证)
            this->dealRegisterSecondStepReply(inpack);
            break;
        case CommandCode::FORGET_PASSWORD_APPLY://忘记密码申请
            this->dealForgetPasswordReply(inpack);
            break;
        case CommandCode::LOGIN_FIRST_STEP://登录第一步
            this->dealLoginFirstStepReply(inpack);
            break;
        case CommandCode::LOGIN_SECOND_STEP://登录第二步
            this->dealFetchSessionkeyReply(inpack);
            break;
        case CommandCode::SERVER_NOTIFY://服务器通知
            this->dealServerNotify(inpack);
            break;
        default://其他
            this->matchUserID(inpack);
            break;
    }
}

void UdpToServer::sendData(quint16 cmdCode, char* data, quint16 dataLen)
{
    OutputPacket* outputPacket = new OutputPacket(curLoginUserID, cmdCode);
    outputPacket->dealData(data,dataLen);

    retranMechanism->appendOutpack(outputPacket);
    retranMechanism->setMaxTimeoutTimes(ConstNum::MAX_PACK_RETRAN_TIMES);
    retranMechanism->startTimer(ConstNum::MAX_PACK_RETRAN_TIME_INTERVAL);
    DPRINT("-->UdpToServer send data to net , the cmdCode is:0x%x", cmdCode);
}

void UdpToServer::dealRegisterFirstStepReply(InputPacket &inpack)
{
    quint8 answerCode = inpack.getAnswerCode();
    if(answerCode == Mi::Success){
        curLoginUserID = inpack.getUserID();
    }
    
    emit readyRead(inpack);
}

void UdpToServer::dealLoginFirstStepReply(InputPacket& inpack)
{
    int offset = 0;
    quint8 answerCode = inpack.getAnswerCode();
    char* dataField = inpack.getPackData();

    if(answerCode == Mi::Success){
        curLoginUserID = inpack.getUserID();//获取登录用户ID
        loginInfoList.insert(curLoginUserName, curLoginUserID);
        DispersalData dispersalData;//获取随机数、加密因子
        offset += dispersalData.outlet(serverRandomNum, dataField);
        Keys::setEncryptFactor(dataField + offset, Keys::EncryptFactorLen);

        GlobalData::setAccountID(curLoginUserID);//保存当前账号ID
        GlobalData::setAccountName(curLoginUserName);//保存当前账号名称
    }

    emit readyRead(inpack);
}

void UdpToServer::dealRegisterSecondStepReply(InputPacket &inpack)
{
    emit readyRead(inpack);
}

void UdpToServer::dealFetchSessionkeyReply(InputPacket& inpack)
{
    char* dataField = inpack.getPackData();
    quint8 answerCode = inpack.getAnswerCode();

    if(answerCode == Mi::Success){
        Keys::setSessionKey(dataField, Keys::SessionKeyLen);//设置会话秘钥
        heartBeat = new QTimer(this); //登录成功，启用心跳包
        heartBeat->start(ConstNum::MAX_HEARTBEAT_TIME_INTERVAL);
        connect(heartBeat, SIGNAL(timeout()), this, SLOT(on_sendHeartBeat()));

        emit readyRead(inpack);
        return;
    }else if(inpack.getPackDataLen() <= 0){
        inpack.setAnswerCode(Mi::Error);
    }
    emit readyRead(inpack);
}

void UdpToServer::dealForgetPasswordReply(InputPacket &inpack)
{
    curLoginUserID = inpack.getUserID();
    emit readyRead(inpack);
}

quint64 UdpToServer::findLoginUserID(QString name)
{
    quint64 userID;
    if(loginInfoList.contains(name))
        userID = loginInfoList.value(name);
    else
        userID = 0;

    return userID;
}

void UdpToServer::dealServerNotify(InputPacket &inpack)
{
    char* data = inpack.getPackData();
    quint16 dataLen = inpack.getPackDataLen();

    ServerNotifyInfo serverNotifyInfo(data, dataLen);
    quint16 msgCmd = serverNotifyInfo.getCmdCode();
    quint32 notifyNum = serverNotifyInfo.getNotifyNum();

    //回复服务器通知,数据域打包
    AssemblyData assemblyData;
    assemblyData.append(notifyNum);
    assemblyData.append(msgCmd);
    quint16 bufLen = assemblyData.getAssemblyDataLen();
    char* dataBuf = assemblyData.getAssemblyData();
    this->sendData(CommandCode::SERVER_NOTIFY, dataBuf, bufLen);

    //发送通知到上层
    emit readyRead(inpack);
}

void UdpToServer::matchUserID(InputPacket& inpack)
{
    if(curLoginUserID == inpack.getUserID() || inpack.getCmdCode() == CommandCode::NET_ERROR)
        emit readyRead(inpack);
}

void UdpToServer::on_sendHeartBeat()
{
    this->sendData(CommandCode::HEART_BEAT, NULL, 0);
    DPRINT("-->UdpToServer send heartbeat packet to server, the cmdCode is:0x%x", CommandCode::HEART_BEAT);
}

UdpToServer::~UdpToServer(){}

