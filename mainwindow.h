#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include "documentcollection.h"
#include "docsegementation.h"
#include "invertedindex.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    DocCollettion *docCollectionPtr;
    DocSegmentation *docSegmentationPtr;
    InvertedIndex *indexPtr;

private slots:

    void on_choiceDirButton_clicked();

    void on_pushButton_clicked();

    void on_searchButton_clicked();

private:
    Ui::MainWindow *ui;
    void findFiles(QString path);
};

#endif // MAINWINDOW_H
