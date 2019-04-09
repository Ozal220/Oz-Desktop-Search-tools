#ifndef DOCSEGEMENTATION_H
#define DOCSEGEMENTATION_H
#include "cppjieba/include/cppjieba/Jieba.hpp"
#include <vector>
#include <string>
#include <QMap>
#include <QDebug>

using namespace std;

class DocSegmentation {

private:
    // 定义新的数据类型，保存单词出现次数和在文档中的位置。对应与Qmap中的value值
    typedef struct wordsInfo{
        unsigned int count = 0;    // 单词出现次数
        vector<uint32_t> pos;   // 单词出现位置
    }WordsInfo;

    QMap<string, WordsInfo> wordsMap;      //文档最终的<单词表,(单词出现次数,<位置表>)>

public:
    DocSegmentation() = default;
    ~DocSegmentation();

    int useJieba(string);
    QMap<string, WordsInfo> getWordsMap()
    {
        return wordsMap;
    }

};


int DocSegmentation::useJieba(string file_path) {
    // 字典位置
    const string DICT_PATH = "./../Oz-DesktopSearch/cppjieba/dict/jieba.dict.utf8";
    const string HMM_PATH = "./../Oz-DesktopSearch/cppjieba/dict/hmm_model.utf8";
    const string USER_DICT_PATH = "./../Oz-DesktopSearch/cppjieba/dict/user.dict.utf8";
    const string IDF_PATH = "./../Oz-DesktopSearch/cppjieba/dict/idf.utf8";
    const string STOP_WORD_PATH = "./../Oz-DesktopSearch/cppjieba/dict/stop_words.utf8";

    // 实例化jieba类
    cppjieba::Jieba jieba(DICT_PATH,
        HMM_PATH,
        USER_DICT_PATH,
        IDF_PATH,
        STOP_WORD_PATH);

    string s;
    char buffer[512];           //jieba分词buffer值暂设为512
    vector<cppjieba::Word> words_with_offset;

    WordsInfo word_info;

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
        jieba.CutForSearch(s, words_with_offset);       //分词
        for(auto word : words_with_offset)
        {
            iter = this->getWordsMap().find(word.word);
            if(iter != this->getWordsMap().end()){
                // 单词已存在
                iter->count += 1;               //出现次数+1
                iter->pos.push_back(word.unicode_offset);   //添加偏移量pos
            }
            else {
                // 新单词
                word_info.count = 1;
                word_info.pos.push_back(word.unicode_offset);
                this->getWordsMap().insert(word.word, word_info);
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


#endif // DOCSEGEMENTATION_H
