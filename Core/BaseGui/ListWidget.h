#ifndef LISTWIDGET_H
#define LISTWIDGET_H

class QVBoxLayout;
#include <QWidget>

class ListWidget : public QWidget
{
public:
    explicit ListWidget(QWidget* parent = 0);
    virtual ~ListWidget();

public:
    void addWidget(QWidget* widget);
    void insertWidget(int index, QWidget* widget);
    QWidget* removeWidget(int index);
    void removeWidget(QWidget* widget);
    void takeAt(int index);
    void takeAt(QWidget* widget);
    int indexof(QWidget* widget);
    QWidget* itemAt(int index);
    int height();
    int count();
    void setContentsMargins(int left, int top, int right, int bottom);

private:
    QVBoxLayout* vblTotalLayout;
    int widgetHeight;
    int widgetWidth;

private:
    void initializeWidget();
};

#endif // LISTWIDGET_H
