#ifndef FUN_H
#define FUN_H

#define pi 3.14159265358979323//定义圆周率
#define rou 206265            //定义一弧度转换为秒的系数

#include <math.h>             //需要用到math.h库中的函数
#include <QMessageBox>        //需要用到消息框

double torad(double angle);   //将度分秒表示的角度转化为弧度
double cot(double angle);     //math库中没有余切函数，需要自己定义
double todegminsec(double rad);//将弧度制角转换为以度分秒为单位的角
void warn(QString text);      //用于弹出（不同文本内容的）消息框的函数
int sgn(double a);            //符号函数

double azimuth(double xstart,double ystart,double xend,double yend);
//根据两点坐标推算方位角

void rad_normalize(double &rad);//将弧度制角的范围等效在0-2π内

//用模板定义交换的函数，可用于任何数据类型的排序
template<class T>
void swap1(T &a,T &b)
{
    T temp;//用临时变量存储其中一个待交换元素的值
    temp=a;
    a=b;
    b=temp;
}

#endif // FUN_H
