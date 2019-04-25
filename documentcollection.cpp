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

    foreach(QFileInfo mfi, dir.entryInfoList())
    {
        if(mfi.isFile())
        {
            qDebug()<< "File :" << mfi.filePath();
            this->newDocInfo.insert(this->docID, mfi.filePath());
            this->allDocInfo.insert(this->docID++, mfi.filePath());
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

// 将采集的文档信息保存在文件。格式为: id path
bool DocCollettion::saveOnfile()
{
    std::ofstream file;
    file.open(this->savedPath.toStdString(), std::ios::app);    //以追加方式添加
    if(!file.is_open())
    {
        qDebug("open file failed!");
        return false;
    }
    QMap<unsigned int, QString>::Iterator iter;
    for (iter = newDocInfo.begin(); iter != newDocInfo.end(); iter++) {
        file  << iter.key() << " " << iter.value().toStdString() << std::endl;
    }
    file.close();
    return true;
}

// 从已存文档读取信息，存到map: docInfo
void DocCollettion::load()
{
    QFileInfo file(savedPath);
    if(file.exists())              //如果已建立此文件
    {
        QFile file(savedPath);
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            qDebug("doc_info file open failed");
        }
        else {
            unsigned int id = 0;
            QString path;
            QTextStream in(&file);
            while (!in.atEnd()) {
                in >> id;
                path = in.readLine();
//                qDebug() << id << " " << path;
                this->allDocInfo.insert(id, path);
            }
            this->docID = id + 1;                   //本次docID自增从最后读取的id开始(这话可能只有自己懂。。。)
        }
    }
}
