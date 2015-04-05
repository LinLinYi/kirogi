#ifndef UCLUSTERINFOPAGE_H
#define UCLUSTERINFOPAGE_H

class InputPacket;
#include "BasePage.h"

class UClusterInfoPage : public BasePage
{
public:
    explicit UClusterInfoPage(QWidget* parent = 0);
    virtual ~UClusterInfoPage();

public:
    void fetchData(InputPacket& inputpack);

private:
    void initializeWidget();
};

#endif // UCLUSTERINFOPAGE_H
