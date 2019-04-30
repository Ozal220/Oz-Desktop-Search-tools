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
            if(mfi.suffix() == "pdf")                                   //pdf
            {
                qDebug("pdf2txt");
                QString newPath = this->tempPath + "pdf" + QString("%1").arg(docID) + ".txt";
                pdfToText(mfi.filePath(), newPath);
                this->newDocInfo.insert(this->docID, newPath);              //pdf转txt后的新地址
            }
            else if(mfi.suffix() == "html") {
                qDebug() << "html2txt" << ",filePath=" << mfi.filePath();
                QString newPath = this->tempPath + "html" + QString("%1").arg(docID) + ".txt";
                html2Text(mfi.filePath(), newPath);
                this->newDocInfo.insert(this->docID, newPath);
            }
            else {                                                          //txt
                this->newDocInfo.insert(this->docID, mfi.filePath());
            }
            this->allDocInfo.insert(this->docID++, mfi.filePath());

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
    QMap<unsigned int, QString>::Iterator iter = allDocInfo.find(newDocInfo.firstKey());
    int count = newDocInfo.size();
    for (int i = 0; i < count; i++) {
        file  << iter.key() << " " << iter.value().toStdString() << std::endl;
        iter++;
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

// 解析pdf文件，提取其中文本并保存txt文件
bool DocCollettion::pdfToText(QString originPath, QString newPath)
{
    QFile file(newPath);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "建立" << newPath << "失败";
        return false;
    }
    //调用pdf转txt程序
    QProcess *pdftotext = new QProcess();
    QString proPath = "./../Oz-DesktopSearch/xpdf/pdftotext.exe";
    QStringList argument;
    argument << "-raw" << "-nopgbrk" << originPath << newPath;
    pdftotext->start(proPath, argument);
    return true;
}

// 解析html文件，提取其中文本并保存txt文件
bool DocCollettion::html2Text(QString originPath, QString newPath)
{
    QFile file(newPath);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "建立" << newPath << "失败";
        return false;
    }
    //调用html转txt程序
    QProcess *pdftotext = new QProcess();
    qDebug() << "originPath:" << originPath;
    QString proPath = "./../Oz-DesktopSearch/htmlToTxt/htmlToTxt.exe";
    QStringList argument;
    argument << originPath << newPath;
    pdftotext->start(proPath, argument);
    return true;
}
