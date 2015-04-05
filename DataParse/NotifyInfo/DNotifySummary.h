#ifndef DNOTIFYSUMMARY_H
#define DNOTIFYSUMMARY_H

#include <QObject>
#include "GlobalDefine.hpp"

class DNotifySummary : public QObject
{
public:
    explicit DNotifySummary(QObject* parent = 0);
    explicit DNotifySummary(Mi::NotifyCategory type, QString& msgHint);
    DNotifySummary& operator=(const DNotifySummary& info);
    virtual ~DNotifySummary();

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
    quint64 notifyID;               //备注ID(根据情况使用)

private:
    QString getCurrentTime();
};

#endif // DNOTIFYSUMMARY_H
