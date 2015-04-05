#include <QVBoxLayout>
#include <QPixmap>

#include "GListWidget.h"
#include "GroupButton.h"
#include "ListWidget.h"
#include "SheetStyleConfig.hpp"

GListWidget::GListWidget(QWidget* parent): QWidget(parent)
{
    initializeWidget();
}

void GListWidget::initializeWidget()
{
    //标题按钮部分
    titleButton = new GroupButton(this);
    titleButton->setFixedHeight(45);
    titleButton->setAttribute(Qt::WA_LayoutUsesWidgetRect);
    connect(titleButton, SIGNAL(clicked()), this, SLOT(on_titleButton_clicked()));

    //列表条目部分
    listWidget = new ListWidget(this);
    listWidget->setStyleSheet(SheetStyle::LISTWIDGET);
    listWidget->setAttribute(Qt::WA_LayoutUsesWidgetRect);
    this->setListVisible(false);

    //整体布局部分
    vblTotalLayout = new QVBoxLayout;
    vblTotalLayout->addWidget(titleButton);
    vblTotalLayout->addWidget(listWidget);
    vblTotalLayout->setSpacing(0);
    vblTotalLayout->setMargin(0);
    this->setLayout(vblTotalLayout);
}

void GListWidget::setListVisible(bool visible)
{
    if(visible)
        listWidget->show();
    else
        listWidget->hide();

    visibleStatus = visible;
}

void GListWidget::setContentsMargins(int left, int top, int right, int bottom)
{
    listWidget->setContentsMargins(left, top, right, bottom);
}

void GListWidget::setPixmap(const QPixmap &pixMap)
{
    QPixmap px = pixMap;
    px.setDevicePixelRatio(2);
    titleButton->setPixMap(px);
}

void GListWidget::setTitle(const QString &title)
{
    titleButton->setText(title);
}

void GListWidget::setTitleStyleSheet(const QString &objName)
{
    titleButton->setStyleSheet(objName);
}

void GListWidget::setTitleHeight(int height)
{
    titleButton->setFixedHeight(height);
}

int GListWidget::titleHeight()
{
    return titleButton->height();
}

int GListWidget::listWidth()
{
    return (this->width());
}

int GListWidget::listHeight()
{
    if(isListVisible())
        return listWidget->height();
    else
        return 0;
}

void GListWidget::addWidget(QWidget *widget)
{
    if(widget == NULL) return;
    listWidget->addWidget(widget);
    this->changeListHeight();
}

void GListWidget::insertWidget(int index, QWidget *widget)
{
    if(widget == NULL) return;
    listWidget->insertWidget(index, widget);
    this->changeListHeight();
}

QWidget* GListWidget::removeWidget(int index)
{
    QWidget* widget = NULL;
    if(index >= listWidget->count()) return NULL;

    widget = listWidget->itemAt(index);
    if(widget != NULL){
        listWidget->removeWidget(widget);
    }

    this->changeListHeight();
    return widget;
}

void GListWidget::takeAt(int index)
{
    QWidget* widget = NULL;
    if(index >= listWidget->count()) return;

    widget = listWidget->itemAt(index);
    if(widget != NULL){
        listWidget->removeWidget(widget);
        delete widget;
    }

    this->changeListHeight();
}

bool GListWidget::isListVisible()
{
    return visibleStatus;
}

int GListWidget::itemCount()
{
    return listWidget->count();
}

QWidget* GListWidget::itemAt(int index)
{
    return (listWidget->itemAt(index));
}

void GListWidget::changeListHeight()
{
    if(this->isListVisible())
        this->setFixedHeight(listWidget->height() + titleButton->height());
    else
        this->setFixedHeight(titleButton->height());
}

void GListWidget::on_titleButton_clicked()
{
    this->setListVisible(!this->isListVisible());
    if(this->isListVisible()){
        listWidget->show();
        this->setFixedHeight(listWidget->height() + titleButton->height());
        emit flexClicked(listWidget->height());
    }
    else{
        listWidget->hide();
        this->setFixedHeight(titleButton->height());
        emit flexClicked(-listWidget->height());
    }
}

GListWidget::~GListWidget(){}

