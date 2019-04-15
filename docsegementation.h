#ifndef DOCSEGEMENTATION_H
#define DOCSEGEMENTATION_H
#include "cppjieba/include/cppjieba/Jieba.hpp"
#include <vector>
#include <string>
#include <QMap>
#include <QDebug>

using namespace std;

class DocSegmentation {

public:
    // 定义新的数据类型，保存单词出现次数和在文档中的位置。对应与Qmap中的value值
    class WordsInfo {
    public:
        unsigned int count = 0;    // 单词出现次数
        vector<uint32_t> pos;   // 单词出现位置
        WordsInfo() = default;
        WordsInfo(unsigned int c, vector<uint32_t> p)
        {
            this->count = c;
            this->pos = p;
        }
        WordsInfo(unsigned int c, uint32_t p)
        {
            this->count = c;
            this->pos.push_back(p);
        }
        ~WordsInfo()
        {
            this->count = 0;
            this->pos.clear();
        }
    };

    DocSegmentation() = default;
    ~DocSegmentation();

    int useJieba(string);

    void clearWordsMap()
    {
        wordsMap.clear();
    }

    QMap<string, WordsInfo> &getWordsMap()
    {
        return wordsMap;
    }

    void showMap();
private:
    QMap<string, WordsInfo> wordsMap;      //文档最终的<单词表,(单词出现次数,<位置表>)>
};

#endif // DOCSEGEMENTATION_H
