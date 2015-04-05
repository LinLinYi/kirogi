#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QEvent>

#include "SearchBar.h"
#include "SheetStyleConfig.hpp"
#include "ImageConfig.hpp"

SearchBar::SearchBar(QWidget* parent) : BaseWidget(parent)
{
    this->initializeWidget();
}

void SearchBar::initializeWidget()
{
    //搜索框
    editSearchContent = new QLineEdit(this);
    editSearchContent->setStyleSheet(SheetStyle::SEARCHBAR_LINEEDIT);
    editSearchContent->setFixedHeight(this->screenHeight()*0.077);
    editSearchContent->setPlaceholderText(tr("搜索"));
    editSearchContent->installEventFilter(this);
    editSearchContent->setTextMargins(this->screenWidth()*0.047,0,0,0);

    // 搜索按钮
    btnSearch = new QPushButton(this);
    btnSearch->setFixedHeight(this->screenHeight()*0.077);
    btnSearch->setFixedWidth(this->screenWidth()*0.15);
    btnSearch->setStyleSheet(SheetStyle::SEARCHBAR_BUTTON);
    QPixmap pixmap(ImagePath::BLACK_SEARCH_BAR);
    pixmap.setDevicePixelRatio(2.0);
    btnSearch->setIcon(QIcon(pixmap));
    connect(btnSearch, SIGNAL(clicked()), this, SLOT(on_btnSearch_clicked()));

    QVBoxLayout* vbSearchLayout = new QVBoxLayout;
    vbSearchLayout->addWidget(btnSearch);
    vbSearchLayout->setMargin(0);

    QHBoxLayout* hblSearch = new QHBoxLayout(this);
    hblSearch->addWidget(editSearchContent);
    hblSearch->addLayout(vbSearchLayout);
    hblSearch->setSpacing(0);
    hblSearch->setMargin(0);

    searchBarHeight = this->screenHeight()*0.077;
    this->setLayout(hblSearch);
}

quint32 SearchBar::barHeight()
{
    return searchBarHeight;
}

void SearchBar::setPlaceholderText(QString &text)
{
    editSearchContent->setPlaceholderText(text);
}

void SearchBar::setButtonStyle(QString barStyle)
{
    btnSearch->setStyleSheet(barStyle);
}

void SearchBar::on_btnSearch_clicked()
{
    QString value = editSearchContent->text().trimmed();
    emit searchInputCompleted(value);
}

SearchBar::~SearchBar()
{

}
