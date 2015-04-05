#ifndef UNOTIFYSUMMARY_H
#define UNOTIFYSUMMARY_H

#include <QObject>
#include "GlobalDefine.hpp"

class UNotifySummary : public QObject
{
public:
    explicit UNotifySummary(QObject* parent = 0);
    explicit UNotifySummary(Mi::NotifyCategory type, QString& msgHint);
    UNotifySummary& operator=(const UNotifySummary& info);
    virtual ~UNotifySummary();

public:
    void setNotifyType(const Mi::NotifyCategory type) { notifyType = type; }
    void setNotifyName(const QString& name) { notifyName = name; }
    void setNotifyContent(const QString& hint) { notifyContent = hint; }
    void setNotifyImg(const QString& img) { notifyImg = img; }
    void setNotifyTime(const QString& time) { notifyTime = time; }
    void setNotifyID(const quint64 data) { notifyID = data; }
    void setUnreadCount(const quint64 count) { unReadCount = count; }

    QString& getNotifyName() { return notifyName; }
    QString& getNotifyTime() { return notifyTime; }
    QString& getNotifyContent() { return notifyContent; }
    quint16 getUnreadCount() { return unReadCount; }
    QString& getNotifyImg() { return notifyImg; }
    quint64 getNotifyID() { return notifyID; }
    Mi::NotifyCategory getNotifyType() { return notifyType; }

    void updateUnreadMsgCount(int count);//更新未读消息数目

private:
    Mi::NotifyCategory notifyType;  //通知类型
    QString notifyName;             //通知名称
    QString notifyTime;             //通知时间
    QString notifyContent;          //通知提示
    quint16 unReadCount;            //未读消息数目
    QString notifyImg;              //通知图片
    quint64 notifyID;               //通知ID

private:
    QString getCurrentTime();
};

#endif // UNOTIFYSUMMARY_H
