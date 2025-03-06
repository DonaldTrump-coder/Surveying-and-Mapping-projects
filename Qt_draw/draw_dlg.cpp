#include "draw_dlg.h"
#include "./ui_draw_dlg.h"
#include "canvas.h"
#include <QDialog>

input a;//建立一个全局对象变量a，已在头文件中声明
//该对话框和绘图对话框中都可访问到a

draw_dlg::draw_dlg(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::draw_dlg)
{
    ui->setupUi(this);
}

draw_dlg::~draw_dlg()
{
    delete ui;
}

void draw_dlg::on_pushButton_clicked()
{
    //通过读取文件的函数将文件内容输入进输入文本框中
    ui->textEdit->setText(a.getfile());
}

void draw_dlg::on_pushButton_2_clicked()
{
    //对文本框中的数据进行处理
    a.init(ui->textEdit->toPlainText());

    //若输入格式有问题，则停止后续的绘图程序
    if(a.test()==1)
        return;

    //建立绘图对话框的对象
    //打开用于绘图的canvas对话框
    canvas* b=new canvas;
    b->show();
    b->setAttribute(Qt::WA_DeleteOnClose);
}

void draw_dlg::on_pushButton_3_clicked()
{
    //清空输入文本框
    ui->textEdit->clear();
}
