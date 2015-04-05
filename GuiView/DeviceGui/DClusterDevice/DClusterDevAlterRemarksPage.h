#ifndef DCLUSTERDEVALTERREMARKSPAGE_H
#define DCLUSTERDEVALTERREMARKSPAGE_H

class QLineEdit;
class HintMsgPage;
class LoadingPage;
class NavigationBar;

#include "BasePage.h"
#include "DClusterDevInfo.h"

class DClusterDevAlterRemarksPage : public BasePage
{
    Q_OBJECT
public:
    explicit DClusterDevAlterRemarksPage(QWidget* parent, quint64 clusID, quint64 devID);
    virtual ~DClusterDevAlterRemarksPage();

public:
    void fetchData(InputPacket &inpack);

private:
    quint64 curClusterID;
    quint64 curDeviceID;
    DClusterDevInfo curDeviceInfo;
    HintMsgPage* hintMsgPage;
    LoadingPage* loadingPage;
    NavigationBar* navigationBar;
    QLineEdit* editRemarks;

private:
    void initializeWidget();
    void handleNetError();
    void loadLocalData();
    void handleRemarksInfo(InputPacket& inpack);
    void handleHintInfo(QString& strText);

private slots:
    void on_btnPrePage_clicked();
    void on_btnSubmitPage_clicked();
    void on_hintMsgPage(bool isJump);
};

#endif // DCLUSTERDEVALTERREMARKSPAGE_H
