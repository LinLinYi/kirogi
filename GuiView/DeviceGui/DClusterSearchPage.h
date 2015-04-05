#ifndef DCLUSTERSEARCHPAGE_H
#define DCLUSTERSEARCHPAGE_H

class InputPacket;
class SearchBar;
class NavigationBar;
class LoadingPage;
class HintMsgPage;
class DClusterSearchResultPage;

#include "BasePage.h"

class DClusterSearchPage : public BasePage
{
    Q_OBJECT
public:
    explicit DClusterSearchPage(QWidget* parent = 0);
    virtual ~DClusterSearchPage();

public:
    void fetchData(InputPacket& inpack);

private:
    NavigationBar* navigationBar;
    SearchBar* searchBar;
    LoadingPage* loadingPage;
    HintMsgPage* hintMsgpage;
    DClusterSearchResultPage* clusterSearchResultPage;

private:
    void initializeWidget();
    void handleHintInfo(QString& text);
    void handleNetError();
    void handleSearchDevCluster(InputPacket& inpack);
    void displayConcretePage(BasePage* page);
    void transferDataToAnotherPage(InputPacket& inpack, BasePage* basePage);

private slots:
    void on_btnPrePage_clicked();
    void on_searchBar(QString& text);
    void on_hintMsgpage(bool isJump);
    void on_deleteSubPages();
    void on_recvDataFromOtherPage(quint16 cmdCode, char* data, quint16 dataLen);
};

#endif // DCLUSTERSEARCHPAGE_H
