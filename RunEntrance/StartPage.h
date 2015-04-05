#ifndef STARTPAGE_H
#define STARTPAGE_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QPaintEvent>

class StartPage : public QWidget
{
    Q_OBJECT
public:
    explicit StartPage(QWidget* parent = 0);
    virtual ~StartPage();

private:
    quint16 screenHeight;
    quint16 screenWidth;

protected:
    void paintEvent(QPaintEvent* event);
};

#endif // STARTPAGE_H
