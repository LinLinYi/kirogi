#include <QVBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QToolButton>
#include <QDebug>

#include "ListWidget.h"
#include "AddProjectsPage.h"
#include "UserProtocol.hpp"
#include "InputPacket.h"
#include "BasePage.h"
#include "GroupButton.h"
#include "ImageConfig.hpp"
#include "NavigationBar.h"
#include "SheetStyleConfig.hpp"

AddProjectsPage::AddProjectsPage(QWidget* parent) : BasePage(parent)
{
    this->initializeWidget();
}

void AddProjectsPage::initializeWidget()
{
    //导航栏
    QString strTitle = tr("添加场景");
    QString strLeftBtn = tr("取消");
    QPixmap leftArrowsPixmap(ImagePath::NAVIGATION_LEFT_ARROWS);
    navigationBar = new NavigationBar(this);
    navigationBar->setTitleText(strTitle);
    navigationBar->setLeftText(strLeftBtn);
    navigationBar->setLeftPixMap(leftArrowsPixmap);
    connect(navigationBar, SIGNAL(leftClicked()), this, SLOT(on_btnPrePage_clicked()));

    QHBoxLayout* pHLTop = new QHBoxLayout();
    pHLTop->addWidget(navigationBar);
    pHLTop->setSpacing(0);
    pHLTop->setMargin(0);
    this->setTopbarLayout(pHLTop);

    //扫一扫
    QPixmap sweepPixmap(ImagePath::SWEEP);
    QIcon icon = QIcon(sweepPixmap);
    btnSweepProject = new QToolButton(this);
    btnSweepProject->setIcon(icon);
    btnSweepProject->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    btnSweepProject->setStyleSheet("background-color: #ececec; color:#7b7b7b; font:16px;");
    btnSweepProject->setFixedWidth(this->screenWidth());
    btnSweepProject->setText(QString(tr("二维码扫描")));
    btnSweepProject->setFixedHeight(this->screenHeight()*0.18);
    btnSweepProject->setIconSize(QSize(btnSweepProject->height()*0.8, btnSweepProject->height()*0.8));
    connect(btnSweepProject, SIGNAL(clicked()), SLOT(on_btnSweepProject_clicked()));

    //添加场景
    QPixmap addProjectPixmap(ImagePath::SEARCH_SCENE);
    addProjectPixmap.setDevicePixelRatio(2.0);
    btnAddProject = new GroupButton(this);
    btnAddProject->setStyleSheet(SheetStyle::GROUPBUTTON_BOTTOMBORDER);
    this->setGroupButton(btnAddProject, addProjectPixmap, QString("添加场景"));
    connect(btnAddProject, SIGNAL(clicked()), SLOT(on_btnAddProject_clicked()));

    //创建场景
    QPixmap createProjectPixmap(ImagePath::CREATE_SCENE);
    createProjectPixmap.setDevicePixelRatio(2.0);
    btnCreateProject = new GroupButton(this);
    btnCreateProject->setStyleSheet(SheetStyle::GROUPBUTTON_NOBORDER);
    setGroupButton(btnCreateProject, createProjectPixmap, QString("创建场景"));
    connect(btnCreateProject, SIGNAL(clicked()), SLOT(on_btnCreateProject_clicked()));

    ListWidget*sceneOper = new ListWidget(this);
    sceneOper->setStyleSheet(SheetStyle::LISTWIDGET);
    sceneOper->addWidget(btnAddProject);
    sceneOper->addWidget(btnCreateProject);
    sceneOper->setFixedWidth(this->screenWidth());
    sceneOper->setContentsMargins(this->screenWidth()*0.05,0,0,0);

    QVBoxLayout* vblTotalLayout = new QVBoxLayout();
    vblTotalLayout->addWidget(btnSweepProject);
    vblTotalLayout->addWidget(sceneOper);
    vblTotalLayout->setAlignment(Qt::AlignTop);
    vblTotalLayout->setContentsMargins(0,0,0,0);
    this->setBodyPartLayout(vblTotalLayout);

    //屏幕触摸滚动设置
    this->installScrollViewportArea();
}

void AddProjectsPage::setGroupButton(GroupButton* groupButton, QPixmap photo, QString title)
{
    groupButton->setPixMap(photo);
    groupButton->setText(title);

    QPixmap pixmap(ImagePath::RIGHT_HOLLOW_ARROWS);
    groupButton->setReserve(pixmap);
    groupButton->setFixedHeight(this->screenHeight()*0.0775);
    groupButton->setAttribute(Qt::WA_LayoutUsesWidgetRect);
    groupButton->setContentsMargins(0, 0, this->screenWidth()*0.03, 0);
}

void AddProjectsPage::on_btnPrePage_clicked()
{
    this->hide();
    this->deleteLater();
}

void AddProjectsPage::on_btnSweepProject_clicked()
{

}

void AddProjectsPage::on_btnCreateProject_clicked()
{

}

void AddProjectsPage::on_btnAddProject_clicked()
{

}

void AddProjectsPage::fetchData(InputPacket&)
{

}

AddProjectsPage::~AddProjectsPage()
{

}
