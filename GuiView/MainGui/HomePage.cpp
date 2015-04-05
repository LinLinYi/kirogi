#include <QDebug>
#include <QPixmap>
#include <QColor>
#include <QPainter>
#include <GroupButton.h>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>

#include "InputPacket.h"
#include "AddProjectsPage.h"
#include "ListWidget.h"
#include "HomePage.h"
#include "UserProtocol.hpp"
#include "ToolKit.h"
#include "ImageConfig.hpp"
#include "NavigationBar.h"
#include "SheetStyleConfig.hpp"

HomePage::HomePage(QWidget *parent, PageType type)
    : BasePage(parent, type)
    , curFixedContentHeight(0)
{
    this->initializeWidget();
}

void HomePage::initializeWidget()
{
    //导航栏
    QString strTitle = tr("首 页");
    QPixmap pixRightBtn = QPixmap(ImagePath::ADDTION);
    navigationBar = new NavigationBar(this);
    navigationBar->setTitleText(strTitle);
    navigationBar->setRightPixMap(pixRightBtn);

    QPixmap leftSearchPixmap(ImagePath::WHITE_SEARCH_BAR);
    leftSearchPixmap.setDevicePixelRatio(2);
    navigationBar->setLeftPixMap(leftSearchPixmap);
    connect(navigationBar, SIGNAL(rightClicked()), this, SLOT(on_btnAddProject_clicked()));

    QHBoxLayout* pHLTop = new QHBoxLayout;
    pHLTop->addWidget(navigationBar);
    pHLTop->setSpacing(0);
    pHLTop->setMargin(0);
    this->setTopbarLayout(pHLTop);

    //图片显示栏
    QPixmap homePixmap(ImagePath::THEME);
    homePixmap.setDevicePixelRatio(2);
    lblHomeGraphics = new QLabel(this);
    lblHomeGraphics->setStyleSheet(SheetStyle::HOMEPHOTO_LABEL);
    lblHomeGraphics->setPixmap(homePixmap);
    lblHomeGraphics->setScaledContents(true);
    lblHomeGraphics->setFixedHeight(this->screenHeight()*0.2);
    lblHomeGraphics->setFixedWidth(this->screenWidth());
    lblHomeGraphics->setMargin(0);
    curFixedContentHeight += lblHomeGraphics->height();

    //项目组标签显示栏
    lblProjectHint = new QLabel(tr("场景"));
    lblProjectHint->setFixedHeight(this->screenHeight()*0.05);
    lblProjectHint->setFixedWidth(this->screenWidth());
    lblProjectHint->setStyleSheet(SheetStyle::HOMEPROJECT_LABLE);
    lblProjectHint->setContentsMargins(this->screenWidth()*0.03,0,0,0);
    curFixedContentHeight += lblProjectHint->height();

    sceneListWidget = new ListWidget(this);
    this->setListWidget(sceneListWidget);
    this->test();

    QVBoxLayout* vblTotalLayout = new QVBoxLayout();
    vblTotalLayout->addWidget(lblHomeGraphics);
    vblTotalLayout->addWidget(lblProjectHint);
    vblTotalLayout->addWidget(sceneListWidget);
    vblTotalLayout->setMargin(0);
    vblTotalLayout->setSpacing(0);
    vblTotalLayout->setAlignment(Qt::AlignTop);
    this->setBodyPartLayout(vblTotalLayout);

    //屏幕触摸滚动设置
    this->setBodyScreenHeight(this->scrollAreaHasBottomBarHeight());
    this->installScrollViewportArea();
}

void HomePage::displayConcretePage(BasePage *page)
{
    if(page == NULL) return;
    page->show();
    connect(page, SIGNAL(destroyed()), this, SLOT(on_deleteSubPages()));
    connect(page, SIGNAL(readyRead(quint16,char*,quint16)), this, SLOT(on_recvDataFromOtherPage(quint16,char*,quint16)));
}

void HomePage::setGroupButton(GroupButton* btn)
{
    //设备按钮设置
    if(btn == NULL) return;
    QPixmap pixmap(ImagePath::SCENE);
    pixmap.setDevicePixelRatio(2.0);
    btn->setPixMap(pixmap);
    btn->setText(tr("场景1"));
    btn->setFixedHeight(this->screenHeight()*0.1);
    btn->setFixedWidth(this->screenWidth());
    btn->setAttribute(Qt::WA_LayoutUsesWidgetRect);
    btn->setStyleSheet(SheetStyle::GROUPBUTTON_BOTTOMBORDER);
    btn->setContentsMargins(0,0,this->screenWidth()*0.05,0);
}

void HomePage::setListWidget(ListWidget *listWidget)
{
    listWidget->setStyleSheet(SheetStyle::LISTWIDGET);
    listWidget->setContentsMargins(this->screenWidth()*0.03,0,0,0);
}

void HomePage::test()
{
    for(int i = 0; i < 2; i++)
    {
        GroupButton* btn = new GroupButton(this);
        this->setGroupButton(btn);
        sceneListWidget->addWidget(btn);

        curFixedContentHeight += btn->height();
        if(curFixedContentHeight <= this->bodyPartHeight())
            this->setBodyRealHeight(this->bodyPartHeight());
        else
            this->setBodyRealHeight(curFixedContentHeight);
    }
}

void HomePage::fetchData(InputPacket&)
{

}

void HomePage::on_btnAddProject_clicked()
{
    emit readyRead(CommandCode::BOTTOM_BAR_UNDISPLAY, NULL, 0);//隐藏底部任务栏
    addProjectsPage = new AddProjectsPage(this);
    this->displayConcretePage(addProjectsPage);
}

void HomePage::on_btnSearch_clicked()
{

}

void HomePage::on_deleteSubPages()
{
    addProjectsPage = NULL;
    emit readyRead(CommandCode::BOTTOM_BAR_DISPLAY, NULL, 0);//显示底部任务栏
}

void HomePage::on_recvDataFromOtherPage(quint16 cmdCode, char *packData, quint16 dataLen)
{
    emit readyRead(cmdCode, packData, dataLen);
}

HomePage::~HomePage()
{

}
