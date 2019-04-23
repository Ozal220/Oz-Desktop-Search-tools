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
    QDebug debug = qDebug().nospace();

    QMap<string, QMap<unsigned int, WordsInfo>>::Iterator iter1;
    QMap<unsigned int, WordsInfo>::Iterator iter2;
    for (iter1 = this->indexMap.begin(); iter1 != this->indexMap.end(); iter1++) {
        debug << iter1.key().c_str() << ": ";    //单词
        for (iter2 = iter1.value().begin(); iter2 != iter1.value().end(); iter2++) {
            debug << "<" << iter2.key() << "," << iter2.value().count << "," << iter2.value().pos << ">, " ;
        }
        debug << endl;
    }

}

bool InvertedIndex::saveOnFile()
{
    ofstream file(this->savedPath);
    if(!file.is_open())
    {
        qDebug("index file open failed");
        return false;
    }
    else {
        QMap<string, QMap<unsigned int, WordsInfo>>::Iterator it1 = indexMap.begin();
        while (it1 != indexMap.end()) {
            file << it1.key() << " ";         //单词string
            QMap<unsigned int, WordsInfo>::Iterator it2 = it1.value().begin(); //文档信息
            while(it2 != it1.value().end()) {
                file << it2.key() << " " << it2.value().count << " ";      //文档编号和单词出现次数
                for(auto i : it2.value().pos)
                    file << i << " ";           //出现位置的偏移量
                it2++;
            }
            file << endl;
            it1++;
        }
        qDebug("索引已读入");
    }
    return true;
}

bool InvertedIndex::loadIndex(string path)
{
    indexMap.clear();
    ifstream file(path);
    if(!file.is_open())
    {
        qDebug("file open failed");
        return false;
    }
    else {
        string word;
        unsigned int id;
        unsigned int count;
        vector<uint32_t> pos;
        wordsInfo w;
        QMap<unsigned int, WordsInfo> doc;

        for(string line; getline(file, line); )
        {
            istringstream sstream(line);
            sstream >> word;                        //第一个参数是单词
            while (sstream >> id) {              //单词之后的剩余信息.第一个是docId，然后是count，再是pos
                sstream >> count;
                w.count = count;                //count需要插入到wordsInfo里
                while (count--) {                   //读位置
                    uint32_t n;
                    sstream >> n;
                    pos.push_back(n);
                }
                w.pos = pos;
                doc.insert(id, w);
                pos.clear();
            }
            indexMap.insert(word, doc);
            doc.clear();
            sstream.clear();
        }
    }
    return true;
}
