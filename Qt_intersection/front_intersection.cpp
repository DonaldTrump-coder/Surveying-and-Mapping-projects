#include "front_intersection.h"
#include "ui_front_intersection.h"
#include "func.h"//引用自定义函数头文件，用于处理数据

front_intersection::front_intersection(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::front_intersection)
{
    ui->setupUi(this);
}

front_intersection::~front_intersection()
{
    delete ui;
}

//定义用于传参的构造函数
calculation::calculation(double XA,double YA,double XB,double YB,double ALPHA,double BETA)
{
    xa=XA;
    ya=YA;
    xb=XB;
    yb=YB;
    alpha=torad(ALPHA);
    beta=torad(BETA);
    //将输入的度分秒表示的角度在初始化时就转为弧度的形式并存储
}

//用于计算的函数，是实现主要功能的部分
void calculation::calculate()
{
    test();//计算前先检查输入数据是否格式正确
    if(wrong==1)
    {
        //若格式不对，则直接弹出警告框并结束计算程序
        //无需再进行后续计算
        warn();
        return;
    }

    //格式正确时，才进行目标点坐标的计算
    xp=(xa*cot(beta)+xb*cot(alpha)+yb-ya)/(cot(alpha)+cot(beta));
    yp=(ya*cot(beta)+yb*cot(alpha)+xa-xb)/(cot(alpha)+cot(beta));
    //前方交会公式（逆时针）
}

//为加强程序的封装性与安全性，用专门的函数来返回目标点的坐标
//且返回值的类型为const double，无法被修改
const double calculation::showx()
{
    return xp;
}

const double calculation::showy()
{
    return yp;
}

//检查输入格式是否有误的函数
void calculation::test()
{
    if(alpha<=0||beta<=0)
        wrong=1;
    //若输入的角度为负数，则视为有误
}

//用于弹出警告框的函数
void calculation::warn()
{
    QMessageBox a;//建立消息框对象
    a.setText("请输入正确的角度值");
    //在前方交会中，只会存在输入角度值不正确的情况，因此警告框应提示输入正确的角度值

    //弹出警告框
    a.exec();

    //弹出警告框后无需进行计算，因此无目标点坐标的结果，需要将其置为默认值0
    xp=0;
    yp=0;
}

//用于计算并输出结果的函数
void front_intersection::on_pushButton_clicked()
{
    //在构造函数中用输入数据进行参数的初始化
    calculation a(ui->XA->text().toDouble(),ui->YA->text().toDouble(),ui->XB->text().toDouble(),ui->YB->text().toDouble(),ui->alpha->text().toDouble(),ui->beta->text().toDouble());

    //调用该对象的计算函数，计算结果
    a.calculate();

    //结合对象的坐标返回函数进行目标点坐标的输出，结果保留三位小数
    ui->PX->setText(QString::number(a.showx(),'f',3));
    ui->PY->setText(QString::number(a.showy(),'f',3));
}

//清空所有输入、输出框的内容，以便重新输入
void front_intersection::on_pushButton_2_clicked()
{
    ui->PX->clear();
    ui->PY->clear();
    ui->XA->clear();
    ui->XB->clear();
    ui->YA->clear();
    ui->YB->clear();
    ui->alpha->clear();
    ui->beta->clear();
}

