#ifndef REGACCOUNTDEPLOYPAGE_H
#define REGACCOUNTDEPLOYPAGE_H

class NavigationBar;
class QPushButton;
class LoadingPage;
class HintMsgPage;
class QValidator;

#include "BasePage.h"

class RegAccountDeployPage : public BasePage
{
    Q_OBJECT
public:
    explicit RegAccountDeployPage(QWidget* parent = 0);
    virtual ~RegAccountDeployPage();

public:
    void fetchData(InputPacket& inpack);

private:
    NavigationBar* navigationBar;
    QLineEdit* editAccount;
    QLineEdit* editPassword;
    QPushButton* btnSubmit;
    QValidator* validator;

    LoadingPage* loadingPage;
    HintMsgPage* hintMsgPage;
    quint16 packBufLen;

private:
    void initializeWidget();
    void handleNetError();
    void handleHintInfo(QString& strText);
    void handleRegisterThirdStep(InputPacket& inpack);
    void setInputMode(QLineEdit* line);

private slots:
    void on_btnPrePage_clicked();
    void on_btnSubmit_clicked();
    void on_hintMsgPage(bool isJump);
};

#endif // REGACCOUNTDEPLOYPAGE_H
