#ifndef NAVIGATIONBAR_H
#define NAVIGATIONBAR_H

class QPushButton;
class QLabel;
class QPixmap;

#include <QWidget>

class NavigationBar : public QWidget
{
    Q_OBJECT
public:
    explicit NavigationBar(QWidget* parent = 0);
    virtual ~NavigationBar();

public:
    void setLeftText(const QString& leftText);
    void setRightText(const QString& rightText);
    void setLeftPixMap(const QPixmap& leftPix);
    void setRightPixMap(const QPixmap& rightPix);
    void setTitleText(const QString& tilText);

private:
    QPushButton* leftButton;         //左边按钮
    QPushButton* rightButton;        //右边按钮
    QLabel* titleLabel;              //中间标题

    QString leftButtonText;
    QString rightButtonText;
    QPixmap leftButtonPixMap;
    QPixmap rightButtonPixMap;
    QString titleText;
    
    quint16 screenHeight;
    quint16 screenWidth;

signals:
    void rightClicked();
    void leftClicked();

private slots:
    void on_leftBtn_clicked();
    void on_rightBtn_clicked();

private:
    void initializeWidget();
};

#endif // NAVIGATIONBAR_H
