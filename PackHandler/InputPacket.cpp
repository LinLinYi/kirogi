#include <QtEndian>

#include "InputPacket.h"
#include "ToolKit.h"
#include "Keys.h"
#include "GlobalDefine.hpp"
#include "DispersalData.h"
#include "UserProtocol.hpp"

quint16 InputPacket::inputPacketSerialNum = 0;

InputPacket::InputPacket(char *buf, quint32 len)
{
    plaintextArray.clear();
    this->unpackHeadData(buf, len);
    this->decryptDataField();
}

void InputPacket::dealData(char *buf, quint32 len)
{
    plaintextArray.clear();
    this->unpackHeadData(buf, len);
    this->decryptDataField();
}

InputPacket& InputPacket::operator =(const InputPacket& inpack)
{
    prefix = inpack.prefix;
    answerCode = inpack.answerCode;
    cmdCode = inpack.cmdCode;
    dataFieldLen = inpack.dataFieldLen;
    serialNum = inpack.serialNum;
    userID = inpack.userID;
    destID = inpack.destID;
    versionNum = inpack.versionNum;
    postfix = inpack.postfix;
    plaintextArray = inpack.plaintextArray;
    return *this;
}

void InputPacket::unpackHeadData(char* data, quint32 dataLen)
{
    if(data == NULL || dataLen == 0) return;

    int offset = 0;
    DispersalData dispersalData;
    offset += dispersalData.outlet(prefix, data + offset);
    offset += dispersalData.outlet(serialNum, data + offset);
    offset += dispersalData.outlet(cmdCode, data + offset);
    offset += dispersalData.outlet(versionNum, data + offset);
    offset += dispersalData.outlet(userID, data + offset);
    offset += dispersalData.outlet(destID, data + offset);
    offset += dispersalData.outlet(dataFieldLen, data + offset);

    if(dataField != NULL){
        SAFE_DELETE_ARRAY(dataField);
    }

    if(dataFieldLen != 0){
        dataField = new char[dataFieldLen];
        memset(dataField, 0, dataFieldLen);
    }
    offset += dispersalData.outlet(dataField, data + offset, dataFieldLen);
    dispersalData.outlet(postfix, data + offset);
}
void InputPacket::decryptDataField()
{
    if(isDecryptByNull(cmdCode)){//无须解密的数据域
        this->adjustDecryptType(NULL);
    }else if(isDecryptByLoginkey(cmdCode)){//登录秘钥解密数据域
        int ret = adjustDecryptType(Keys::getLoginKey());
        if(ret < 0) answerCode = Mi::Error;//密码错误
    }else if(isDecryptByVerifykey(cmdCode)){//验证码MD5解密数据域
        this->adjustDecryptType(Keys::getVerifyCodeKey());
    }else//会话秘钥解密数据域
        this->adjustDecryptType(Keys::getSessionKey());
}

int InputPacket::adjustDecryptType(char *key)
{
    char decryptDataBuf[ConstNum::MAX_PACKET_SIZE] = { 0 };
    char* chDataField = NULL;
    int decryptDataLen = -1;

    if(key != NULL && dataField != NULL && dataFieldLen != 0){//数据域解密
        decryptDataLen = ToolKit::DecryptData((quint8*)dataField, dataFieldLen, (quint8*)key, (quint8*)decryptDataBuf);
        chDataField = decryptDataBuf;
    }else if(key == NULL && dataField != NULL && dataFieldLen != 0){//数据域不解密
        chDataField = dataField;
        decryptDataLen = dataFieldLen;
    }

    if(decryptDataLen < 0) return -1;//数据域长度小于0，数据有误
    answerCode = chDataField[0];//获取应答码
    char* data = chDataField + sizeof(answerCode);

    decryptDataLen = decryptDataLen - sizeof(answerCode);
    plaintextArray.append(data, decryptDataLen);
    SAFE_DELETE_ARRAY(dataField);
    return decryptDataLen;
}

bool InputPacket::isDecryptByLoginkey(quint16 cmdCode)
{
    switch(cmdCode){
        case CommandCode::LOGIN_SECOND_STEP:
        return true;
    }
    return false;
}

bool InputPacket::isDecryptByVerifykey(quint16 cmdCode)
{
    switch(cmdCode){
        case CommandCode::FORGET_PASSWORD_RESET:
        return true;
    }
    return false;
}

bool InputPacket::isDecryptByNull(quint16 cmdCode)
{
    switch(cmdCode){
        case CommandCode::LOGIN_FIRST_STEP:
        case CommandCode::FORGET_PASSWORD_APPLY:
        case CommandCode::FORGET_PASSWORD_VERIFY:
        case CommandCode::REGISTER_FIRST_STEP:
        case CommandCode::REGISTER_SECOND_STEP:
        case CommandCode::REGISTER_THIRD_STEP:
        return true;
    }
    return false;
}

char* InputPacket::getPackData()
{
    if(plaintextArray.length() >= 1)
        return plaintextArray.data();
    else
        return NULL;
}

void InputPacket::setPackData(char *data, quint16 dataLen)
{
    plaintextArray.clear();
    plaintextArray.append(data, dataLen);
}

quint16 InputPacket::getPackDataLen()
{
    return plaintextArray.length();
}

InputPacket::~InputPacket()
{

}
