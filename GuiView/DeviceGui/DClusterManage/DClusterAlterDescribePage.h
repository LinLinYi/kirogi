#ifndef DCLUSTERALTERDESCRIBEPAGE
#define DCLUSTERALTERDESCRIBEPAGE

class QTextEdit;
class HintMsgPage;
class LoadingPage;
class NavigationBar;

#include "BasePage.h"
#include "DClusterInfo.h"

class DClusterAlterDescribePage : public BasePage
{
    Q_OBJECT
public:
    explicit DClusterAlterDescribePage(QWidget* parent = 0, quint64 clusID = 0);
    virtual ~DClusterAlterDescribePage();

public:
    void fetchData(InputPacket &inpack);

private:
    quint64 curClusterID;
    DClusterInfo curClusterInfo;
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

#endif // DCLUSTERALTERDESCRIBEPAGE
