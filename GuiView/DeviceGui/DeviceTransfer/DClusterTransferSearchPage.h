#ifndef DCLUSTERTRANSFERSEARCHPAGE_H
#define DCLUSTERTRANSFERSEARCHPAGE_H

class SearchBar;
class LoadingPage;
class HintMsgPage;
class NavigationBar;
class DClusterTransferSearchResultPage;

#include"BasePage.h"

class DClusterTransferSearchPage : public BasePage
{
    Q_OBJECT
public:
    explicit DClusterTransferSearchPage(QWidget* parent = 0, quint64 clusterID = 0);
    virtual ~DClusterTransferSearchPage();

public:
    void fetchData(InputPacket& inputpack);
    void setCurClusterName(QString& name);

private:
    quint64 curClusterID;               /*! 设备群ID*/
    QString curClusterName;             /*! 设备群名称*/
    SearchBar* searchBar;               /*! 查找设备群*/
    LoadingPage* loadingPage;           /*! 加载页面*/
    HintMsgPage* hintMsgpage;           /*! 信息提示页面*/
    NavigationBar* navigationBar;       /*! 导航栏*/
    DClusterTransferSearchResultPage* clusterTransferSearchResultPage;

private:
    void initializeWidget();
    void handleNetError();
    void handleHintInfo(QString& text);
    void handleSearchClusterResult(InputPacket& inpack);
    void displayConcretePage(BasePage* page);
    void transferDataToAnotherPage(InputPacket& inpack, BasePage* basePage);

private slots:
    void on_btnPrePage_clicked();
    void on_searchBar(QString& text);
    void on_hintMsgpage(bool isJump);
    void on_deleteSubPages();
    void on_recvDataFromOtherPage(quint16 cmdCode, char* data, quint16 dataLen);
};

#endif // DCLUSTERTRANSFERSEARCHPAGE_H
