#ifndef ADDINDEXDIALOG_H
#define ADDINDEXDIALOG_H

#include <QDialog>
#include <QFileDialog>
#include "qprogressindicator.h"

namespace Ui {
class addIndexDialog;
}

class addIndexDialog : public QDialog
{
    Q_OBJECT

public:
    explicit addIndexDialog(QWidget *parent = nullptr);
    QString choicePath;                                 //路径
    QStringList filter = {"*.txt", "*.html", "*.htm"};  //先放着
    QString indexName;                                  //索引名称
    ~addIndexDialog();

private slots:
    void on_choiceDir_clicked();

    void on_cancelButton_clicked();

    void on_OkButton_clicked();

signals:
    void sendData(QString, QStringList, QString);

private:
    Ui::addIndexDialog *ui;
};

#endif // ADDINDEXDIALOG_H
