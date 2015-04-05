#ifndef DCLUSTERCLAIMDEVICEPWDPAGE_H
#define DCLUSTERCLAIMDEVICEPWDPAGE_H

class HintMsgPage;
class QPushButton;
class LoadingPage;
class NavigationBar;

#include "BasePage.h"

class DClusterClaimDevicePwdPage : public BasePage
{
    Q_OBJECT
public:
    explicit DClusterClaimDevicePwdPage(QWidget* parent = 0, quint64 clusID = 0, quint64 devID = 0);
    virtual ~DClusterClaimDevicePwdPage();

public:
    void fetchData(InputPacket& inpack);

private:
    quint64 curClusterID;
    quint64 curDeviceID;

    NavigationBar* navigationBar;
    QLineEdit* editClaimPwd;
    QPushButton* btnSubmit;
    LoadingPage* loadingPage;
    HintMsgPage* hintMsgPage;

private:
    void initializeWidget();
    void handleHintInfo(QString& strText);
    void handleNetError();
    void handleClaimDevicePwd(InputPacket& inpack);

private slots:
    void on_btnPrePage_clicked();
    void on_btnSubmit_clicked();
    void on_hintMsgPage(bool);
};

#endif // DCLUSTERCLAIMDEVICEPWDPAGE_H
