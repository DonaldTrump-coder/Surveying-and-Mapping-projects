#include "back_intersection.h"
#include "ui_back_intersection.h"
#include "func.h"//引用自定义函数头文件，用于处理数据

back_intersection::back_intersection(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::back_intersection)
{
    ui->setupUi(this);
}

back_intersection::~back_intersection()
{
    delete ui;
}

//定义用于传参的构造函数
calculation2::calculation2(double XA,double YA,double XB,double YB,double XC,double YC,double ALPHA,double BETA,double GAMMA)
{
    xa=XA;
    ya=YA;
    xb=XB;
    yb=YB;
    xc=XC;
    yc=YC;
    alpha=torad(ALPHA);
    beta=torad(BETA);
    gamma=torad(GAMMA);
    //将输入的度分秒表示的角度在初始化时就转为弧度的形式并存储
}

//用于计算的函数，是实现主要功能的部分
void calculation2::calculate()
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

    double dab=sqrt((xb-xa)*(xb-xa)+(yb-ya)*(yb-ya));
    double dbc=sqrt((xc-xb)*(xc-xb)+(yc-yb)*(yc-yb));
    double dac=sqrt((xc-xa)*(xc-xa)+(yc-ya)*(yc-ya));
    //计算A，B，C三点之间三条边的距离

    //用余弦公式计算A，B，C三个角度的大小
    double A=acos((dac*dac+dab*dab-dbc*dbc)/(2*dab*dac));
    double B=acos((dbc*dbc+dab*dab-dac*dac)/(2*dab*dbc));
    double C=acos((dac*dac+dbc*dbc-dab*dab)/(2*dbc*dac));

    //危险圆的判定需要对三个值的大小范围进行判断，其中每个值是三个角度之和
    //因此用三个double类型变量计算并存储该三个值的大小
    double d_danger1=alpha+beta+C;
    double d_danger2=beta+gamma+A;
    double d_danger3=gamma+alpha+B;
    //将各三个角度之和分别存入三个变量中

    //对三个变量的范围进行判断
    //若其中有一个变量位于170°与190°之间，说明P点在危险圆上，则无法用后方交会计算
    //可直接弹出警告框并结束计算（该警告框中的内容与输入数据格式出错时弹出的不同）
    if(torad(170)<d_danger1&&torad(190)>d_danger1)
    {
        warn();
        return;
    }
    if(torad(170)<d_danger2&&torad(190)>d_danger2)
    {
        warn();
        return;
    }
    if(torad(170)<d_danger3&&torad(190)>d_danger3)
    {
        warn();
        return;
    }

    //根据后方交会公式，计算三个p值
    double Pa=1/(cot(A)-cot(alpha));
    double Pb=1/(cot(B)-cot(beta));
    double Pc=1/(cot(C)-cot(gamma));

    //最后的目标点坐标相当于ABC三点坐标依据p进行加权平均
    xp=(Pa*xa+Pb*xb+Pc*xc)/(Pa+Pb+Pc);
    yp=(Pa*ya+Pb*yb+Pc*yc)/(Pa+Pb+Pc);
}

//为加强程序的封装性与安全性，用专门的函数来返回目标点的坐标
//且返回值的类型为const double，无法被修改
const double calculation2::showx()
{
    return xp;
}

const double calculation2::showy()
{
    return yp;
}

//检查输入格式是否有误的函数
void calculation2::test()
{
    if(alpha<=0||beta<=0||gamma<=0)
        wrong=1;
    //若输入的角度为负数，则视为有误
}

//用于弹出警告框的函数
void calculation2::warn()
{
    QMessageBox a;//建立消息框对象

    //在后方交会中，警告框被用于角度输入格式不正确、目标点在危险圆上的情况
    //且这两种情况是互斥的，因此需要对使用情况进行判断，对警告内容进行分类
    if(wrong==1)
        a.setText("请输入正确的角度值");
    //当wrong变量为1时，说明输入格式不正确，此时无需进行危险圆判断以及后续计算
    //因此提示应输入正确的角度值

    //当wrong变量不为1时却调用了该函数，说明只能是目标点在危险圆上
    //提示该点无法进行计算
    else
        a.setText("P点位于危险圆上！");

    //弹出警告框
    a.exec();

    //弹出警告框后无需进行计算，因此无目标点坐标的结果，需要将其置为默认值0
    xp=0;
    yp=0;
}

//用于计算并输出结果的函数
void back_intersection::on_pushButton_clicked()
{
    //在构造函数中用输入数据进行参数的初始化
    calculation2 a(ui->XA->text().toDouble(),ui->YA->text().toDouble(),ui->XB->text().toDouble(),ui->YB->text().toDouble(),ui->XC->text().toDouble(),ui->YC->text().toDouble(),ui->alpha->text().toDouble(),ui->beta->text().toDouble(),ui->gamma->text().toDouble());

    //调用该对象的计算函数，计算结果
    a.calculate();

    //结合对象的坐标返回函数进行目标点坐标的输出，结果保留三位小数
    ui->PX->setText(QString::number(a.showx(),'f',3));
    ui->PY->setText(QString::number(a.showy(),'f',3));
}

//清空所有输入、输出框的内容，以便重新输入
void back_intersection::on_pushButton_2_clicked()
{
    ui->PX->clear();
    ui->PY->clear();
    ui->XA->clear();
    ui->XB->clear();
    ui->XC->clear();
    ui->YA->clear();
    ui->YB->clear();
    ui->YC->clear();
    ui->alpha->clear();
    ui->beta->clear();
    ui->gamma->clear();
}

