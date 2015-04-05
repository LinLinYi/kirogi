#ifndef DCLUSTERALTERREMARKSPAGE_H
#define DCLUSTERALTERREMARKSPAGE_H

class QLineEdit;
class HintMsgPage;
class LoadingPage;
class NavigationBar;

#include "BasePage.h"
#include "DClusterInfo.h"

class DClusterAlterRemarksPage : public BasePage
{
    Q_OBJECT
public:
    explicit DClusterAlterRemarksPage(QWidget* parent = 0, quint64 clusID = 0);
    virtual ~DClusterAlterRemarksPage();

public:
    void fetchData(InputPacket &inpack);

private:
    quint64 curClusterID;
    DClusterInfo curClusterInfo;
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

#endif // DCLUSTERALTERREMARKSPAGE_H
