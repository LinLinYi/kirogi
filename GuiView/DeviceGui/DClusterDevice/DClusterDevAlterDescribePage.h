#ifndef DCLUSTERDEVALTERDESCRIBEPAGE_H
#define DCLUSTERDEVALTERDESCRIBEPAGE_H

class QTextEdit;
class HintMsgPage;
class LoadingPage;
class NavigationBar;

#include "BasePage.h"
#include "DClusterDevInfo.h"

class DClusterDevAlterDescribePage : public BasePage
{
    Q_OBJECT
public:
    explicit DClusterDevAlterDescribePage(QWidget* parent, quint64 clusID, quint64 devID);
    virtual ~DClusterDevAlterDescribePage();

public:
    void fetchData(InputPacket &inpack);

private:
    quint64 curClusterID;
    quint64 curDeviceID;
    DClusterDevInfo curDeviceInfo;
    HintMsgPage* hintMsgPage;
    LoadingPage* loadingPage;
    NavigationBar* navigationBar;
    QTextEdit* editDescribe;

private:
    void initializeWidget();
    void handleNetError();
    void loadLocalData();
    void handleDescribeInfo(InputPacket& inpack);
    void handleHintInfo(QString& strText);

private slots:
    void on_btnPrePage_clicked();
    void on_btnSubmitPage_clicked();
    void on_hintMsgPage(bool isJump);
};

#endif // DCLUSTERDEVALTERDESCRIBEPAGE_H
