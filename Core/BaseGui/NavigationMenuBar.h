#ifndef NAVIGATIONMENUBAR_H
#define NAVIGATIONMENUBAR_H

class QPushButton;
class QLabel;
class QPixmap;
class QMenu;
class QMenuBar;
class QAction;
class MenuBar;

#include "BaseWidget.h"

class NavigationMenuBar : public BaseWidget
{
    Q_OBJECT
public:
    explicit NavigationMenuBar(QWidget* parent = 0);
    virtual ~NavigationMenuBar();

public:
    void setLeftText(QString& text);
    void setMenuText(QString& text);
    void setLeftPixMap(QPixmap& pixmap);
    void setMenuPixMap(QPixmap& pixmap);
    void setTitleText(QString& text);
    MenuBar* getMenuBar() { return m_menuBar; }

private:
    QPushButton* m_leftButton;
    MenuBar* m_menuBar;
    QLabel* m_titleLabel;

    QString leftButtonText;
    QString menuBarText;
    QPixmap leftButtonPixMap;
    QPixmap menuBarPixMap;
    QString titleText;

signals:
    void leftClicked();

private slots:
    void on_leftBtn_clicked();

private:
    void initializeWidget();
};

#endif // NAVIGATIONMENUBAR_H
