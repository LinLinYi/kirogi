#include "FileOperation.h"

FileOperation::FileOperation(QObject* parent) : QObject(parent){}
FileOperation::~FileOperation(){}

void FileOperation::writeInfoToFile(QString& fileName, QString &logText)
{
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;
    QTextStream out(&file);
    out<<logText<<"\n";
    file.close();
}

QStringList& FileOperation::readInfoFromFile(QString& fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return strList;

    QTextStream in(&file);
    QString line = in.readLine();
    while (!line.isNull()) {
       line = in.readLine();
       strList += line;
    }
    file.close();

    return strList;
}
