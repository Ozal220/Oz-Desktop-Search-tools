#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include "documentcollection.h"
#include "docsegementation.h"
#include "invertedindex.h"
#include "addindexdialog.h"


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
    addIndexDialog *indexDialogPtr;
    void loadIndexUI();
    inline void addBox(QString str);

private slots:

    void on_searchButton_clicked();

    void on_addIndexButton_clicked();

    void receiveData(QString, QStringList, QString);

    void on_deleteIndexButton_clicked();

private:
    Ui::MainWindow *ui;
    void findFiles(QString path);
};

#endif // MAINWINDOW_H
