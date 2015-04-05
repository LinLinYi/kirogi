#ifndef SEARCHBAR_H
#define SEARCHBAR_H

class QLabel;
class QLineEdit;
class QPushButton;
class QEvent;

#include "BaseWidget.h"

class SearchBar : public BaseWidget
{
    Q_OBJECT

public:
    explicit SearchBar(QWidget* parent = 0);
    virtual ~SearchBar();

public:
    quint32 barHeight();
    void setPlaceholderText(QString& text);
    void setButtonStyle(QString barStyle);

private:
    QLabel* lblSearchHintImg;       /*! 搜索提示标签*/
    QLineEdit* editSearchContent;   /*! 搜索输入框*/
    QPushButton* btnSearch;         /*! 搜索按钮*/
    quint32 searchBarHeight;        /*! 按钮高度*/

private:
    void initializeWidget();

signals:
    void searchInputCompleted(QString& content);

private slots:
    void on_btnSearch_clicked();
};

#endif // SEARCHBAR_H
