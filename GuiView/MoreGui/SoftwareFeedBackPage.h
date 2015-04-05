#ifndef SOFTWAREFEEDBACKPAGE_H
#define SOFTWAREFEEDBACKPAGE_H

class HintMsgPage;
class LoadingPage;
class QTextEdit;
class NavigationBar;
#include "BasePage.h"

class SoftwareFeedBackPage : public BasePage
{
    Q_OBJECT
public:
    explicit SoftwareFeedBackPage(QWidget* parent = 0, quint64 accountID = 0);
    virtual ~SoftwareFeedBackPage();

public:
    void fetchData(InputPacket &inpack);

private:
    quint64 curAccountID;
    HintMsgPage* hintMsgPage;
    LoadingPage* loadingPage;
    NavigationBar* navigationBar;
    QTextEdit* editFeedback;

private:
    void initializeWidget();
    void handleNetError();
    void handleFeedbackInfo(InputPacket& inpack);
    void handleHintInfo(QString& strText);

private slots:
    void on_btnPrePage_clicked();
    void on_btnSubmitPage_clicked();
    void on_hintMsgPage(bool isJump);
};

#endif // SOFTWAREFEEDBACKPAGE_H
