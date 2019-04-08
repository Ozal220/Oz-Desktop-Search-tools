#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include "documentcollection.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    DocumentCollettion *docControlPtr;

private slots:

    void on_choiceDirButton_clicked();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    void findFiles(QString path);
};

#endif // MAINWINDOW_H
