#ifndef DOCUMENTCOLLECTION_H
#define DOCUMENTCOLLECTION_H

#include <QStringList>
#include <QDir>
#include <QFileInfo>
#include <QDirIterator>
#include <QStringListIterator>
#include <QDebug>
#include <QStringList>
#include <QFileDialog>
#include <QDesktopServices>
#include <QDateTime>
#include <fstream>


class  DocCollettion {

public:

    DocCollettion() = default;
    ~DocCollettion();

    void setFilters(QStringList filters)
    {
        this->filters = filters;
    }

    void findFiles(QString);

    bool saveOnfile(QString);
    bool saveOnfile();

    void clearDocInfo()
    {
        docInfo.clear();
    }

    typedef struct fileInfo     //保存在文件和内存中的value格式
    {
        QString filePath;
        QString lastModifiedTime;
        //unsigned int id;
    }FileInfo;

    QMap<unsigned int, fileInfo> getDocInfo()
    {
        return this->docInfo;
    }

private:
    unsigned int docID = 0;
    QStringList filters = {"*.txt", "*.html", "*.htm"};    //filters << "*.txt" << "*.html" << "*.htm" ;
    QString savedPath = "C:/Users/10367/Desktop/Desktop Search/Oz-Search Project/docs_path_info.txt";

    QMap<unsigned int, fileInfo> docInfo;
};


#endif // DOCUMENTCOLLECTION_H
