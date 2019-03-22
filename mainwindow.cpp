#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDir>
#include <QFileInfo>
#include <QDirIterator>
#include <QStringListIterator>
#include <QDebug>
#include <QStringList>
#include <QFileDialog>

void findFiles(QString);

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//    QString genPath = "C:/Users/10367/Desktop/Desktop Search";
//    findFiles(genPath);
}

// 尝试QtCreator用git

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::findFiles(QString path){
  QDir dir(path);
  if(!dir.exists())
  {
      qDebug() << "wrong path";
      return;
  }
  QStringList filters;
  filters << "*.txt" << "*.html" << "*.htm" ;
  dir.setNameFilters(filters);  //设置文件名称过滤器，只为filters格式
  dir.setFilter(QDir::AllDirs | QDir::Files | QDir::NoSymLinks | QDir::NoDot | QDir::NoDotDot); //设置类型过滤器，只为文件格式
  foreach(QFileInfo mfi, dir.entryInfoList())
  {
    if(mfi.isFile())
    {
        qDebug()<< "File :" << mfi.fileName();
        ui->filesList->addItem(new QListWidgetItem(mfi.fileName()));
        //ui->filesList->setViewMode(QListView::IconMode); //用大圖示顯示
        //QObject::connect(fileList, SIGNAL(currentTextChanged(const QString &)),myLabel, SLOT(setText(const QString &)));
    }
    else
    {
        //qDebug() << "Entry Dir" << mfi.absoluteFilePath();
        findFiles(mfi.absoluteFilePath());
    }
  }
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
    findFiles(ui->DirLabel->text());
}
