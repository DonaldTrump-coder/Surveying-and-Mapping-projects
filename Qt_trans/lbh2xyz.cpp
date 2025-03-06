#include "lbh2xyz.h"
#include "ui_lbh2xyz.h"

//输入的LBH坐标中，L和B坐标是以度分秒表示的，需要先将其转化为以度表示的角度数值才便于处理和计算
//这里先定义一个将度分秒角度数值转换为以度为单位的角度的函数

//具体的转换方式取决于输入的形式，这里的输入是double类型，整数位表示度，小数位前两位表示分，第三、四为表示秒的整数部分，
//最后两位表示秒的小数部分
double tohour(double a)
{
    double hour=int(a);//将度部分的数值取出
    double min=int((a-hour)*100);//将小数中的“分”部分的数值取出
    double sec=((a-hour)*100-min)*100;//将小数中的“秒”部分的数值取出

    //将分和秒都划归至度的小数部分中
    min=min/60.0;
    sec=sec/3600.0;

    //将以度为单位的度、分、秒部分的数值相加，得到最终以度为单位的角度值
    return hour+min+sec;
}

//类构造函数，将输入的LBH坐标值传入类中，用于后续的计算处理
calculation::calculation(double l,double b,double h)
{
    //使用定义的转换函数将L和B角度值转换为以度为单位的角度值，再将其转换为弧度值
    L=(tohour(l)*pie)/180;
    B=(tohour(b)*pie)/180;
    H=h;
    N=a/(sqrt(1-e2*sin(B)*sin(B)));//传入参数时N的值已可确定为具体值，因此在构造函数中就可先将其计算好
}

//实现计算功能的函数，也是实现主要功能的函数
void calculation::calculate()
{
    //根据公式对X、Y、Z坐标值进行计算，调用math.h头文件中的计算函数
    X=(N+H)*cos(B)*cos(L);
    Y=(N+H)*cos(B)*sin(L);
    Z=(N*(1-e2)+H)*sin(B);
}

//为实现较好的封装性和安全性，使用函数用于返回输出计算的结果坐标值
const double calculation::showX()
{
    return X;
}
const double calculation::showY()
{
    return Y;
}
const double calculation::showZ()
{
    return Z;
}

LBH2XYZ::LBH2XYZ(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::LBH2XYZ)
{
    ui->setupUi(this);
}

LBH2XYZ::~LBH2XYZ()
{
    delete ui;
}

//计算按钮按下时的计算程序
void LBH2XYZ::on_pushButton_clicked()
{
    //建立对象用于实现计算，同时从输入文本编辑框传入计算所使用的L、B、H坐标参数
    calculation d(ui->L->text().toDouble(),ui->B->text().toDouble(),ui->H->text().toDouble());

    //调用计算函数，将对象中的X、Y、Z值计算出来
    d.calculate();

    //根据输出格式，将X、Y、Z以浮点数形式在TextBrowser中输出
    ui->X->setText(QString::number(d.showX(),'f',3));
    ui->Y->setText(QString::number(d.showY(),'f',3));
    ui->Z->setText(QString::number(d.showZ(),'f',3));
}

//“重新输入”按钮按下时的清除程序，将所有的文本框清空
void LBH2XYZ::on_pushButton_2_clicked()//清空文本框，用于重新输入数据
{
    ui->X->clear();
    ui->Y->clear();
    ui->Z->clear();
    ui->B->clear();
    ui->L->clear();
    ui->H->clear();
}
