#include "BasePacket.h"

BasePacket::BasePacket()
{
    prefix  = 0;
    serialNum = 0;
    versionNum = 0;
    cmdCode = 0;
    userID = 0;
    destID = 0;
    dataFieldLen = 0;
    answerCode = 0;
    dataField = NULL;
    postfix = 0;
}

BasePacket::~BasePacket(){}
