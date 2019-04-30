#include <docsegementation.h>
#include <QSet>

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

    //加载停用词
    set<string> stop_words;
    stop_words.insert(" ");
    QString stopWord;
    QFile stop_words_file("./../Oz-DesktopSearch/cppjieba/dict/my_stop_words.utf8");
    if(stop_words_file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&stop_words_file);
        while (!in.atEnd()) {
            in >> stopWord;
            stop_words.insert(stopWord.toStdString());
        }
    }

    WordsInfo *wordInfoPtr;
    QMap<string, WordsInfo>::Iterator iter;

    ifstream ifs(file_path);
    if(!ifs.is_open())       
        return -1;

//    int i = 1;
    while(!ifs.eof())
    {
        ifs.getline(buffer, 512);
        s = buffer;
        jieba.CutForSearch(s, words_with_offset);       //分词
//        qDebug() << i++ << " " << buffer << QString::fromStdString(s);
        for(auto word : words_with_offset)
        {

            //跳过停用词
            if(stop_words.find(word.word) != stop_words.end())
            {
//                qDebug() << "是停用词" << QString::fromStdString(word.word);
                continue;
            }

            iter = wordsMap.find(word.word);
            if(iter != wordsMap.end()){
                // 单词已存在
                //qDebug() << "单词已存在:" << word.word.c_str();
                iter->count += 1;               //出现次数+1
                iter->pos.push_back(word.unicode_offset);   //添加偏移量pos
            }
            else {
                // 新单词
                //qDebug() << "新单词:" << word.word.c_str();
                wordInfoPtr = new WordsInfo(1, word.unicode_offset);
                wordsMap.insert(word.word, *wordInfoPtr);
            }
        }
    }
    return 0;
}

void DocSegmentation::showMap()
{
    QDebug debug = qDebug().nospace();

    if(wordsMap.isEmpty()){
        debug << "wordsMap is empty";
        return;
    }
//    cout << "wordsMap size: " << wordsMap.size() << endl;
    QMap<string, WordsInfo>::Iterator iter = wordsMap.begin();
    while(iter != wordsMap.end())
    {
        debug << "<" << iter.key().c_str() << "," << iter.value().count << "," << iter.value().pos << ">,";
        iter++;
    }
    debug << endl;
}
