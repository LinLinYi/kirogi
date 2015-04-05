#ifndef INITMANAGMENT_H
#define INITMANAGMENT_H

class InputPacket;
class UserLoginPage;
class BasePage;
class DistributeDataStation;

#include <QObject>

class InitManagment : public QObject
{
    Q_OBJECT
public:
    explicit InitManagment(QObject *parent = 0);
    ~InitManagment();

public:
    UserLoginPage* userLoginPage;

private:
    int timerID;
    DistributeDataStation* distributeDataStation;

private:
    void init();
    void entryLoginPage();
    void transferDataToAnotherPage(InputPacket& inpack, BasePage* basePage);

private slots:
    void on_distributeDataStation(InputPacket& inputData);
    void on_loginPage(quint16 cmdCode, char* msgData, quint16 dataLen);
};

#endif // INITMANAGMENT_H
