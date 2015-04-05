#include "InitManagment.h"
#include "ToolKit.h"
#include "GlobalDefine.hpp"
#include "UserProtocol.hpp"
#include "InputPacket.h"
#include "UserLoginPage.h"
#include "DistributeDataStation.h"

#define START_TIMER_INTERVAL 2000

InitManagment::InitManagment(QObject *parent)
    : QObject(parent)
    , userLoginPage(NULL)
    , distributeDataStation(NULL)
{
    this->init();
}

void InitManagment::init()
{
    this->entryLoginPage();
}

void InitManagment::entryLoginPage()
{
    //初始化视图与数据处理
    if(userLoginPage != NULL || distributeDataStation != NULL) return;
    userLoginPage = new UserLoginPage;
    userLoginPage->show();

    distributeDataStation = new DistributeDataStation(this);
    connect(userLoginPage, SIGNAL(readyRead(quint16,char*,quint16)), this, SLOT(on_loginPage(quint16,char*,quint16)));
    connect(distributeDataStation, SIGNAL(readyRead(InputPacket&)), this, SLOT(on_distributeDataStation(InputPacket&)));
}

void InitManagment::on_distributeDataStation(InputPacket& inpack)
{
    DPRINT("-->InitManagment receive data, the cmdCode is:0x%x", inpack.getCmdCode());
    this->transferDataToAnotherPage(inpack, userLoginPage);
}

void InitManagment::transferDataToAnotherPage(InputPacket &inpack, BasePage *basePage)
{
    if(basePage == NULL) return;
    basePage->fetchData(inpack);
}

void InitManagment::on_loginPage(quint16 cmdCode, char *msgData, quint16 dataLen)
{
    if(distributeDataStation == NULL) return;
     distributeDataStation->fetchData(cmdCode, msgData, dataLen);
}

InitManagment::~InitManagment()
{

}

