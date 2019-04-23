#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTextCodec>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QTextCodec *codec = QTextCodec::codecForName("utf-8");
    QTextCodec::setCodecForLocale(codec);
    // 选择文件夹，找出文件夹内所有符合格式的文档,并将这些文档按Map<unsigned int DocID, struct<string path, string lastmodifiedTime>>形式读入内存并保存到文件

    docCollectionPtr = new DocCollettion();     //采集文档并保存类
    docSegmentationPtr = new DocSegmentation(); //对文档进行分词并保存类
    // 打开索引文件夹，并加载索引,修改索引部分的界面

//    indexPtr = new InvertedIndex();
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
    QString choicePath;
    choicePath = QFileDialog::getExistingDirectory(this, tr("选择文件夹"), "./../Test"); //选择路径
    if(choicePath.isEmpty()) return;

    docCollectionPtr->findFiles(choicePath);
    if(!docCollectionPtr->saveOnfile())
        qDebug("创建<文档信息文件>失败");
    qDebug("创建<文档信息文件>成功");

    QMap<unsigned int, DocCollettion::fileInfo>::iterator docIter;          //遍历所有文档，根据路径打开
    QMap<string, DocSegmentation::WordsInfo>::iterator wordIter;
    InvertedIndex newIndex();

    for(docIter = docCollectionPtr->getDocInfo().begin(); docIter != docCollectionPtr->getDocInfo().end(); docIter++)
    {
        if(-1 == docSegmentationPtr->useJieba(docIter.value().filePath.toStdString()))     //对文档分词
            qDebug() << "文档" << docIter.value().filePath << "打开失败";
        else {
            qDebug() << "文档" << docIter.key() << "分词完成";
            docSegmentationPtr->showMap();
        // 将得到的文档单词信息保存到索引
        for (wordIter = docSegmentationPtr->getWordsMap().begin(); wordIter != docSegmentationPtr->getWordsMap().end(); wordIter++)
        {
            // 插入到索引
            newIndex.addToIndex(docIter.key(), wordIter.key(), wordIter.value().count, wordIter.value().pos);
        }
          docSegmentationPtr->clearWordsMap();
        }
    }
    //newIndex.show();
    newIndex().saveOnFile();
    indexes.push_back(newIndex);                        //添加新索引
    addBox("new index");
}

//点击搜索按钮
void MainWindow::on_searchButton_clicked()
{
/*
    qDebug() << "搜索:<" << ui->searchLine->text() << ">的结果";
    auto result = indexPtr->getIndexMap().find(ui->searchLine->text().toStdString());
   if(result == indexPtr->getIndexMap().end())
        qDebug() << "未找到包含<" << ui->searchLine->text() << ">的文档";
    else {
        auto docIter = result.value().begin();
        while(docIter != result.value().end())
        {
            qDebug() << "文档:" << docIter.key() << " 出现次数:" << docIter->count << " 位置" << docIter->pos;
            docIter++;
        }
    }
*/
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
    InvertedIndex newIndex;
    for(auto indexFile : dir.entryInfoList())   //循环打开文件夹内的文件
    {
        qDebug() << indexFile.filePath() << " " << indexFile.fileName();
        if(newIndex.loadIndex(indexFile.filePath().toStdString())) //加载索引成功，则添加索引图标
        {
            indexes.push_back(newIndex);                            //将索引添加到索引列表
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
