#-------------------------------------------------
#
# Project created by QtCreator 2019-03-21T10:18:55
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Oz-DesktopSearch
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    docsegmentation.cpp \
    documentcollection.cpp \
    invertedindex.cpp \
    addindexdialog.cpp

HEADERS += \
        mainwindow.h \
    include/DictTrie.hpp \
    include/FullSegment.hpp \
    include/HMMModel.hpp \
    include/HMMSegment.hpp \
    include/ISegment.hpp \
    include/KeywordExtractor.hpp \
    include/MixSegment.hpp \
    include/MPSegment.hpp \
    include/PosTagger.hpp \
    include/QuerySegment.hpp \
    include/SegmentBase.hpp \
    include/TransCode.hpp \
    include/Trie.hpp \
    cppjieba/include/cppjieba/DictTrie.hpp \
    cppjieba/include/cppjieba/FullSegment.hpp \
    cppjieba/include/cppjieba/HMMModel.hpp \
    cppjieba/include/cppjieba/HMMSegment.hpp \
    cppjieba/include/cppjieba/Jieba.hpp \
    cppjieba/include/cppjieba/KeywordExtractor.hpp \
    cppjieba/include/cppjieba/MixSegment.hpp \
    cppjieba/include/cppjieba/MPSegment.hpp \
    cppjieba/include/cppjieba/PosTagger.hpp \
    cppjieba/include/cppjieba/PreFilter.hpp \
    cppjieba/include/cppjieba/QuerySegment.hpp \
    cppjieba/include/cppjieba/SegmentBase.hpp \
    cppjieba/include/cppjieba/SegmentTagged.hpp \
    cppjieba/include/cppjieba/TextRankExtractor.hpp \
    cppjieba/include/cppjieba/Trie.hpp \
    cppjieba/include/cppjieba/Unicode.hpp \
    cppjieba/include/cppjieba/limonp/Config.hpp \
    cppjieba/include/cppjieba/limonp/HandyMacro.hpp \
    cppjieba/include/cppjieba/limonp/InitOnOff.hpp \
    cppjieba/include/cppjieba/limonp/LocalVector.hpp \
    cppjieba/include/cppjieba/limonp/Logger.hpp \
    cppjieba/include/cppjieba/limonp/MysqlClient.hpp \
    cppjieba/include/cppjieba/limonp/NonCopyable.hpp \
    cppjieba/include/cppjieba/limonp/StdExtension.hpp \
    cppjieba/include/cppjieba/limonp/StringUtil.hpp \
    cppjieba/include/cppjieba/limonp/ArgvContext.hpp \
    cppjieba/include/cppjieba/limonp/BlockingQueue.hpp \
    cppjieba/include/cppjieba/limonp/BoundedBlockingQueue.hpp \
    cppjieba/include/cppjieba/limonp/BoundedQueue.hpp \
    cppjieba/include/cppjieba/limonp/Closure.hpp \
    cppjieba/include/cppjieba/limonp/Colors.hpp \
    cppjieba/include/cppjieba/limonp/Condition.hpp \
    cppjieba/include/cppjieba/limonp/Config.hpp \
    cppjieba/include/cppjieba/limonp/FileLock.hpp \
    cppjieba/include/cppjieba/limonp/ForcePublic.hpp \
    cppjieba/include/cppjieba/limonp/LocalVector.hpp \
    cppjieba/include/cppjieba/limonp/Logger.hpp \
    cppjieba/include/cppjieba/limonp/Logging.hpp \
    cppjieba/include/cppjieba/limonp/Md5.hpp \
    cppjieba/include/cppjieba/limonp/MutexLock.hpp \
    cppjieba/include/cppjieba/limonp/NonCopyable.hpp \
    cppjieba/include/cppjieba/limonp/StdExtension.hpp \
    cppjieba/include/cppjieba/limonp/StringUtil.hpp \
    cppjieba/include/cppjieba/limonp/Thread.hpp \
    cppjieba/include/cppjieba/limonp/ThreadPool.hpp \
    documentcollection.h \
    docsegementation.h \
    invertedindex.h \
    addindexdialog.h

FORMS += \
        mainwindow.ui \
    addindexdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
