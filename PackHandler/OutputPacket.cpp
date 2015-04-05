#include <QtEndian>

#include "OutPutPacket.h"
#include "ToolKit.h"
#include "Keys.h"
#include "AssemblyData.h"
#include "UserProtocol.hpp"
#include "GlobalDefine.hpp"

quint16 OutputPacket::outPacketSerialNum = 0;
quint16 OutputPacket::outPacketVersionNum = Mi::Version;
OutputPacket::OutputPacket()
{
    dataField = NULL;
    dataFieldLen = 0;
    timeoutCount = 0;
    versionNum = outPacketVersionNum;
    serialNum = (outPacketSerialNum++) % 0xffff;
}

OutputPacket::OutputPacket(quint64 id, quint16 cmd)
{
    userID = id;
    destID = 0;
    cmdCode = cmd;
    dataField = NULL;
    dataFieldLen = 0;
    timeoutCount = 0;
    versionNum = outPacketVersionNum;
    serialNum = (outPacketSerialNum++) % 0xffff;
}

void OutputPacket::dealData(char *inStr, quint32 inLen)
{
    if(inStr != NULL){//获取数据域
        SAFE_DELETE_ARRAY(dataField);
        dataFieldLen = inLen;
        dataField = new char[inLen];
        memset(dataField, 0, inLen);
        memcpy(dataField, inStr, dataFieldLen);
    }

    ciphertextArray.clear();//清空当前数据
    packHeadData();         //包头组装
    encryptDataField();     //包加密组装
}

void OutputPacket::packHeadData()
{
    AssemblyData assemblyData;
    assemblyData.append(ConstNum::PACKET_START_SYSMBOL);
    assemblyData.append(serialNum);
    assemblyData.append(cmdCode);
    assemblyData.append(versionNum);
    assemblyData.append(userID);
    assemblyData.append(destID);

    char* data = assemblyData.getAssemblyData();
    quint16 dataLen =  assemblyData.getAssemblyDataLen();
    ciphertextArray.append(data, dataLen);
}

void OutputPacket::encryptDataField()
{
    if(isEncryptByNull(cmdCode))//有数据域,不加密
        adjustEncryptType(NULL);
    else if(isEncryptByLoginkey(cmdCode))//登录秘钥加密数据域
        adjustEncryptType(Keys::getLoginKey());
    else if(isEncryptByVerifykey(cmdCode))//验证码MD5加密数据域
        adjustEncryptType(Keys::getVerifyCodeKey());
    else//会话密钥加密数据域
        adjustEncryptType(Keys::getSessionKey());
}

void OutputPacket::adjustEncryptType(char *key)
{
    char dataLenPBuf[2] = { 0 };
    if(key != NULL && dataField != NULL && dataFieldLen != 0){//有数据域,加密
        int encryptBufLen = 0;
        char encryptBuf[ConstNum::MAX_PACKET_SIZE] = { 0 };

        encryptBufLen = ToolKit::EncryptData((uchar*)dataField, dataFieldLen, (uchar*)key, (uchar*)encryptBuf);
        ToolKit::HostToNetEndian(dataLenPBuf, (quint16)encryptBufLen);
        ciphertextArray.append(dataLenPBuf, sizeof(quint16));//数据长度
        ciphertextArray.append(encryptBuf, encryptBufLen);//数据加密内容
    }
    else if(dataField == NULL && dataFieldLen == 0){//无数据域,不加密
        ToolKit::HostToNetEndian(dataLenPBuf, dataFieldLen);
        ciphertextArray.append(dataLenPBuf, sizeof(quint16));
    }
    else if(dataField != NULL && dataFieldLen != 0){//有数据域,不加密
        ToolKit::HostToNetEndian(dataLenPBuf, dataFieldLen);
        ciphertextArray.append(dataLenPBuf, sizeof(quint16));
        ciphertextArray.append(dataField, dataFieldLen);
    }
    ciphertextArray.append((char) ConstNum::PACKET_END_SYSMBOL);//包后缀
    SAFE_DELETE_ARRAY(dataField);
}

OutputPacket& OutputPacket::operator=(const OutputPacket& outpack)
{
    prefix = outpack.prefix;
    answerCode = outpack.answerCode;
    cmdCode = outpack.cmdCode;
    dataFieldLen = outpack.dataFieldLen;
    serialNum = outpack.serialNum;
    userID = outpack.userID;
    destID = outpack.destID;
    versionNum = outpack.versionNum;
    postfix = outpack.postfix;
    ciphertextArray = outpack.ciphertextArray;
    return *this;
}

bool OutputPacket::isEncryptByLoginkey(quint16 cmdCode)
{
    switch(cmdCode){
        case CommandCode::LOGIN_SECOND_STEP:
        return true;
    }
    return false;
}

bool OutputPacket::isEncryptByVerifykey(quint16 cmdCode)
{
    switch(cmdCode){
        case CommandCode::FORGET_PASSWORD_RESET:
        return true;
    }
    return false;
}

bool OutputPacket::isEncryptByNull(quint16 cmdCode)
{
    switch(cmdCode){
        case CommandCode::FORGET_PASSWORD_APPLY:
        case CommandCode::FORGET_PASSWORD_VERIFY:
        case CommandCode::LOGIN_FIRST_STEP:
        case CommandCode::REGISTER_FIRST_STEP:
        case CommandCode::REGISTER_SECOND_STEP:
        case CommandCode::REGISTER_THIRD_STEP:
        return true;
    }
    return false;
}

char* OutputPacket::getPackData()
{
    return ciphertextArray.data();
}

quint16 OutputPacket::getPackDataLen()
{
    return ciphertextArray.length();
}


OutputPacket::~OutputPacket()
{

}
