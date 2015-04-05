#ifndef DEVICETRANSNOTIFY_H
#define DEVICETRANSNOTIFY_H

#include <QObject>
#include "GlobalDefine.hpp"

class DeviceTransNotify : public QObject
{
public:
    explicit DeviceTransNotify(QObject* parent = 0);
    virtual ~DeviceTransNotify();

public:
    void unpack(char* data, Mi::DeviceTransfer type);
    void saveToLocal();

    void setTransUserID(quint64 id) { m_srcUserID = id; }
    void setTransUserName(QString& name) { m_srcUserName = name; }
    void setSrcClusterID(quint64 id) { m_srcClusterID = id; }
    void setSrcClusterName(QString& name) { m_srcClusterName = name; }
    void setDestClusterID(quint64 id) { m_destClusterID = id; }
    void setDestClusterName(QString& name) { m_destClusterName = name; }
    void setNotifyContent(QString& content) { m_msgContent = content;}
    void setNotifyVerify(QString& verify) { m_msgVerify = verify; }
    void setNotifyTime(QString& time) { m_msgTime = time;}
    void setReviewStatus(Mi::MsgStatus status) { m_msgStatus = status; }

    quint8 getMsgResult() { return m_msgResult; }
    quint64 getSrcUserID() { return m_srcUserID; }
    QString& getSrcUserName() { return m_srcUserName; }
    quint64 getSrcClusterID() { return m_srcClusterID; }
    QString& getSrcClusterName() { return m_srcClusterName; }
    quint64 getDestClusterID() { return m_destClusterID; }
    QString& getDestClusterName() { return m_destClusterName; }
    QString& getNotifyContent() { return m_msgContent; }
    QString& getNotifyTime() { return m_msgTime; }
    QString& getNotifyVerify() { return m_msgVerify; }
    Mi::MsgStatus getReviewStatus() { return m_msgStatus; }
    void updateInfoStatus(Mi::MsgStatus status, quint64 userID = 0, quint64 srcID = 0, quint64 destID = 0);

private:
    quint64 m_srcUserID;
    QString m_srcUserName;
    quint64 m_srcClusterID;
    QString m_srcClusterName;
    quint64 m_destClusterID;
    QString m_destClusterName;
    QString m_msgTime;
    QString m_msgContent;
    quint8 m_msgResult;
    QString m_msgVerify;
    Mi::MsgStatus m_msgStatus;

private:
    QString getCurrentTime();
};

#endif // DEVICETRANSNOTIFY_H
