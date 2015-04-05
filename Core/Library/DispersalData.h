#ifndef DISPERSALDATA_H
#define DISPERSALDATA_H

#include <QObject>
#include "ToolKit.h"

class DispersalData : public QObject
{
    Q_OBJECT
public:
    explicit DispersalData(QObject* parent = 0);
    virtual ~DispersalData();

public:
    quint16 outlet(QString& dest, char* src);
    quint16 outlet(char *dest, char *src);
    quint16 outlet(char *dest, char *src, quint16 len);
    template<typename T> quint16 outlet(T& dest, char *src){
        ToolKit::NetToHostEndian(dest, src);
        return sizeof(dest);
    }
};

#endif // DISPERSALDATA_H
