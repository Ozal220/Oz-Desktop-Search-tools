#ifndef INVERTEDINDEX_H
#define INVERTEDINDEX_H

#include <vector>
#include <string>
#include <QMap>
#include <QDebug>
#include <fstream>
#include <sstream>

using namespace std;

class InvertedIndex {

private:
    // 定义数据结构类型，保存单词出现次数和在文档中的位置。
    typedef struct wordsInfo{
        unsigned int count = 0;    // 单词出现次数
        vector<uint32_t> pos;   // 单词出现位置
    }WordsInfo;

    QMap<string, QMap<unsigned int, WordsInfo>> indexMap;      //文档最终的<单词表,<docId,(单词出现次数,<位置表>)>>
    string savedPath = "./../Index";                            //索引保存位置

public:
    InvertedIndex() = default;
    ~InvertedIndex();
    QMap<string, QMap<unsigned int, WordsInfo>> getIndexMap()
    {
        return indexMap;
    }

    void addToIndex(unsigned int docId, string, WordsInfo);
    void addToIndex(unsigned int docId, string, unsigned int, vector<uint32_t>);    // 将<单词-文档信息>添加到索引
    bool saveOnFile();
    bool loadIndex(string);
    void show();
};

#endif // INVERTEDINDEX_H