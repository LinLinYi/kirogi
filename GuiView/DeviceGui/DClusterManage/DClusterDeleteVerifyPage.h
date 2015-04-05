#ifndef DCLUSTERDELETEVERIFYPAGE_H
#define DCLUSTERDELETEVERIFYPAGE_H

class QPushButton;
class QLabel;
class NavigationBar;
class LoadingPage;
class HintMsgPage;

#include "BasePage.h"

class DClusterDeleteVerifyPage : public BasePage
{
    Q_OBJECT
public:
    explicit DClusterDeleteVerifyPage(QWidget* parent = 0, quint64 clusID = 0);
    virtual ~DClusterDeleteVerifyPage();

public:
    void fetchData(InputPacket& inpack);

private:
    quint64 clusterID;
    QLabel* lblMsgHint;
    QLabel* lblAuthMail;
    QLineEdit* editAuthCode;
    QPushButton* btnSubmit;
    NavigationBar* navigationBar;

    LoadingPage* loadingPage;
    HintMsgPage* hintMsgPage;

private:
    void initializeWidget();
    void handleHintInfo(QString& text);
    void handleNetError();
    void handleDelDevClusterVerify(InputPacket& inpack);

private slots:
    void on_btnPrePage_clicked();
    void on_btnSubmit_clicked();
    void on_hintMsgPage(bool isJump);
};

#endif // DCLUSTERDELETEVERIFYPAGE_H
