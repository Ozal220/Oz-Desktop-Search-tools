#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "cppjieba/include/cppjieba/Jieba.hpp"
using namespace std;

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
    const string DICT_PATH = "./../Oz-DesktopSearch/cppjieba/dict/jieba.dict.utf8";
    const string HMM_PATH = "./../Oz-DesktopSearch/cppjieba/dict/hmm_model.utf8";
    const string USER_DICT_PATH = "./../Oz-DesktopSearch/cppjieba/dict/user.dict.utf8";
    const string IDF_PATH = "./../Oz-DesktopSearch/cppjieba/dict/idf.utf8";
    const string STOP_WORD_PATH = "./../Oz-DesktopSearch/cppjieba/dict/stop_words.utf8";

    cppjieba::Jieba jieba(DICT_PATH,
        HMM_PATH,
        USER_DICT_PATH,
        IDF_PATH,
        STOP_WORD_PATH);

    vector<string> words;
    vector<cppjieba::Word> words_with_offset;

    string s = "吾孜艾力·热夏提硕士毕业于china科学院计算所，后在日本京都大学深造japanese sorry";
    string s1= "我是拖拉机学院手扶拖拉机专业的。不用多久，我就会升职加薪，当上CEO，走上人生巅峰。";

    ofstream ofs("./../shit.txt");
    if(ofs.is_open())
    {
        qDebug("open shit success");
        ofs << "test output ：" << s1 << endl;
        jieba.CutForSearch(s1, words_with_offset, true);
        for(cppjieba::Word word : words_with_offset){

            ofs << word << endl;
        }
    }
    return 0;
}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    JiebaTest();
    //ui->setupUi(this);
//    QString genPath = "C:/Users/10367/Desktop/Desktop Search";
//    findFiles(genPath);
}

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

