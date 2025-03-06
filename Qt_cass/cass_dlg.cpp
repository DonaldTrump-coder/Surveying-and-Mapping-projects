#include "cass_dlg.h"
#include "./ui_cass_dlg.h"
#include "sort_cass.h"
#include <QFileDialog>
#include "func.h"

//定义一个枚举类型，用于指定对文件的读或写操作
enum file
{
    Read,Write
};

//定义一个返回指定文件路径的函数
//通过选择文件的对话框
//分别得到读或写文件的路径
QString getpath(file method)
{
    QFileDialog a;//创建QFileDialog对象

    if(method==Read)
    {
        //读文件模式
        return a.getOpenFileName(nullptr,
                                 "打开文件",//对话框的标题
                                 "",
                                 "Text(*.dat *.txt)");
        //最后一个参数指定能选择打开的文件类型：仅限.dat和.txt文件
    }

    if(method==Write)
    {
        //写文件模式
        return a.getSaveFileName(nullptr,
                                 "保存文件",//对话框的标题
                                 "",
                                 "Text(*.dat)");
        //指定保存文件的类型：仅限.dat文件
    }
}

cass_dlg::cass_dlg(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::cass_dlg)
{
    ui->setupUi(this);
}

cass_dlg::~cass_dlg()
{
    delete ui;
}

//“整理”按钮的响应函数，用于读入数据，排序并显示结果
void cass_dlg::on_pushButton_2_clicked()
{
    //读入、解译输入文本框中的数据至内存中
    sort_cass a(ui->textEdit->toPlainText());

    //若读入的数据有格式问题，则无需再进行后续的排序过程
    if(a.test()==1)
        return;

    //排序
    a.sort();

    //输出
    ui->textBrowser->setText(a.output());
}

//“选取文件”按钮的响应函数：用于选取并读取指定文件的内容，显示在输入文本框中
void cass_dlg::on_pushButton_1_clicked()
{
    //将需要读入的地址存入QFile类型变量中
    QFile file(getpath(Read));

    //以读方式打开文本文件
    bool ret=file.open(QIODevice::ReadOnly|QIODevice::Text);

    //若打开文件失败，需要结束该过程，无需再将文件内容读入至内存中
    if(ret==0)
    {
        warn("打开文件失败！");
        return;
    }

    //将文件的所有内容读至text字符串变量中
    QString text=file.readAll();

    //在输入文本框中显示所有读入的内容
    ui->textEdit->setPlainText(text);
    file.close();//关闭文件

    //整个读文件过程结束后弹出结束提示
    warn("文件已打开，请检查无误后点击整理按钮");
}

//“重新输入”按钮的响应函数
void cass_dlg::on_pushButton_3_clicked()
{
    //清空所有的输入、输出文本框用以重新输入
    ui->textBrowser->clear();
    ui->textEdit->clear();
}

//“保存文件”按钮的响应函数，将输出内容保存至指定文件中
void cass_dlg::on_pushButton_4_clicked()
{
    //将指定要保存的地址存至QFile类型变量中
    QFile file(getpath(Write));

    //以只写方式打开文本文件
    bool ret=file.open(QIODevice::WriteOnly|QIODevice::Text);

    //若打开文件失败，则结束该过程，无需进行后续写文件的步骤
    if(ret==0)
    {
        warn("保存文件失败");
        return;
    }

    QTextStream stream(&file);
    stream<<ui->textBrowser->toPlainText();
    //将输出文本框中的内容写入文件中

    file.close();//关闭文件

    //写文件过程结束后弹出结束提示
    warn("数据保存成功");
}
