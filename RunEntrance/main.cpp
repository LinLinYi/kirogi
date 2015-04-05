#include <QApplication>
#include <QTextCodec>
#include <QDir>
#include <QFileInfoList>
#include <QFile>
#include "InitManagment.h"

bool qCopyDirectory(const QDir& fromDir, const QDir& toDir, bool bCoverIfFileExists = true);

#ifdef Q_OS_IOS
extern "C" int qtmn(int argc, char **argv)
#else
int main(int argc, char **argv)
#endif
{
    QApplication::addLibraryPath("./plugins");
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QApplication app(argc, argv);
    app.setAttribute(Qt::AA_UseHighDpiPixmaps);
    new InitManagment;
    return app.exec();
}

/**
  qCopyDirectory -- 拷贝目录
  fromDir : 源目录
  toDir   : 目标目录
  bCoverIfFileExists : ture:同名时覆盖  false:同名时返回false,终止拷贝
  返回: ture拷贝成功 false:拷贝未完成
*/
bool qCopyDirectory(const QDir& fromDir, const QDir& toDir, bool bCoverIfFileExists)
{
    QDir formDir_ = fromDir;
    QDir toDir_ = toDir;

    if(!toDir_.exists())
    {
        if(!toDir_.mkdir(toDir.absolutePath()))
            return false;
    }

    QFileInfoList fileInfoList = formDir_.entryInfoList();
    foreach(QFileInfo fileInfo, fileInfoList)
    {
        if(fileInfo.fileName() == "." || fileInfo.fileName() == "..")
            continue;

        //拷贝子目录
        if(fileInfo.isDir())
        {
            //递归调用拷贝
            if(!qCopyDirectory(fileInfo.filePath(), toDir_.filePath(fileInfo.fileName())))
                return false;
        }
        //拷贝子文件
        else
        {
            if(bCoverIfFileExists && toDir_.exists(fileInfo.fileName()))
            {
                toDir_.remove(fileInfo.fileName());
            }
            if(!QFile::copy(fileInfo.filePath(), toDir_.filePath(fileInfo.fileName())))
            {
                return false;
            }
        }
    }
    return true;
}
