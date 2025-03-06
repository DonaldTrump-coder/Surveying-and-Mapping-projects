#include "quad_dlg.h"
#include "./ui_quad_dlg.h"
#include "func.h"

quadList d;//对话框打开时建立大地四边形链表的全局对象

quad_dlg::quad_dlg(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::quad_dlg)
{
    ui->setupUi(this);
}

quad_dlg::~quad_dlg()
{
    delete ui;
}

//单个大地四边形的直接计算
void quad_dlg::on_pushButton_2_clicked()
{
    //检查输入数据是否完整，若不完整则停止计算，继续等待输入（不清空输入框）
    if(ui->XA->text().isEmpty()||ui->YA->text().isEmpty()||
        ui->XB->text().isEmpty()||ui->YB->text().isEmpty()||
        ui->A1->text().isEmpty()||ui->A2->text().isEmpty()||
        ui->A3->text().isEmpty()||ui->A4->text().isEmpty()||
        ui->A5->text().isEmpty()||ui->A6->text().isEmpty()||
        ui->A7->text().isEmpty()||ui->A8->text().isEmpty())
    {
        warn("请输入完整的已知参数");
        return;
    }

    //检查输入已知角度是否格式正确
    //若输入的已知角度值存在负数，则说明输入数据有误，停止计算，重新输入
    if(ui->A1->text().toDouble()<=0||ui->A2->text().toDouble()<=0
        ||ui->A3->text().toDouble()<=0||ui->A4->text().toDouble()<=0
        ||ui->A5->text().toDouble()<=0||ui->A6->text().toDouble()<=0
        ||ui->A7->text().toDouble()<=0||ui->A8->text().toDouble()<=0)
    {
        warn("请输入正确的角度值");
        quad_dlg::on_pushButton_3_clicked();//调用清空按钮响应函数进行输入文本框清空
        return;
    }

    //建立单个大地四边形对象，传入输入文本框的已知参数
    quad a(ui->XA->text().toDouble(),ui->YA->text().toDouble(),
           ui->XB->text().toDouble(),ui->YB->text().toDouble(),
           ui->A1->text().toDouble(),ui->A2->text().toDouble(),
           ui->A3->text().toDouble(),ui->A4->text().toDouble(),
           ui->A5->text().toDouble(),ui->A6->text().toDouble(),
           ui->A7->text().toDouble(),ui->A8->text().toDouble());

    //进行大地四边形未知参数的计算
    a.calculate();

    //进行目标参数的输出
    ui->XC->setText(QString::number(a.showxc(),'f',6));
    ui->YC->setText(QString::number(a.showyc(),'f',6));
    ui->XD->setText(QString::number(a.showxd(),'f',6));
    ui->YD->setText(QString::number(a.showyd(),'f',6));

    //showw()函数返回的是数组首地址，可通过下标引用方式引用各闭合差的值
    ui->W1->setText(QString::number(a.showw()[1],'f',2));
    ui->W2->setText(QString::number(a.showw()[2],'f',2));
    ui->W3->setText(QString::number(a.showw()[3],'f',2));
    ui->WD->setText(QString::number(a.showw()[0],'f',6));
}

//清空所有输入、输出文本框，便于重新输入
void quad_dlg::on_pushButton_3_clicked()
{
    ui->XA->clear();
    ui->YA->clear();
    ui->XB->clear();
    ui->YB->clear();
    ui->A1->clear();
    ui->A2->clear();
    ui->A3->clear();
    ui->A4->clear();
    ui->A5->clear();
    ui->A6->clear();
    ui->A7->clear();
    ui->A8->clear();
    ui->XC->clear();
    ui->YC->clear();
    ui->XD->clear();
    ui->YD->clear();
    ui->W1->clear();
    ui->W2->clear();
    ui->W3->clear();
    ui->WD->clear();
}

//用于存储大地四边形的按钮，在内存中实现多个大地四边形的数据存储
void quad_dlg::on_pushButton_clicked()
{
    //检查输入数据是否完整
    if(ui->XA->text().isEmpty()||ui->YA->text().isEmpty()||
        ui->XB->text().isEmpty()||ui->YB->text().isEmpty()||
        ui->A1->text().isEmpty()||ui->A2->text().isEmpty()||
        ui->A3->text().isEmpty()||ui->A4->text().isEmpty()||
        ui->A5->text().isEmpty()||ui->A6->text().isEmpty()||
        ui->A7->text().isEmpty()||ui->A8->text().isEmpty())
    {
        warn("请输入完整的已知参数");
        return;
    }

    //检查输入角度是否为正
    if(ui->A1->text().toDouble()<=0||ui->A2->text().toDouble()<=0
        ||ui->A3->text().toDouble()<=0||ui->A4->text().toDouble()<=0
        ||ui->A5->text().toDouble()<=0||ui->A6->text().toDouble()<=0
        ||ui->A7->text().toDouble()<=0||ui->A8->text().toDouble()<=0)
    {
        warn("请输入正确的角度值");
        quad_dlg::on_pushButton_3_clicked();
        return;
    }

    //将已知数据传入大地四边形链表结点
    d.append(ui->XA->text().toDouble(),ui->YA->text().toDouble(),
             ui->XB->text().toDouble(),ui->YB->text().toDouble(),
             ui->A1->text().toDouble(),ui->A2->text().toDouble(),
             ui->A3->text().toDouble(),ui->A4->text().toDouble(),
             ui->A5->text().toDouble(),ui->A6->text().toDouble(),
             ui->A7->text().toDouble(),ui->A8->text().toDouble());

    //每存储一个大地四边形数据后都清空文本框，便于输入下一组数据
    quad_dlg::on_pushButton_3_clicked();

    //存储第n个大地四边形数据后给出提示
    warn(QString("当前已输入第%1个大地四边形").arg(d.showNum()));
    //此处警告函数的调用作用不是警告，而是提示
}

//用于计算以存储的第n个大地四边形的相关数据
//n由另一个输入文本框指定
void quad_dlg::on_pushButton_4_clicked()
{
    //当需要计算的大地四边形序号超过了当前存储的最大值，则无法计算，需要弹出警告
    if(ui->N->text().toInt()>d.showNum())
    {
        warn("没有这么多的大地四边形！");
        ui->N->clear();//重新输入序号
        return;
    }

    //当没有输入大地四边形序号时，也无法计算，需要弹出警告
    if(ui->N->text().isEmpty())
    {
        warn("请输入要计算的大地四边形序号");
        return;
    }

    //根据输入的序号找到需要访问的大地四边形结点对象
    quadNode* a=d.find(ui->N->text().toInt());

    //对该对象进行参数计算
    a->calculate();

    //输出该对象的目标参数
    ui->XC->setText(QString::number(a->showxc(),'f',6));
    ui->YC->setText(QString::number(a->showyc(),'f',6));
    ui->XD->setText(QString::number(a->showxd(),'f',6));
    ui->YD->setText(QString::number(a->showyd(),'f',6));
    //角度闭合差以秒为单位，保留两位小数
    ui->W1->setText(QString::number(a->showw()[1],'f',2));
    ui->W2->setText(QString::number(a->showw()[2],'f',2));
    ui->W3->setText(QString::number(a->showw()[3],'f',2));
    //极条件闭合差保留两位小数
    ui->WD->setText(QString::number(a->showw()[0],'f',6));

    //当输出指定序号的大地四边形目标数据时，还需要对该大地四边形的已知数据进行恢复，便于查看
    ui->XA->setText(QString::number(a->showxa()));
    ui->YA->setText(QString::number(a->showya()));
    ui->XB->setText(QString::number(a->showxb()));
    ui->YB->setText(QString::number(a->showyb()));
    //已知坐标数据存储时没变，可直接输出

    //已知角度以弧度制存储，恢复数据时需转成度分秒的格式，因此调用预定义好的转换函数
    ui->A1->setText(QString::number(todegminsec(a->showa1()),'f',4));
    ui->A2->setText(QString::number(todegminsec(a->showa2()),'f',4));
    ui->A3->setText(QString::number(todegminsec(a->showa3()),'f',4));
    ui->A4->setText(QString::number(todegminsec(a->showa4()),'f',4));
    ui->A5->setText(QString::number(todegminsec(a->showa5()),'f',4));
    ui->A6->setText(QString::number(todegminsec(a->showa6()),'f',4));
    ui->A7->setText(QString::number(todegminsec(a->showa7()),'f',4));
    ui->A8->setText(QString::number(todegminsec(a->showa8()),'f',4));
}
