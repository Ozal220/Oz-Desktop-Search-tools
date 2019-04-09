#ifndef INVERTEDINDEX_H
#define INVERTEDINDEX_H

#include <vector>
#include <string>
#include <QMap>

using namespace std;

class InvertedIndex {

private:
    // 定义数据结构类型，保存单词出现次数和在文档中的位置。
    typedef struct wordsInfo{
        unsigned int count = 0;    // 单词出现次数
        vector<uint32_t> pos;   // 单词出现位置
    }WordsInfo;

    QMap<string, QMap<int, WordsInfo>> indexMap;      //文档最终的<单词表,(单词出现次数,<位置表>)>

public:
    InvertedIndex() = default;
    ~InvertedIndex();
    QMap<string, QMap<int, WordsInfo>> getIndexMap()
    {
        return indexMap;
    }

    bool addToIndex(string, QMap<int, WordsInfo>);
};

bool InvertedIndex::addToIndex(string word, QMap<int, WordsInfo> docInfo)
{
/*
    iter = this->indexMap.find(word.word);
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
 */
    QMap<string, QMap<int, WordsInfo>>::Iterator iter;
    iter = this->indexMap.find(word);
    if(iter != this->indexMap.end())
    {
        // 单词已存在
        iter
    }
    else {
        // 新单词
        this->indexMap.insert(word, docInfo);
    }
}

#endif // INVERTEDINDEX_H
