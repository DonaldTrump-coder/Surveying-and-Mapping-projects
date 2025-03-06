#include "xyz2lbh.h"
#include "ui_xyz2lbh.h"

//根据输出时的格式进行角度转换
//参数a是以度为单位的角度值
double tohour_min_sec(double a)
{
    double hour=int(a);//将角度的整数值取出，作为度的部分
    double min=int((a-hour)*60);//取出单位为“分”且是整数的部分
    double sec=((a-hour)*60-min)*60;//将剩余部分转换以“秒”为单位的数值

    //输出的时候要以double的形式输出，因此需要将分、秒的部分移至小数部分但不改变数值
    min=min/100.0;
    sec=sec/10000.0;
    return hour+min+sec;
}
//将以度为单位的角度转化为以度分秒为单位的角

//类构造函数，将X，Y，Z坐标值传入类中，用于后续计算
//XYZ坐标可直接带入计算公式，无需使用任何转换的程序
calculation2::calculation2(double x,double y,double z)
{
    X=x;
    Y=y;
    Z=z; 
}

//用于实现计算的函数，也是实现功能的主要函数
void calculation2::calculate()
{
    //将表征H精度的————此次计算H值与上一次迭代计算的H值之差：dH————初始化为0
    //但dH=0时进入循环，会直接结束循环，因此需要先通过一次迭代计算得到初始的、较大的dH值
    double dH=0;

    //计算N的初值
    N=a/(sqrt(1-e2*sin(B)*sin(B)));

    //根据公式计算迭代一次后得到的LBH坐标值，使用math.h中的计算函数
    L=atan2(Y,X);
    B=atan(Z/(sqrt(X*X+Y*Y)*(1-(e2*N)/(N+H))));
    H=sqrt(X*X+Y*Y)/cos(B)-N;

    //使用变量H0保存上一次迭代计算的H值并初始化为0
    double H0=0;

    //计算本次计算与上次迭代计算的H值之差，并取绝对值，以防出现负值导致跳出循环
    dH=abs(H-H0);

    //进行下一次迭代前更新H0值
    H0=H;
    do
        {
            //每一次迭代计算前先用前一次迭代得到的数据更新一次N的值
            N=a/(sqrt(1-e2*sin(B)*sin(B)));

            //根据公式计算迭代一次后得到的LBH坐标值，使用math.h中的计算函数
            L=atan2(Y,X);
            B=atan2(Z,sqrt(X*X+Y*Y)*(1-(e2*N)/(N+H0)));
            H=sqrt(X*X+Y*Y)/cos(B)-N;

            //计算本次计算与上次迭代计算的H值之差，并取绝对值，以防出现负值导致跳出循环
            dH=abs(H-H0);

            //进行下一次迭代前更新H0值
            H0=H;
        }
    while(dH>0.00001);
    //使用dH的条件进行迭代循环，当dH足够小之后跳出循环，LBH的值会已保存在对象中
}

//为实现较好的封装性和安全性，使用函数用于返回输出计算的结果坐标值
const double calculation2::showL()
{
    return L;
}
const double calculation2::showB()
{
    return B;
}
const double calculation2::showH()
{
    return H;
}

XYZ2LBH::XYZ2LBH(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::XYZ2LBH)
{
    ui->setupUi(this);
}

XYZ2LBH::~XYZ2LBH()
{
    delete ui;
}

//计算按钮按下时的计算程序
void XYZ2LBH::on_pushButton_clicked()
{
    //建立对象用于实现计算，同时从输入文本编辑框传入计算所使用的X、Y、Z坐标参数
    calculation2 d(ui->X->text().toDouble(),ui->Y->text().toDouble(),ui->Z->text().toDouble());

    //调用计算函数，将对象中的L、B、H值计算出来
    d.calculate();

    //根据输出格式，将L、B、H以浮点数形式在TextBrowser中输出
    //将输出的L、B值转换为合适的、以度分秒为单位的角度值，先从弧度值变为角度值，再调用先前定义的函数转换为度分秒的浮点数形式
    ui->B->setText(QString::number(tohour_min_sec((d.showB()*180)/pie),'f',8));
    ui->L->setText(QString::number(tohour_min_sec((d.showL()*180)/pie),'f',8));
    ui->H->setText(QString::number(d.showH(),'f',4));
}

//“重新输入”按钮按下时的清除程序，将所有的文本框清空
void XYZ2LBH::on_pushButton_2_clicked()//清空文本框，用于重新输入数据
{
    ui->B->clear();
    ui->L->clear();
    ui->H->clear();
    ui->X->clear();
    ui->Y->clear();
    ui->Z->clear();
}
