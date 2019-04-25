#include "addindexdialog.h"
#include "ui_addindexdialog.h"

addIndexDialog::addIndexDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addIndexDialog)
{
    ui->setupUi(this);
    ui->waitLabel->setHidden(true);
}

addIndexDialog::~addIndexDialog()
{
    delete ui;
}

void addIndexDialog::on_choiceDir_clicked()
{
    ui->Path->setText(QFileDialog::getExistingDirectory(this, tr("选择文件夹"), "./../Test")); //选择路径
}

//确认按钮
void addIndexDialog::on_OkButton_clicked()
{
    ui->waitLabel->setHidden(false);
    emit sendData(ui->Path->text(), filter, ui->indexName->text());
    close();
}

//取消按钮
void addIndexDialog::on_cancelButton_clicked()
{
    this->close();
}
