#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    // 选择文件夹，找出文件夹内所有符合格式的文档,并将这些文档按Map<unsigned int DocID, struct<string path, string lastmodifiedTime>>形式读入内存并保存到文件

    docCollectionPtr = new DocCollettion();     //采集文档并保存类
    docCollectionPtr->load();
    docSegmentationPtr = new DocSegmentation(); //对文档进行分词并保存类   
    indexPtr = new InvertedIndex();             //索引控制

    ui->setupUi(this);
//    indexPtr->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

//点击添加索引按钮
void MainWindow::on_addIndexButton_clicked()
{
    indexDialogPtr = new addIndexDialog();
    connect(indexDialogPtr, SIGNAL(sendData(QString, QStringList, QString)), this, SLOT(receiveData(QString, QStringList, QString)));
    indexDialogPtr->setModal(true);
    indexDialogPtr->show();
}

void MainWindow::receiveData(QString path, QStringList filter, QString indexName)
{
    docCollectionPtr->setFilters(filter);
    docCollectionPtr->findFiles(path);
    if(!docCollectionPtr->saveOnfile())
        qDebug("创建<文档信息文件>失败");
    else
        qDebug("创建<文档信息文件>成功");

    QMap<unsigned int, QString>::iterator docIter;          //遍历所有文档，根据路径打开
    QMap<string, DocSegmentation::WordsInfo>::iterator wordIter;

    for(docIter = docCollectionPtr->getNewDocInfo().begin(); docIter != docCollectionPtr->getNewDocInfo().end(); docIter++)
    {
        if(-1 == docSegmentationPtr->useJieba(docIter.value().toStdString()))     //对文档分词
            qDebug() << "文档" << docIter.value() << "打开失败";
        else {
            qDebug() << "文档" << docIter.key() << "分词完成";
            // 将得到的文档单词信息保存到索引
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
    qDebug() << "搜索:<" << ui->searchLine->text() << ">的结果";
    auto indexList = indexPtr->getIndexList();
    for(int i = 0; i < ui->indexList->count(); i++)
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
                        resultStr = "文档:" + iter.value() + " 出现次数:" + QString("%1").arg(docIter->count);
                        qDebug() << resultStr;
                        ui->resultListWidget->addItem(resultStr);
                    }
                }
            }

        }
    }
    if(!have)
    {
        qDebug() << "未找到包含<" << ui->searchLine->text() << ">的文档";
        ui->resultListWidget->insertItem(5,"未找到包含该词的文档");
    }

}

//加载索引并修改索引界面
void MainWindow::loadIndexUI()
{
    QDir dir("./../Index");    //打开索引文件夹
    if(!dir.exists())
    {
        qDebug() << "wrong path";
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
        qDebug() << indexFile.filePath() << " " << indexFile.fileName();

        if(indexPtr->loadIndex(indexFile.filePath().toStdString())) //加载索引成功，则添加索引图标
        {
            indexPtr->addToIndexList();                            //将索引添加到索引列表
            indexPtr->addToPathList(indexFile.filePath());
            addBox(indexFile.fileName());                           //添加索引界面
        }
        else {
            qDebug("添加索引失败");
        }
    }
}

inline void MainWindow::addBox(QString str){
    QListWidgetItem *item = new QListWidgetItem(str);
    item->setCheckState(Qt::Unchecked);
    ui->indexList->addItem(item);
}

void MainWindow::on_deleteIndexButton_clicked()
{
    for(int i = 0; i < ui->indexList->count(); i++){
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
