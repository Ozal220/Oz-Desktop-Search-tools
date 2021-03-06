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
#include <QProcess>

class  DocCollettion {

public:

    DocCollettion() = default;
    ~DocCollettion();

    void setFilters(QStringList filters)
    {
        this->filters = filters;
    }

    void findFiles(QString);

    bool saveOnfile();

    void load();

    bool pdfToText(QString originPath, QString newPath);

    bool html2Text(QString originPath, QString newPath);

    //key = docId, value = path
    QMap<unsigned int, QString> &getNewDocInfo()
    {
        return this->newDocInfo;
    }

    QMap<unsigned int, QString> &getAllDocInfo()
    {
        return this->allDocInfo;
    }

private:
    unsigned int docID = 0;                                 // 为每个文档分配ID,从0开始自增
    QStringList filters = {"*.txt", "*.html", "*.htm", "*.pdf"};    //filters << "*.txt" << "*.html" << "*.htm" ;
    QString savedPath = "./../docs_path_info.txt";
    QString tempPath = "./../TempFile/";

    QMap<unsigned int, QString> newDocInfo;
    QMap<unsigned int, QString> allDocInfo;
};


#endif // DOCUMENTCOLLECTION_H
