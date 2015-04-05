#ifndef ONLINESTATUSINFOLIST_H
#define ONLINESTATUSINFOLIST_H

#include <QObject>
#include <QHash>
#include "OnlineStatusInfo.h"

class OnlineStatusInfoList : public QObject
{
    Q_OBJECT
public:
    explicit OnlineStatusInfoList(QObject *parent = 0);
    virtual ~OnlineStatusInfoList();

public:
    void append(char* data, quint16 dataLen);
    QHash<quint64, OnlineStatusInfo*> fetchList();
    void appendList(char* data, quint16 dataLen, bool isClusterMember = false);

    void append(OnlineStatusInfo* onlineInfo);
    void remove(quint64 id);
    void remove(OnlineStatusInfo* onlineInfo);

private:
    QHash<quint64, OnlineStatusInfo*> onlineList;
};

#endif // ONLINESTATUSINFOLIST_H
