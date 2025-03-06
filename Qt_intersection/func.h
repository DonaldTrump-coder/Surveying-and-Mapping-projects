//头文件中声明额外的数据处理函数，在其它源文件中引用，以便后续重复调用
#ifndef FUNC_H
#define FUNC_H

#define pi 3.14159265358979323//定义圆周率

#include <math.h>             //需要用到math.h库中的函数
#include <QMessageBox>        //需要用到消息框

double torad(double angle);   //将度分秒表示的角度转化为弧度
double cot(double angle);     //math库中没有余切函数，需要自己定义

#endif // FUNC_H
