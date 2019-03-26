#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "include/Application.hpp"
using namespace CppJieba;

#include <QDir>
#include <QFileInfo>
#include <QDirIterator>
#include <QStringListIterator>
#include <QDebug>
#include <QStringList>
#include <QFileDialog>
#include <QDesktopServices>

void findFiles(QString);

int JiebaTest() {
    CppJieba::Application app("./../Oz-DesktopSearch/dict/jieba.dict.utf8",
                            "./../Oz-DesktopSearch/dict/hmm_model.utf8",
                            "./../Oz-DesktopSearch/dict/user.dict.utf8",
                            "./../Oz-DesktopSearch/dict/idf.utf8",
                            "./../Oz-DesktopSearch/dict/stop_words.utf8");
    vector<string> words;
    string result;
    string s = "我叫吾孜艾力·热夏提,搞这个破分词工具搞了一天了,拜托结巴大哥好好运作吧！。";
    string buf;
    ofstream ofs("./../shit.txt");
    if(ofs.is_open())
    {
        ofs << "test output ：" << s << endl;

        ofs << "\n\e[32m" << "[demo] METHOD_MP" << "\e[0m\n"; // colorful
        app.cut(s, words, METHOD_MP);
        ofs << join(words.begin(), words.end(), "/") << endl;

        ofs << "\n\e[32m" << "[demo] METHOD_HMM" << "\e[0m\n"; // colorful
        app.cut(s, words, METHOD_HMM);
        ofs << join(words.begin(), words.end(), "/") << endl;

        ofs << "\n\e[32m" << "[demo] METHOD_MIX" << "\e[0m\n"; // colorful
        app.cut(s, words, METHOD_MIX);
        ofs << join(words.begin(), words.end(), "/") << endl;

        ofs << "\n\e[32m" << "[demo] METHOD_FULL" << "\e[0m\n"; // colorful
        app.cut(s, words, METHOD_FULL);
        ofs << join(words.begin(), words.end(), "/") << endl;

        ofs << "\n\e[32m" << "[demo] METHOD_QUERY" << "\e[0m\n"; // colorful
        app.cut(s, words, METHOD_QUERY);
        ofs << join(words.begin(), words.end(), "/") << endl;


        ofs << "\n\e[32m" << "[demo] TAGGING" << "\e[0m\n"; // colorful
        vector<pair<string, string> > tagres;
        app.tag(s, tagres);
        ofs << s << endl;
        ofs << tagres << endl;;

        ofs << "\n\e[32m" << "[demo] KEYWORD" << "\e[0m\n"; // colorful
        vector<pair<string, double> > keywordres;
        app.extract(s, keywordres, 5);
        ofs << s << endl;
        ofs << keywordres << endl;
    }
    return 0;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    JiebaTest();
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

