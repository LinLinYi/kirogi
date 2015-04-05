#ifndef USERSELECTHEADPORTRAITPAGE_H
#define USERSELECTHEADPORTRAITPAGE_H

class ListWidget;
class GroupButton;
class NavigationBar;

#include "BasePage.h"

class UserAlterHeadPortraitPage : public BasePage
{
    Q_OBJECT
public:
    UserAlterHeadPortraitPage(QWidget* parent = 0, quint64 accountID = 0);
    ~UserAlterHeadPortraitPage();

public:
    void fetchData(InputPacket& inpack);

private:
    quint64 curAccountID;
    QStringList imgPathList;
    ListWidget* imgListWidget;
    NavigationBar* navigationBar;
    quint32 curFixedContentHeight;
    GroupButton* curSelectedButton;

private:
    void initializeWidget();
    void loadLocalImage();
    void setImageButton(GroupButton* btn, const QString& userPortrait);

private slots:
    void on_btnPrePage_clicked();
    void on_btnSelectImage_clicked();
    void on_btnSubmitImage_clicked();
};

#endif // USERSELECTHEADPORTRAITPAGE_H
