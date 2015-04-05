#include <QVBoxLayout>
#include "ListWidget.h"

ListWidget::ListWidget(QWidget* parent) : QWidget(parent)
{
    widgetHeight = 0;
    widgetWidth = 0;
    this->initializeWidget();
    this->setAttribute(Qt::WA_AcceptTouchEvents);
}

void ListWidget::initializeWidget()
{
    vblTotalLayout = new QVBoxLayout(this);
    vblTotalLayout->setSpacing(0);
    vblTotalLayout->setMargin(0);
    vblTotalLayout->setAlignment(Qt::AlignTop);
    this->setFixedHeight(0);
    this->setLayout(vblTotalLayout);
}

void ListWidget::addWidget(QWidget *widget)
{
    if(widget == NULL) return;
    widgetHeight += widget->height();
    vblTotalLayout->addWidget(widget);
    this->setFixedHeight(widgetHeight);
}

void ListWidget::insertWidget(int index, QWidget *widget)
{
    if(widget == NULL) return;
    widgetHeight += widget->height();
    vblTotalLayout->insertWidget(index, widget);
    this->setFixedHeight(widgetHeight);
}

QWidget* ListWidget::removeWidget(int index)
{
    QWidget* widget = vblTotalLayout->itemAt(index)->widget();
    vblTotalLayout->removeWidget(widget);
    widgetHeight -= widget->height();
    this->setFixedHeight(widgetHeight);
    return widget;
}

void ListWidget::removeWidget(QWidget *widget)
{
    if(widget == NULL) return;
    widgetHeight -= widget->height();
    vblTotalLayout->removeWidget(widget);
    this->setFixedHeight(widgetHeight);
}

void ListWidget::takeAt(int index)
{
    QWidget* widget = this->removeWidget(index);
    if(widget == NULL) return;
    delete widget;
}

void ListWidget::takeAt(QWidget *widget)
{
    if(widget == NULL) return;
    this->removeWidget(widget);
    delete widget;
}

int ListWidget::indexof(QWidget *widget)
{
    return vblTotalLayout->indexOf(widget);
}

QWidget* ListWidget::itemAt(int index)
{
    if(index >=  vblTotalLayout->count() || index < 0) return NULL;
    QWidget* widget = vblTotalLayout->itemAt(index)->widget();
    return widget;
}

int ListWidget::height()
{
    return widgetHeight;
}

int ListWidget::count()
{
    return vblTotalLayout->count();
}

void ListWidget::setContentsMargins(int left, int top, int right, int bottom)
{
    if(left <= 0) left = 0;
    if(top <= 0) top = 0;
    if(right <= 0) right = 0;
    if(bottom <= 0) bottom = 0;
    QWidget::setContentsMargins(left, top, right, bottom);
}

ListWidget::~ListWidget()
{

}

