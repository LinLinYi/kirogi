#ifndef SOFTWAREABOUTPAGE_H
#define SOFTWAREABOUTPAGE_H

class QLabel;
class GroupButton;
class NavigationBar;

#include "BasePage.h"

class SoftwareAboutPage : public BasePage
{
    Q_OBJECT
public:
    explicit SoftwareAboutPage(QWidget* parent = 0, quint64 accountID = 0);
    virtual ~SoftwareAboutPage();

public:
    void fetchData(InputPacket& inpack);

private:
    quint64 curAccountID;
    QLabel* lblSoftwareIcon;
    QLabel* lblSoftwareVersion;
    QLabel* lblCompanyName;
    QLabel* lblCopyRightDeclare;
    NavigationBar* navigationBar;
    GroupButton* btnFunctionIntroduce;

private:
    void initializeWidget();
    void setGroupButton(GroupButton* btn, const QString& title, const quint16 height = 0);

private slots:
    void on_btnPrePage_clicked();
    void on_btnFunIntroduce_clicked();
};

#endif // SOFTWAREABOUTPAGE_H
