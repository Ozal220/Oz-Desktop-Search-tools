#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.loadIndexUI();            //加载索引并修改索引界面
    w.show();

    return a.exec();
}
