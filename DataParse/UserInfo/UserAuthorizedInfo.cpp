#include "DispersalData.h"
#include "UserAuthorizedInfo.h"

UserAuthorizedInfo::UserAuthorizedInfo(QObject* parent) : QObject(parent){}

quint16 UserAuthorizedInfo::unpack(char *data)
{
    if(data == NULL) return 0;

    quint16 offset = 0;
    quint8 role = 0;
    DispersalData dispersalData;

    offset += dispersalData.outlet(m_userID ,data + offset);//用户ID
    offset += dispersalData.outlet(role ,data + offset);    //用户权限
    m_userRole = AuthContent::numToAuthlevel(role);
    offset += dispersalData.outlet(m_userName ,data + offset);//用户名称
    return offset;
}

UserAuthorizedInfo::~UserAuthorizedInfo()
{

}


