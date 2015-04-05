#ifndef BASEPACKET_H
#define BASEPACKET_H

#include <QObject>

class BasePacket
{
public:
    explicit BasePacket();
    virtual ~BasePacket();

public:
    virtual char* getPackData() { return dataField; }
    virtual quint16 getPackDataLen() { return dataFieldLen; }
    virtual quint16 getSerialNum() { return serialNum; }
    virtual quint16 getVersionNum() { return versionNum; }
    virtual quint16 getCmdCode() { return cmdCode; }
    virtual quint64 getUserID() { return userID; }
    virtual quint64 getDestID() { return destID; }
    virtual quint8 getAnswerCode() { return answerCode; }

    virtual void setSerialNum(quint16 serial) { serialNum = serial; }
    virtual void setVersion(quint16 ver) { versionNum = ver; }
    virtual void setCmdCode(quint16 cmd) { cmdCode = cmd; }
    virtual void setUserID(quint64 id) { userID = id; }
    virtual void setDestID(quint64 id) { destID = id; }
    virtual void setAnswerCode(quint8 ansCode) { answerCode = ansCode; }
    virtual void setPackData(char*, quint16) {}

protected:
    quint8 prefix;       /*! 前缀*/
    quint16 serialNum;   /*! 包序列号*/
    quint16 versionNum;  /*! 软件版本号*/
    quint16 cmdCode;     /*! 命令字*/
    quint64 userID;      /*! 用户ID*/
    quint64 destID;      /*! 目标ID*/
    quint16 dataFieldLen;/*! 数据长度*/
    quint8  answerCode;  /*! 应答码*/
    char* dataField;     /*! 数据域*/
    quint8 postfix;      /*! 后缀*/
};

#endif // BASEPACKET_H
