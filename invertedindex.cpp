#include "invertedindex.h"

void InvertedIndex::addToIndex(unsigned int docId, string word, WordsInfo docInfo)
{
    QMap<string, QMap<unsigned int, WordsInfo>>::Iterator iter;
    iter = this->indexMap.find(word);
    if(iter != this->indexMap.end())
    {
        // 单词已存在
        iter.value().insert(docId, docInfo);
    }
    else {
        // 新单词
        QMap<unsigned int, WordsInfo> tmp;
        tmp.insert(docId, docInfo);
        this->indexMap.insert(word, tmp);
    }
}

void InvertedIndex::addToIndex(unsigned int docId, string word, unsigned int count, vector<uint32_t> pos)
{
    WordsInfo docInfo;
    docInfo.count = count;
    docInfo.pos = pos;

    QMap<string, QMap<unsigned int, WordsInfo>>::Iterator iter;
    iter = this->indexMap.find(word);
    if(iter != this->indexMap.end())
    {
        // 单词已存在
        iter.value().insert(docId, docInfo);
    }
    else {
        // 新单词
        QMap<unsigned int, WordsInfo> tmp;
        tmp.insert(docId, docInfo);
        this->indexMap.insert(word, tmp);
    }
}

void InvertedIndex::show()
{
    QMap<string, QMap<unsigned int, WordsInfo>>::Iterator iter1;
    QMap<unsigned int, WordsInfo>::Iterator iter2;
    for (iter1 = this->indexMap.begin(); iter1 != this->indexMap.end(); iter1++) {
        qDebug() << iter1.key().c_str() << " -> ";    //单词
        for (iter2 = iter1.value().begin(); iter2 != iter1.value().end(); iter2++) {
            qDebug() << "<" << iter2.key() << "," << iter2.value().count << "," << iter2.value().pos << ">  " ;
        }
        qDebug() << endl;
    }
}
