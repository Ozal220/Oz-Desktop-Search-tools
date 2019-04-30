#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    /*
     *  1.加载已存的索引文件到内存，并加载界面。在main.cpp里调用loadIndexUI()
     *  2.添加新索引：
     *      2.1 打开文件夹采集其中用户指定的类型的文件<txt/html/pdf>, html和pdf需要解析，解析后的文件放到TempFile文件里。文件路径保存到内存和文件
     *      2.2 逐个文件(这时都转为了txt)进行分词
     *      2.3 分词后的结果存到index里，index添加到indexList
     */
    docCollectionPtr = new DocCollettion();     //采集文档并保存类
    docCollectionPtr->load();
    docSegmentationPtr = new DocSegmentation(); //对文档进行分词并保存类   
    indexPtr = new InvertedIndex();             //索引控制
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//加载索引并修改索引界面
void MainWindow::loadIndexUI()
{
    QDir dir("./../Index");    //打开索引文件夹
    if(!dir.exists())
    {
        qDebug() << "wrong path!";
        return;
    }

    dir.setFilter(QDir::Files | QDir::NoDot | QDir::NoDotDot);
    if(dir.entryInfoList().isEmpty())
    {
        qDebug("不存在已建立的索引");
        return;
    }

    ui->indexList->setSelectionMode(QAbstractItemView::MultiSelection);
    for(auto indexFile : dir.entryInfoList())   //循环打开文件夹内的文件
    {
        //qDebug() << indexFile.filePath() << " " << indexFile.fileName();
        if(indexPtr->loadIndex(indexFile.filePath().toStdString())) //加载索引成功，则添加索引图标
        {
            indexPtr->addToIndexList();                            //将新索引添加到索引列表
            qDebug() << "添加第" << indexPtr->getIndexMap().size() << "个索引:" << indexFile.fileName();
            //indexPtr->addToPathList(indexFile.filePath());
            addBox(indexFile.fileName());                           //添加索引界面
        }
        else {
            qDebug("添加索引失败");
        }
    }
}


//添加索引按钮点击事件->打开addIndexDialog窗口.用receiveData函数获取信息
void MainWindow::on_addIndexButton_clicked()
{
    indexDialogPtr = new addIndexDialog();
    connect(indexDialogPtr, SIGNAL(sendData(QString, QStringList, QString)), this, SLOT(receiveData(QString, QStringList, QString)));
    indexDialogPtr->setModal(true);
    indexDialogPtr->show();
}


//文件采集-分词-创建索引
void MainWindow::receiveData(QString path, QStringList filter, QString indexName)
{
    //文件采集并保存模块
    docCollectionPtr->setFilters(filter);
    docCollectionPtr->findFiles(path);
    if(!docCollectionPtr->saveOnfile())
        qDebug("创建<文档信息文件>失败");
    else
        qDebug("创建<文档信息文件>成功");


    //遍历所有新文档、进行分词、添加到索引
    QMap<unsigned int, QString>::iterator docIter;
    QMap<string, DocSegmentation::WordsInfo>::iterator wordIter;

    for(docIter = docCollectionPtr->getNewDocInfo().begin(); docIter != docCollectionPtr->getNewDocInfo().end(); docIter++)
    {
        if(-1 == docSegmentationPtr->useJieba(docIter.value().toStdString()))   //对文档分词
            qDebug() << docIter.value() << "分词失败";
        else                                                                    //分词后的数据保存到索引
        {
            qDebug() << docIter.key() << "分词完成";
            for (wordIter = docSegmentationPtr->getWordsMap().begin(); wordIter != docSegmentationPtr->getWordsMap().end(); wordIter++)
            {
                // 插入到索引
                indexPtr->addToIndex(docIter.key(), wordIter.key(), wordIter.value().count, wordIter.value().pos);
            }
            docSegmentationPtr->clearWordsMap();
        }
    }
    docCollectionPtr->getNewDocInfo().clear();
    indexPtr->saveOnFile(indexName);
    indexPtr->addToIndexList();                     //将新创建的索引加入到<索引集>
    addBox(indexName);
}


//点击搜索按钮
void MainWindow::on_searchButton_clicked()
{
    ui->resultListWidget->clear();
    bool have = false;
    QString resultStr;
    QStringList splitList;
    auto indexList = indexPtr->getIndexList();

    for(int i = 0; i < ui->indexList->count(); i++)                 //遍历索引列表，找出选中的索引
    {
        if(ui->indexList->item(i)->checkState() == Qt::Checked)
        {
            auto result = indexList.at(i).find(ui->searchLine->text().toStdString());
            if(result != indexList.at(i).end())
            {
                have = true;
                for(auto docIter = result.value().begin(); docIter != result.value().end(); docIter++)
                {
                    auto iter = docCollectionPtr->getAllDocInfo().find(docIter.key());
                    if(!iter->isNull()){
                        splitList = iter.value().split('/');                //按/分割，最后的字符串为文件名
                        resultStr = "文档:" + splitList.last() + " 出现次数:" + QString("%1").arg(docIter->count);
                        ui->resultListWidget->addItem(resultStr);
                    }
                }
            }
        }
    }
    if(!have)
    {
        ui->resultListWidget->insertItem(0, "                           未找到包含该词的文档");
    }

}


inline void MainWindow::addBox(QString str){
    QListWidgetItem *item = new QListWidgetItem(str);
    item->setCheckState(Qt::Unchecked);
    ui->indexList->addItem(item);
}


void MainWindow::on_deleteIndexButton_clicked()
{
    for(int i = 0; i < ui->indexList->count(); i++)
    {
        if(ui->indexList->item(i)->checkState() == Qt::Checked){
            ui->indexList->takeItem(i);                     //删除界面对应行
            indexPtr->getIndexList().removeAt(i);           //删除内存中的索引
            QFile delFile(indexPtr->getPathList().at(i));   //删除索引文件
            delFile.remove();
            indexPtr->getPathList().removeAt(i);            //删除索引地址
            i--;
        }
    }
}
