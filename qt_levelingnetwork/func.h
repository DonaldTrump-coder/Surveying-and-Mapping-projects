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

#endif // FUN_H
