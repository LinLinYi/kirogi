#include <QVBoxLayout>
#include <QPushButton>
#include <QMenu>

#include "MenuBar.h"

MenuBar::MenuBar(QWidget* parent) : QWidget(parent)
{
    this->initialWidget();
}

void MenuBar::initialWidget()
{
    menuAction = new QMenu(this);
    menuAction->setAttribute(Qt::WA_LayoutUsesWidgetRect);

    btnMenuBar = new QPushButton(this);
    btnMenuBar->setStyleSheet("border-width: 0px;");
    btnMenuBar->setAttribute(Qt::WA_LayoutUsesWidgetRect);
    connect(btnMenuBar, SIGNAL(clicked()), this, SLOT(on_btnMenuBar_clicked()));

    QVBoxLayout* vboxLayout = new QVBoxLayout;
    vboxLayout->addWidget(btnMenuBar);
    vboxLayout->setSpacing(0);
    vboxLayout->setMargin(0);
    this->setLayout(vboxLayout);
}

QAction* MenuBar::addAction(const QString &text)
{
    menuAction->addSeparator();
    menuAction->setStyleSheet("border-style:solid; border-width:1px; border-color:rgb(208, 208, 208);");
    return menuAction->addAction(text);
}

QAction* MenuBar::addAction(const QIcon &icon, const QString &text)
{
    menuAction->addSeparator();
    menuAction->setStyleSheet("border-style:solid; border-width:1px; border-color:rgb(208, 208, 208);");
    return menuAction->addAction(icon, text);
}

void MenuBar::setTitleText(const QString &text)
{
    btnMenuBar->setText(text);
}

void MenuBar::setPixmap(QPixmap &pixmap)
{
    pixmap.setDevicePixelRatio(2);
    QIcon icon = QIcon(pixmap);
    btnMenuBar->setIcon(icon);

    QSize pixSize(pixmap.size().width()/2, pixmap.size().height()/2);
    btnMenuBar->setIconSize(pixSize);
}

void MenuBar::on_btnMenuBar_clicked()
{
    QPoint pos;
    pos.setX(0);
    pos.setY(this->height());
    menuAction->setFixedWidth(this->width()*2);
    menuAction->exec(btnMenuBar->mapToGlobal(pos));
}

MenuBar::~MenuBar()
{

}

