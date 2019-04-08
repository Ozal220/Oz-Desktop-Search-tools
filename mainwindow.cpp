#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cppjieba/include/cppjieba/Jieba.hpp"

#include <QDebug>
using namespace std;

typedef struct wordsInfo{
    unsigned int count = 0;    // 单词出现次数
    vector<uint32_t> pos;   // 单词出现位置
}WordsInfo;

int JiebaTest(string file_path) {
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

    string s;
    char buffer[512];
    vector<cppjieba::Word> words_with_offset;

    WordsInfo word_info;
    QMap<string, WordsInfo> words_map;
    QMap<string, WordsInfo>::Iterator iter;

    ifstream ifs(file_path);
    if(!ifs.is_open())
    {
        qDebug() << "open " << file_path.c_str() << " error";
        return -1;
    }
    if(!ifs.eof())
    {
        ifs.getline(buffer, 512);
        s = buffer;
        jieba.CutForSearch(s, words_with_offset);
        for(auto word : words_with_offset)
        {
            iter = words_map.find(word.word);
            if(iter != words_map.end()){
                // 单词已存在
                iter->count += 1;               //出现次数+1
                iter->pos.push_back(word.unicode_offset);   //添加偏移量pos
            }
            else {
                // 新单词
                word_info.count = 1;
                word_info.pos.push_back(word.unicode_offset);
                words_map.insert(word.word, word_info);
            }

        }
    }
/*
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
*/

    return 0;
}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // 选择文件夹，找出文件夹内所有符合格式的文档,并将这些文档按Map<unsigned int DocID, struct<string path, string lastmodifiedTime>>形式读入内存并保存到文件
    docControlPtr = new DocumentCollettion();

    //
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
    docControlPtr->findFiles(ui->DirLabel->text());
    if(!docControlPtr->saveOnfile())
        qDebug("创建<文档信息文件>失败");
    qDebug("创建<文档信息文件>成功");

    QMap<unsigned int, DocumentCollettion::fileInfo> docInfo = docControlPtr->getDocInfo();     //获取docInfo
    QMap<unsigned int, DocumentCollettion::fileInfo>::iterator iter = docInfo.begin();          //遍历所有文档，根据路径打开并分词
    while(iter != docInfo.end())
    {
        JiebaTest(iter.value().filePath.toStdString());
        iter++;
    }
}

