#ifndef DCLSUTERCREATEPAGE_H
#define DCLSUTERCREATEPAGE_H

class QPushButton;
class QLineEdit;
class QTextEdit;
class InputPacket;
class HintMsgPage;
class LoadingPage;
class NavigationBar;
class DClusterCreateSuccessedPage;

#include "BasePage.h"

class DClusterCreatePage : public BasePage
{
    Q_OBJECT
public:
    explicit DClusterCreatePage(QWidget* parent = 0);
    virtual ~DClusterCreatePage();

public:
    void fetchData(InputPacket& input);

private:
    quint64 curClusterID;
    NavigationBar* navigationBar;
    QPushButton* btnSubmitPage;
    QLineEdit* editClusterName;
    QTextEdit* editDescribe;

    LoadingPage* loadingPage;
    HintMsgPage* hintMsgPage;
    DClusterCreateSuccessedPage* clusterCreateSuccessedPage;

private:
    void initializeWidget();
    void handleNetError();
    void handleHintInfo(QString& strText);
    void handleCreateDevCluster(InputPacket& inpack);
    void displayConcretePage(BasePage* page);
    void transferDataToAnotherPage(InputPacket& inpack, BasePage* basePage);

private slots:
    void on_btnPrePage_clicked();
    void on_btnSubmitPage_clicked();
    void on_hintMsgPage(bool isJump);
    void on_deleteSubPages();
    void on_recvDataFromOtherPage(quint16 cmdCode, char* data, quint16 dataLen);
};

#endif // DCLSUTERCREATEPAGE_H
