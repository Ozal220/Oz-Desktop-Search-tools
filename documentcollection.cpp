#include <documentcollection.h>

// 采集文件夹内的所有符合格式的文档
void DocCollettion::findFiles(QString path)
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
            fi.filePath = mfi.filePath();
            fi.lastModifiedTime = mfi.lastModified().toString();
            this->docInfo.insert(this->docID++, fi);
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

// 将采集的文档信息保存在文件。格式为: id path time
bool DocCollettion::saveOnfile()
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
    /*
    foreach(auto s, this->docInfo)
    {

        file  << "#id= " << s << "#path= " << s.filePath.toStdString() << "#time= " << s.lastModifiedTime.toStdString() << std::endl;
    }
    */
    QMap<unsigned int, fileInfo>::Iterator iter;
    for (iter = docInfo.begin(); iter != docInfo.end(); iter++) {
        file  << "#id= " << iter.key() << " #path= " << iter.value().filePath.toStdString() << "  #time= " << iter.value().lastModifiedTime.toStdString() << std::endl;
    }
    file.close();
    return true;
}
