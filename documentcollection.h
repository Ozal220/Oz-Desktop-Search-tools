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


class  DocumentCollettion {

public:

    DocumentCollettion() = default;
    ~DocumentCollettion();

    //void setFilters(QStringList filters);
    void setFilters(QStringList filters)
    {
        this->filters = filters;
    }

    //void findFiles(QString);
    void findFiles(QString path)
    {
        QDir dir(path);
        if(!dir.exists())
        {
            qDebug() << "wrong path";
            return;
        }
        if(this->filters.isEmpty())
        {
            qDebug() << "filters is empty, plese use setFilters(QStringList) set filters first";
            return;
        }
        dir.setNameFilters(this->filters);  //设置文件名称过滤器，只为filters格式
        dir.setFilter(QDir::AllDirs | QDir::Files | QDir::NoSymLinks | QDir::NoDot | QDir::NoDotDot); //设置类型过滤器，只为文件格式
        fileInfo fi;
        foreach(QFileInfo mfi, dir.entryInfoList())
        {
            if(mfi.isFile())
            {
                qDebug()<< "File :" << mfi.filePath();
                fi.id = docID;
                fi.filePath = mfi.filePath();
                fi.lastModifiedTime = mfi.lastModified().toString();
                this->docInfo.insert(docID++, fi);
                //ui->filesList->addItem(new QListWidgetItem("path: " + mfi.filePath() + " last modified time:" + mfi.lastModified().toString()));
                //ui->filesList-> setViewMode(QListView::IconMode); //用大圖示顯示
                //QObject::connect(fileList, SIGNAL(currentTextChanged(const QString &)),myLabel, SLOT(setText(const QString &)));
            }
            else
            {
                //qDebug() << "Entry Dir" << mfi.absoluteFilePath();
                findFiles(mfi.absoluteFilePath());
            }
        }
    }

    bool saveOnfile(QString);
    //bool saveOnfile();
    bool saveOnfile()
    {
        if(this->savedPath.isEmpty())
        {
            qDebug() << "path is empty";
            return false;
        }
        std::ofstream file;
        file.open(this->savedPath.toStdString());
        if(!file.is_open())
        {
            qDebug("open file failed!");
            return false;
        }
        foreach(auto s, this->docInfo)
        {

            file  << "#id= " << s.id << "#path= " << s.filePath.toStdString() << "#time= " << s.lastModifiedTime.toStdString() << std::endl;
        }
        file.close();
        return true;
    }

    typedef struct fileInfo     //保存在文件和内存中的value格式
    {
        QString filePath;
        QString lastModifiedTime;
        unsigned int id;
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
