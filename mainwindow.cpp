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
    indexPtr = new InvertedIndex();

    indexPtr->loadIndex();
    qDebug("已加载索引");
    indexPtr->show();
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_choiceDirButton_clicked()
{
    QString choicePath;
    choicePath = QFileDialog::getExistingDirectory(this, tr("选择文件夹"), "./../Test"); //选择路径
    if(choicePath.isEmpty()) return;
    else
    {
        ui->DirLabel->setText(choicePath);
    }
}

void MainWindow::on_pushButton_clicked()
{
    docCollectionPtr->findFiles(ui->DirLabel->text());
    if(!docCollectionPtr->saveOnfile())
        qDebug("创建<文档信息文件>失败");
    qDebug("创建<文档信息文件>成功");

    QMap<unsigned int, DocCollettion::fileInfo>::iterator docIter;          //遍历所有文档，根据路径打开
    QMap<string, DocSegmentation::WordsInfo>::iterator wordIter;

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
            indexPtr->addToIndex(docIter.key(), wordIter.key(), wordIter.value().count, wordIter.value().pos);
        }
          docSegmentationPtr->clearWordsMap();
        }

    }
    indexPtr->show();
    indexPtr->saveOnFile();
}

void MainWindow::on_searchButton_clicked()
{
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

}
