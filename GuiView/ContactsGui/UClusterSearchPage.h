#ifndef UCLUSTERSEARCHPAGE_H
#define UCLUSTERSEARCHPAGE_H

class InputPacket;
class SearchBar;
class UClusterSearchResultPage;
class NavigationBar;
class HintMsgPage;
class LoadingPage;

#include "BasePage.h"

class UClusterSearchPage : public BasePage
{
    Q_OBJECT
public:
    explicit UClusterSearchPage(QWidget* parent = 0);
    virtual ~UClusterSearchPage();

public:
    void fetchData(InputPacket& inpack);

private:
    NavigationBar* navigationBar;
    SearchBar* searchBar;
    LoadingPage* loadingPage;
    HintMsgPage* hintMsgpage;
    UClusterSearchResultPage* clusterSearchResultPage;

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


#endif // UCLUSTERSEARCHPAGE_H
