#ifndef GLISTWIDGET_H
#define GLISTWIDGET_H

class QVBoxLayout;
class QPixmap;
class GroupButton;
class ListWidget;

#include <QWidget>

class GListWidget : public QWidget
{
    Q_OBJECT
public:
    GListWidget(QWidget* parent = 0);
    ~GListWidget();

public:
    void setPixmap(const QPixmap& pixMap);
    void setTitle(const QString& title);
    void setTitleStyleSheet(const QString& objName);
    void setTitleHeight(int height);
    void setListVisible(bool visible);
    void setContentsMargins(int left, int top, int right, int bottom);

    int titleHeight();
    int listWidth();
    int listHeight();

    void addWidget(QWidget* widget);
    void insertWidget(int index, QWidget* widget);
    QWidget* removeWidget(int index);
    void takeAt(int index);
    bool isListVisible();
    int itemCount();
    QWidget* itemAt(int index);
    void changeListHeight();

signals:
    void flexClicked(int listHeight);

private slots:
    void on_titleButton_clicked();

private:
    QString title;                  /*! 标题栏标题*/
    QPixmap titlePixmap;            /*! 标题栏图片*/
    GroupButton* titleButton;       /*! 列表分组按钮*/

    bool visibleStatus;             /*! 列表项可视状态*/
    QVBoxLayout* vblTotalLayout;    /*! 整体布局*/
    ListWidget* listWidget;         /*! 列表条目布局*/

private:
    void initializeWidget();
    int setListWidgetView();
};

#endif // GLISTWIDGET_H
