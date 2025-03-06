#include "func.h"
//自定义函数的函数体部分

double torad(double angle)
{
    //输入的形式是double，整数部分为度，小数点后前两位为分，后面剩余部分为秒
    //将单位为度、分、秒d的部分都取出
    double hour=floor(angle);//向下取整
    double min=floor(100*(angle-hour));
    double sec=100*(100*(angle-hour)-min);

    double temp=hour+min/60+sec/3600;//用变量temp表示以度为单位的角度值
    return (temp*pi)/180;//将以度为单位的角度值变为以弧度为单位的角度值
}

double cot(double angle)
{
    return 1/tan(angle);//余切值为对应正切值的倒数
}
