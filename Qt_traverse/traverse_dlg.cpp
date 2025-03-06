#include "traverse_dlg.h"
#include "./ui_traverse_dlg.h"
#include "calculation.h"

traverse_dlg::traverse_dlg(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::traverse_dlg)
{
    ui->setupUi(this);
}

traverse_dlg::~traverse_dlg()
{
    delete ui;
}

//进行读数据并计算的响应函数
void traverse_dlg::on_pushButton_clicked()
{
    //将输入文本框的文本数据读入对象
    calculation a(ui->textEdit->toPlainText());

    //若输入数据有误，就无需再进行后续的计算了
    if(a.test()==1)
    {
        return;
    }

    //估算方位角和坐标
    a.estimation();

    //用用户输入或默认的闭合差限差参与坐标和方位角改正并输出结果
    //用户勾选复选框后可以使用其输入的闭合差限差
    if(ui->checkBox->isChecked())
    {
        if(a.correction(ui->lineEdit->text().toDouble())==-1)
        {
            //闭合差超出限差
            ui->textBrowser->setText(a.output(1));
        }
        else
        {
            //闭合差未超出限差
            ui->textBrowser->setText(a.output(0));
        }
    }
    else
    {
        if(a.correction(10000)==-1)
        {
            ui->textBrowser->setText(a.output(1));
        }
        else
        {
            ui->textBrowser->setText(a.output(0));
        }
    }
}

//用于读文件的函数
void traverse_dlg::on_pushButton_2_clicked()
{
    //显示读文件得到的内容
    ui->textEdit->setText(file_operation(read));
}

//写文件的函数
void traverse_dlg::on_pushButton_4_clicked()
{
    //将输出文本框中的内容写入指定文件中
    file_operation(write,ui->textBrowser->toPlainText());
}

//清空输入、输出文本框
void traverse_dlg::on_pushButton_3_clicked()
{
    ui->textEdit->clear();
    ui->textBrowser->clear();
}
