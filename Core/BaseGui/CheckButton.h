#ifndef CHECKBUTTON_H
#define CHECKBUTTON_H

class QLabel;

#include "GPushButton.h"
#include "GlobalDefine.hpp"

class CheckButton : public GPushButton
{
    Q_OBJECT
public:
    explicit CheckButton(QWidget *parent = 0);
    virtual ~CheckButton();

public:
    void setUserID(const quint64 id);
    void setUserName(const QString& name);
    void setClusterID(const quint64 id);
    void setClusterName(const QString& name);
    void setPixMap(QPixmap pixmap);
    void setReviewText(const QString& text);
    void setDescribeInfo(const QString& info);
    void setReviewEnable(const bool enable);
    void setCheckType(Mi::ClusterJoin type);
    void setCheckStatus(Mi::MsgStatus check);
    void setReserveID(quint64 id);
    void setReserveName(QString& name);

    inline quint64 getUserID() { return userID; }
    inline QString getUserName() { return userName; }
    inline quint64 getClusterID() { return clusterID; }
    inline quint64 getReserveID() { return reserveID; }
    inline Mi::ClusterJoin getCheckType() { return checkType; }
    inline Mi::MsgStatus getCheckStatus() { return curCheckStatus; }

private:
    quint16 screenHeight;
    quint16 screenWidth;

    quint64 clusterID;
    quint64 userID;
    QPixmap headImg;
    QString clusterName;
    QString userName;
    quint64 reserveID;
    QString reserveName;
    Mi::ClusterJoin checkType;
    Mi::MsgStatus curCheckStatus;

    QLabel* lblHeadImg;
    QLabel* lblName;
    QLabel* lblDescribe;
    QPushButton* btnReview;

private:
    void initializeWidget();

signals:
    void reviewClicked();

private slots:
    void on_btnReview_clicked();
};

#endif // CHECKBUTTON_H
