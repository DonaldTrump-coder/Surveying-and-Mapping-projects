#include "Angle.h"
#include "./ui_Angle.h"
#include<math.h>
const double pi=3.14159265358979323;//定义圆周率常量

//窗口类实现的构造、析构函数
Angle::Angle(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Angle)
{
    ui->setupUi(this);
}

Angle::~Angle()
{
    delete ui;
}
//

int sgn(double a)
{
    if(a>0)
    {
        return 1;
    }
    else if(a<0)
    {
        return -1;
    }
    else
        return 0;
}
//定义符号函数，根据正负性只返回-1，0或1
//用于后续使用该函数参与坐标象限的计算

void Angle::on_pushButton_clicked()
{
    //定义变量X1，X2，Y1，Y2，将手动输入的数值存入内存，并定义两点横、纵坐标之间的距离变量disX，disY
    double X1=ui->X1->text().toDouble();
    double Y1=ui->Y1->text().toDouble();
    double X2=ui->X2->text().toDouble();
    double Y2=ui->Y2->text().toDouble();
    double disY=Y2-Y1;
    double disX=X2-X1;

    //计算两点之间的距离
    double dis=sqrt(pow(disY,2)+pow(disX,2));

    //计算两点坐标的方位角
    double angle_rad=pi-sgn(disY)*atan2(sgn(disY)*disY,-1*disX);//使用一个公式及atan()函数即可解决象限问题
    //atan函数的值域范围大小为2π，充分利用该函数理论上可以直接完成所有方位角的计算，无需使用if语句判断

    //将弧度转换为角度
    double angle=angle_rad*180/pi;

    //将度取整
    int angle_hour=floor(angle);//向下取整
    //将分取整
    int angle_min=floor((angle-angle_hour)*60);
    //将秒取整
    int angle_sec=round(((angle-angle_hour)*60-angle_min)*60);
    //秒位后面不需要继续取整，因此在此处直接四舍五入

    if(disY==0)
    {
        if(disX>0)
        {
            angle_hour=angle_min=angle_sec=0;
        }
        if(disX<0)
        {
            angle_hour=180;
            angle_min=angle_sec=0;
        }
        if(disX==0)
        {
            ui->Dis->setText("0");
            ui->Angle_Edit->setText("当前坐标无法求方位角");
            return;
            //若两点为同一点，则可直接输出提示，并提前结束程序
        }
    }
    //检查特殊情况
    //当disY不为0，disX为0时，经检查不会出现问题，因此只检查disY=0时的情况

    //将度分秒的int型转换为字符串型
    QString s_angle_hour=QString::number(angle_hour);
    QString s_angle_min=QString::number(angle_min);
    QString s_angle_sec=QString::number(angle_sec);

    //同时额外输出两点之间距离，该距离保留三位小数
    ui->Dis->setText(QString::number(dis,'f',3));

    //将取整后的度分秒输出为文本形式并和符号链接，添加度、分、秒符号使输出更加明确
    ui->Angle_Edit->setText(s_angle_hour+"°"+s_angle_min+"'"+s_angle_sec+"\"");
}
