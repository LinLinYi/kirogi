#ifndef FILEOPERATION_H
#define FILEOPERATION_H

#include <QFile>
#include <QTextStream>
#include <QStringList>
#include "FilePath.h"

class FileOperation : public QObject
{
public:
    explicit FileOperation(QObject* parent = 0);
    virtual ~FileOperation();

public:
   void writeInfoToFile(QString& fileName, QString& logText);
   QStringList& readInfoFromFile(QString& fileName);

private:
   QStringList strList;
};

#endif // FILEOPERATION_H
