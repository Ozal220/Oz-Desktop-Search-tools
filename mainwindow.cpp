#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    // 选择文件夹，找出文件夹内所有符合格式的文档,并将这些文档按Map<unsigned int DocID, struct<string path, string lastmodifiedTime>>形式读入内存并保存到文件
    docCollectionPtr = new DocCollettion();     //采集文档并保存类
    docSegmentationPtr = new DocSegmentation(); //对文档进行分词并保存类
    indexPtr = new InvertedIndex();

    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_choiceDirButton_clicked()
{
    QString choicePath;
    choicePath = QFileDialog::getExistingDirectory(this, tr("选择文件夹"), ""); //选择路径
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

    QMap<unsigned int, DocCollettion::fileInfo> docInfo = docCollectionPtr->getDocInfo();     //获取docInfo
    QMap<unsigned int, DocCollettion::fileInfo>::iterator iter = docInfo.begin();          //遍历所有文档，根据路径打开
    while(iter != docInfo.end())
    {
        docSegmentationPtr->useJieba(iter.value().filePath.toStdString());      //对文档分词
        //indexPtr->getIndexMap().push
        iter++;
    }

}

