#ifndef MENUBAR_H
#define MENUBAR_H

class QPushButton;
class QMenu;
#include <QWidget>

class MenuBar : public QWidget
{
    Q_OBJECT
public:
    MenuBar(QWidget* parent = 0);
    ~MenuBar();

public:
    void setTitleText(const QString& text);
    void setPixmap(QPixmap& pixmap);
    QAction* addAction(const QString &text);
    QAction* addAction(const QIcon &icon, const QString &text);

private:
    QPushButton* btnMenuBar;
    QMenu* menuAction;

private:
    void initialWidget();

private slots:
    void on_btnMenuBar_clicked();
};

#endif // MENUBAR_H
